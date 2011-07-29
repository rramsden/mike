#include <assert.h>
#include <exception>

#include "javascript/JavaScriptHandler.h"
#include "javascript/Macros.h"
#include "html/HtmlPage.h"
#include "html/HtmlElement.h"
#include "Browser.h"
#include "Frame.h"

#include "javascript/glue/WindowWrap.h"

namespace mike
{
  JavaScriptHandler::JavaScriptHandler(HtmlPage* page)
  {
    HandleScope scope;
    page_ = page;

    Frame* frame = page_->getEnclosingFrame();

    Persistent<Context> proxy = Context::New();
    proxy->Enter();
    
    Handle<FunctionTemplate> window_tpl = glue::WindowWrap::NewTemplate();
    Handle<Object> window = window_tpl->GetFunction()->NewInstance();

    // Setting up new context with window object as a global.
    context_ = Context::New(NULL, window_tpl->InstanceTemplate());
    context_->ReattachGlobal(window);
    
    proxy->Exit();
    proxy.Dispose();

    Handle<Object> global = context_->Global();
    Handle<Object> global_proto = Handle<Object>::Cast(global->GetPrototype());
    
    // Wrap enclosing window within global object.
    glue::ObjectWrap::Wrap<Frame>(global, frame, 0);
    global_proto->Set(JS_STR("self"), global);
    
    // If there is any context already entered, we can assume that we're setting up
    // context for frame here. That's mean we have to create accessors for this frame
    // within parent window.
    if (frame != frame->getParent()) {
      HtmlPage* parent_page = frame->getParent()->getPage()->asHtml();
      Handle<Context> parent_cxt = parent_page->javaScriptHandler_->context_;

      parent_cxt->Enter();
      
      Handle<Object> parent_window = parent_cxt->Global();
      parent_cxt->SetSecurityToken(context_->GetSecurityToken());
      parent_window->Set(frame->getIndex(), window);

      if (!frame->getName().empty())
	parent_window->Set(JS_STR(frame->getName().c_str()), window);

      parent_cxt->Exit();
    }
  }

  JavaScriptHandler::~JavaScriptHandler()
  {
    context_.Dispose();
  }

  string JavaScriptHandler::evaluate(string source, string fname, unsigned int line)
  {
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
	Handle<Value> err = try_catch.Exception();

	if (!err->IsString()) {
	  Handle<Object> exp = Handle<Object>::Cast(err);
	  page_->getEnclosingWindow()->getBrowser()->clearExpectations();
	    
	  if (!exp.IsEmpty()) {
	    PopupType type = static_cast<PopupType>(exp->Get(String::New("expectation"))->Int32Value());
	    String::Utf8Value msg(exp->Get(String::New("message"))->ToString());

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
}
