#include "javascript/glue/WindowWrap.h"
#include "Browser.h"
#include "Window.h"
#include "Frame.h"
#include "html/HtmlPage.h"

namespace mike {
  namespace glue
  {
    Handle<FunctionTemplate> WindowWrap::NewTemplate()
    {
      Handle<FunctionTemplate> t = JS_FUNC_TPL();
      t->SetClassName(JS_STR("DOMWindow"));

      // Prototype
      Handle<Template> proto_t = t->PrototypeTemplate();
      proto_t->Set(JS_STR("alert"), JS_FUNC_TPL(JS_Alert));
      proto_t->Set(JS_STR("confirm"), JS_FUNC_TPL(JS_Confirm));
      proto_t->Set(JS_STR("prompt"), JS_FUNC_TPL(JS_Prompt));
      proto_t->Set(JS_STR("close"), JS_FUNC_TPL(JS_Close));

      // Instance
      Handle<ObjectTemplate> instance_t = t->InstanceTemplate();
      instance_t->SetAccessor(JS_STR("window"), JS_GetWindow);
      instance_t->SetAccessor(JS_STR("frames"), JS_GetWindow); // frames refers to window
      instance_t->SetAccessor(JS_STR("length"), JS_GetLength);
      instance_t->SetAccessor(JS_STR("parent"), JS_GetParent);
      instance_t->SetAccessor(JS_STR("top"), JS_GetTop);
      instance_t->SetAccessor(JS_STR("closed"), JS_GetClosed);
      instance_t->SetAccessor(JS_STR("status"), JS_GetStatus, JS_SetStatus);
      instance_t->Set(JS_STR("defaultStatus"), JS_STR(""));
      instance_t->Set(JS_STR("defaultstatus"), JS_STR(""));

      instance_t->SetInternalFieldCount(2);

      return t;
    }

    //============================= PROPERTIES ===================================
    
    JS_GETTER(WindowWrap, Window) // window, frame
    {
      return JS_I_HOLDER->Get(JS_STR("self"));
    }
    JS_END

    JS_GETTER(WindowWrap, Length) // length
    {
      JS_I_UNWRAP_HOLDER(Frame);

      if (!self->isBlank()) {
	return JS_INT(self->getPage()->asHtml()->getFrames().size());
      } else {
	return JS_INT(0);
      }
    }
    JS_END

    JS_GETTER(WindowWrap, Parent) // parent
    {
      JS_I_UNWRAP_HOLDER(Frame);
      HtmlPage* page = self->getParent()->getPage()->asHtml();
      return page->getScriptContext()->Global();
    }
    JS_END

    JS_GETTER(WindowWrap, Top) // top
    {
      JS_I_UNWRAP_HOLDER(Frame);
      HtmlPage* page = self->getTop()->getPage()->asHtml();
      return page->getScriptContext()->Global();
    }
    JS_END

    JS_GETTER(WindowWrap, Closed) // closed
    {
      JS_I_UNWRAP_HOLDER(Frame);
      return JS_BOOL(self->isClosed());
    }
    JS_END

    JS_GETTER(WindowWrap, Status)
    {
      JS_I_UNWRAP_HOLDER(Frame);
      return JS_STR(self->getStatus().c_str());
    }
    JS_END

    JS_SETTER(WindowWrap, Status)
    {
      JS_I_UNWRAP_HOLDER(Frame);
      self->setStatus(JS_TO_UTF8(value->ToString()));
    }
    JS_END

    //============================= FUNCTIONS  ===================================

    JS_FUNCTION(WindowWrap, Close) // close()
    {
      JS_UNWRAP_HOLDER(Frame);
      self->close();
      
      // We have to throw exception to stop javascript execution on this page first.
      // Window will be closed from the javascript handler.
      Handle<Object> err(JS_OBJ());
      err->Set(JS_STR("close"), JS_INT(1));

      JS_THROW_OBJ(err);
    }
    JS_END
    
    JS_FUNCTION(WindowWrap, Alert) // alert(msg)
    {
      JS_ARG_UTF8(message, 0);
      JS_UNWRAP_HOLDER(Frame);

      list<PopupExpectation>& expects = self->getBrowser()->getExpectations();

      // Check if browser was expecting this alert.
      if (!expects.empty()) {
	PopupExpectation e = expects.front();
	expects.pop_front();

	if (e.kind == kPopupAlert) {
	  bool match_msg = (e.flags & kMatchMessage) == kMatchMessage;

	  if (!match_msg || (JS_ARGC > 0 && e.message == message))
	    return JS_UNDEF;
	}
      }

      // If alert was unexpected then throw v8 exception to disallow
      // continue execution.
      Handle<Object> err(JS_OBJ());
      err->Set(JS_STR("expectation"), JS_INT(kPopupAlert));
      err->Set(JS_STR("message"), JS_STR(message.c_str()));
      
      JS_THROW_OBJ(err);
    }    
    JS_END

    JS_FUNCTION(WindowWrap, Confirm) // confirm(msg)
    {
      JS_ARG_UTF8(message, 0);
      JS_UNWRAP_HOLDER(Frame);

      list<PopupExpectation>& expects = self->getBrowser()->getExpectations();

      // Check if browser was expecting this confirmation.
      if (!expects.empty()) {
	PopupExpectation e = expects.front();
	expects.pop_front();

	if (e.kind == kPopupConfirm) {
	  bool match_msg = (e.flags & kMatchMessage) == kMatchMessage;

	  if (!match_msg || (JS_ARGC > 0 && e.message == message))
	    return JS_BOOL((e.choice == "true"));
	}
      }

      // If confirmation was unexpected then throw v8 exception to disallow
      // continue execution.
      Handle<Object> err(JS_OBJ());
      err->Set(JS_STR("expectation"), JS_INT(kPopupConfirm));
      err->Set(JS_STR("message"), JS_STR(message.c_str()));

      JS_THROW_OBJ(err);
    }
    JS_END

    JS_FUNCTION(WindowWrap, Prompt) // prompt(msg)
    {
      JS_ARG_UTF8(message, 0);
      JS_UNWRAP_HOLDER(Frame);

      list<PopupExpectation>& expects = self->getBrowser()->getExpectations();

      // Check if browser was expecting this prompt.
      if (!expects.empty()) {
	PopupExpectation e = expects.front();
	expects.pop_front();

	if (e.kind == kPopupPrompt) {
	  bool match_msg = (e.flags & kMatchMessage) == kMatchMessage;

	  if (!match_msg || (JS_ARGC > 0 && e.message == message)) {
	    if ((e.flags & kCancelPrompt) == kCancelPrompt) {
	      return JS_NULL;
	    } else
	      return JS_STR(e.choice.c_str());
	  }
	}
      }

      // If prompt was unexpected then throw v8 exception to disallow
      // continue execution.
      Handle<Object> err(JS_OBJ());
      err->Set(JS_STR("expectation"), JS_INT(kPopupPrompt));
      err->Set(JS_STR("message"), JS_STR(message.c_str()));
      
      JS_THROW_OBJ(err);
    }
    JS_END
  }
}
