#include "javascript/glue/WindowWrap.h"
#include "Browser.h"
#include "Window.h"
#include "Frame.h"
#include "html/HtmlPage.h"

namespace mike {
  namespace glue
  {
    //============================= BUILDER    ===================================

    Handle<FunctionTemplate> WindowWrap::NewTemplate()
    {
      Handle<FunctionTemplate> t = JS_FUNC_TPL();
      t->SetClassName(JS_STR("DOMWindow"));

      // Prototype
      Handle<Template> proto_t = t->PrototypeTemplate();
      proto_t->Set(JS_STR("alert"), JS_FUNC_TPL(JS_Alert));
      proto_t->Set(JS_STR("confirm"), JS_FUNC_TPL(JS_Confirm));
      proto_t->Set(JS_STR("prompt"), JS_FUNC_TPL(JS_Prompt));

      // Instance
      Handle<ObjectTemplate> instance_t = t->InstanceTemplate();
      instance_t->SetAccessor(JS_STR("window"), JS_GetWindow);
      instance_t->SetAccessor(JS_STR("frames"), JS_GetWindow); // frames refers to window
      instance_t->SetAccessor(JS_STR("length"), JS_GetLength);
      instance_t->SetAccessor(JS_STR("parent"), JS_GetParent);
      instance_t->SetAccessor(JS_STR("top"), JS_GetTop);

      instance_t->SetInternalFieldCount(2);

      return t;
    }

    //============================= PROPERTIES ===================================
    
    JS_GETTER(WindowWrap, Window) // window, frame
    {
      return info.Holder()->Get(JS_STR("self"));
    }
    JS_END

    JS_GETTER(WindowWrap, Length) // length
    {
      Frame* self = Unwrap<Window>(info.Holder());
      HtmlPage* page = static_cast<HtmlPage*>(self->getPage());

      if (!self->isBlank()) {
	return JS_INT(page->getFrames().size());
      } else {
	return JS_INT(0);
      }
    }
    JS_END

    JS_GETTER(WindowWrap, Parent) // parent
    {
      Frame* self = Unwrap<Window>(info.Holder());
      Frame* parent = self->getParent();
      HtmlPage* page = parent->getPage()->asHtml();
      return page->javaScriptHandler_->context_->Global();
    }
    JS_END

    JS_GETTER(WindowWrap, Top) // top
    {
      Frame* self = Unwrap<Window>(info.Holder());
      Frame* top = self->getTop();
      HtmlPage* page = top->getPage()->asHtml();
      return page->javaScriptHandler_->context_->Global();
    }
    JS_END

    //============================= FUNCTIONS  ===================================

    JS_FUNCTION(WindowWrap, Alert) // alert(msg)
    {
      JS_ARG_UTF8(message, 0);
      Frame* self = Unwrap<Window>(JS_HOLDER);
      list<PopupExpectation>& expects = self->getBrowser()->expectedPopups_;

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
      Frame* self = Unwrap<Window>(JS_HOLDER);
      list<PopupExpectation>& expects = self->getBrowser()->expectedPopups_;

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
      Frame* self = Unwrap<Window>(JS_HOLDER);
      list<PopupExpectation>& expects = self->getBrowser()->expectedPopups_;

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
