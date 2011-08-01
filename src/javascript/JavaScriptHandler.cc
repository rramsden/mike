#include <assert.h>
#include <exception>

#include "javascript/JavaScriptHandler.h"
#include "javascript/Macros.h"
#include "html/HtmlPage.h"
#include "html/HtmlElement.h"
#include "Browser.h"
#include "Frame.h"

#include "javascript/glue/WindowWrap.h"
#include "javascript/glue/NavigatorWrap.h"
#include "javascript/glue/BarInfo.h"

namespace mike
{
  JavaScriptHandler::JavaScriptHandler(HtmlPage* page)
  {
    HandleScope scope;
    page_ = page;

    Frame* frame = page_->getEnclosingFrame();

    // We have to use proxy context to create proper global (window) object and
    // use it in target context. 
    Persistent<Context> proxy = Context::New();
    proxy->Enter();
    
    Handle<FunctionTemplate> window_tpl = glue::WindowWrap::NewTemplate();
    Persistent<Object> window_ = Persistent<Object>::New(window_tpl->GetFunction()->NewInstance());

    // Setting up target context with window template as a global...
    context_ = Context::New(NULL, window_tpl->InstanceTemplate());

    // ... and now we have to reattach global object with window instance created
    // in proxy context.
    context_->ReattachGlobal(window_);

    // Don't need proxy context anymore.
    proxy->Exit();
    proxy.Dispose();

    Handle<Object> global = context_->Global();
    Handle<Object> global_proto = Handle<Object>::Cast(global->GetPrototype());
    
    // Global object have to wrap frame containing current page...
    glue::ObjectWrap::Wrap<Frame>(global, frame, 0);

    context_->Enter();
    
    // ... and we have to assign bunch of properties at runtime.
    global_proto->Set(JS_STR("self"), global);
    global_proto->Set(JS_STR("navigator"), glue::NavigatorWrap::New());
    global_proto->Set(JS_STR("menubar"), glue::BarInfo::New());
    global_proto->Set(JS_STR("toolbar"), glue::BarInfo::New());
    global_proto->Set(JS_STR("locationbar"), glue::BarInfo::New());
    global_proto->Set(JS_STR("personalbar"), glue::BarInfo::New());
    global_proto->Set(JS_STR("scrollbars"), glue::BarInfo::New());
    global_proto->Set(JS_STR("statusbar"), glue::BarInfo::New());

    context_->Exit();
    
    // If current page is enclosed in internal frame we have to make a tie between its
    // parent window. 
    if (frame != frame->getParent()) {
      HtmlPage* parent_page = frame->getParent()->getPage()->asHtml();
      Handle<Context> parent_cxt = parent_page->getScriptContext();

      parent_cxt->Enter();

      // Contexts have to share the same security token.
      parent_cxt->SetSecurityToken(context_->GetSecurityToken());

      Handle<Object> parent_window = parent_cxt->Global();
      parent_window->Set(frame->getIndex(), global);

      if (!frame->getName().empty())
	parent_window->Set(JS_STR(frame->getName().c_str()), global);

      parent_cxt->Exit();
    }
  }

  JavaScriptHandler::~JavaScriptHandler()
  {
    eventCallbacks_.clear();
    window_.Dispose();
    context_.Dispose();
  }

  // TODO: this part is getting ugly... but so far it works so i don't give a shit :P
  string JavaScriptHandler::evaluate(string source, string fname, unsigned int line)
  {
    //Locker lock;
    HandleScope scope;
    Context::Scope context_scope(context_);

    string out = "";
      
    Local<String> s = String::New(source.c_str());
    Local<String> n = String::New(fname.c_str());
    
    TryCatch try_catch;
    Local<Script> script = Script::Compile(s, n);
      
    if (try_catch.HasCaught()) {
      printf("JS: Compile error\n");
    } else {
      Handle<Value> result = script->Run();

      if (try_catch.HasCaught()) {
	Unlocker unlock;
	Handle<Value> err = try_catch.Exception();

	if (!err->IsString()) {
	  Handle<Object> erro = Handle<Object>::Cast(err);

	  if (erro->Has(JS_STR("close"))) {
	    throw CloseWindow();
	  } else if (erro->Has(JS_STR("expectation"))) {
	    page_->getEnclosingWindow()->getBrowser()->clearExpectations();
	    PopupType type = static_cast<PopupType>(erro->Get(JS_STR("expectation"))->Int32Value());
	    String::Utf8Value msg(erro->Get(JS_STR("message"))->ToString());

	    // Throw info about unexpected popup!
	    throw UnexpectedPopupError(type, string(*msg));
	  }
	} else {
	  printf("JS: Runtime error\n");
	}
      } else {
	String::Utf8Value str(result->ToString());
	  
	if (*str)
	  out = string(*str);
      }
    }

    return out;
  }

  Handle<Context> JavaScriptHandler::getContext()
  {
    return context_;
  }

  Handle<Value> JavaScriptHandler::getEventCallback(string name)
  {
    map< string, Handle<Value> >::iterator found = eventCallbacks_.find(name);

    if (found == eventCallbacks_.end())
      return JS_UNDEF;
    else
      return (*found).second;
  }
  
  void JavaScriptHandler::setEventCallback(string name, Handle<Value> callback)
  {
    eventCallbacks_[name] = callback;
  }

}
