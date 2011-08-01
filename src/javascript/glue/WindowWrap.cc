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
      proto_t->Set(JS_STR("print"), JS_FUNC_TPL(JS_Print));
      proto_t->Set(JS_STR("find"), JS_FUNC_TPL(JS_Find));

      // Instance
      Handle<ObjectTemplate> instance_t = t->InstanceTemplate();
      instance_t->SetAccessor(JS_STR("window"), JS_GetWindow);
      instance_t->SetAccessor(JS_STR("frames"), JS_GetWindow); // frames refers to window
      instance_t->SetAccessor(JS_STR("length"), JS_GetLength);
      instance_t->SetAccessor(JS_STR("parent"), JS_GetParent);
      instance_t->SetAccessor(JS_STR("top"), JS_GetTop);
      instance_t->SetAccessor(JS_STR("opener"), JS_GetOpener);
      instance_t->SetAccessor(JS_STR("closed"), JS_GetClosed);
      instance_t->SetAccessor(JS_STR("status"), JS_GetStatus, JS_SetStatus);
      instance_t->SetAccessor(JS_STR("name"), JS_GetName);
      instance_t->SetAccessor(JS_STR("innerWidth"), JS_GetInnerWidth);
      instance_t->SetAccessor(JS_STR("innerHeight"), JS_GetInnerHeight);
      instance_t->SetAccessor(JS_STR("outerWidth"), JS_GetOuterWidth);
      instance_t->SetAccessor(JS_STR("outerHeight"), JS_GetOuterHeight);
      // ... window events (TODO: figure out which one are webkit-only!)
      instance_t->SetAccessor(JS_STR("onabort"), JS_GetEventCallback, JS_SetEventCallback);
      instance_t->SetAccessor(JS_STR("onbeforeunload"), JS_GetEventCallback, JS_SetEventCallback);
      instance_t->SetAccessor(JS_STR("onblur"), JS_GetEventCallback, JS_SetEventCallback);
      instance_t->SetAccessor(JS_STR("oncanplay"), JS_GetEventCallback, JS_SetEventCallback);
      instance_t->SetAccessor(JS_STR("oncanplaythrough"), JS_GetEventCallback, JS_SetEventCallback);
      instance_t->SetAccessor(JS_STR("onchange"), JS_GetEventCallback, JS_SetEventCallback);
      instance_t->SetAccessor(JS_STR("onclick"), JS_GetEventCallback, JS_SetEventCallback);
      instance_t->SetAccessor(JS_STR("oncontextmenu"), JS_GetEventCallback, JS_SetEventCallback);
      instance_t->SetAccessor(JS_STR("ondblclick"), JS_GetEventCallback, JS_SetEventCallback);
      instance_t->SetAccessor(JS_STR("ondeviceorientation"), JS_GetEventCallback, JS_SetEventCallback);
      instance_t->SetAccessor(JS_STR("ondrag"), JS_GetEventCallback, JS_SetEventCallback);
      instance_t->SetAccessor(JS_STR("ondragend"), JS_GetEventCallback, JS_SetEventCallback);
      instance_t->SetAccessor(JS_STR("ondragenter"), JS_GetEventCallback, JS_SetEventCallback);
      instance_t->SetAccessor(JS_STR("ondragleave"), JS_GetEventCallback, JS_SetEventCallback);
      instance_t->SetAccessor(JS_STR("ondragover"), JS_GetEventCallback, JS_SetEventCallback);
      instance_t->SetAccessor(JS_STR("ondragstart"), JS_GetEventCallback, JS_SetEventCallback);
      instance_t->SetAccessor(JS_STR("ondrop"), JS_GetEventCallback, JS_SetEventCallback);
      instance_t->SetAccessor(JS_STR("ondurationchange"), JS_GetEventCallback, JS_SetEventCallback);
      instance_t->SetAccessor(JS_STR("onemptied"), JS_GetEventCallback, JS_SetEventCallback);
      instance_t->SetAccessor(JS_STR("onended"), JS_GetEventCallback, JS_SetEventCallback);
      instance_t->SetAccessor(JS_STR("onerror"), JS_GetEventCallback, JS_SetEventCallback);
      instance_t->SetAccessor(JS_STR("onfocus"), JS_GetEventCallback, JS_SetEventCallback);
      instance_t->SetAccessor(JS_STR("onhashchange"), JS_GetEventCallback, JS_SetEventCallback);
      instance_t->SetAccessor(JS_STR("oninput"), JS_GetEventCallback, JS_SetEventCallback);
      instance_t->SetAccessor(JS_STR("oninvalid"), JS_GetEventCallback, JS_SetEventCallback);
      instance_t->SetAccessor(JS_STR("onkeydown"), JS_GetEventCallback, JS_SetEventCallback);
      instance_t->SetAccessor(JS_STR("onkeypress"), JS_GetEventCallback, JS_SetEventCallback);
      instance_t->SetAccessor(JS_STR("onkeyup"), JS_GetEventCallback, JS_SetEventCallback);
      instance_t->SetAccessor(JS_STR("onload"), JS_GetEventCallback, JS_SetEventCallback);
      instance_t->SetAccessor(JS_STR("onloadeddata"), JS_GetEventCallback, JS_SetEventCallback);
      instance_t->SetAccessor(JS_STR("onloadedmetadata"), JS_GetEventCallback, JS_SetEventCallback);
      instance_t->SetAccessor(JS_STR("onloadstart"), JS_GetEventCallback, JS_SetEventCallback);
      instance_t->SetAccessor(JS_STR("onmessage"), JS_GetEventCallback, JS_SetEventCallback);
      instance_t->SetAccessor(JS_STR("onmousedown"), JS_GetEventCallback, JS_SetEventCallback);
      instance_t->SetAccessor(JS_STR("onmousemove"), JS_GetEventCallback, JS_SetEventCallback);
      instance_t->SetAccessor(JS_STR("onmouseout"), JS_GetEventCallback, JS_SetEventCallback);
      instance_t->SetAccessor(JS_STR("onmouseover"), JS_GetEventCallback, JS_SetEventCallback);
      instance_t->SetAccessor(JS_STR("onmouseup"), JS_GetEventCallback, JS_SetEventCallback);
      instance_t->SetAccessor(JS_STR("onmousewheel"), JS_GetEventCallback, JS_SetEventCallback);
      instance_t->SetAccessor(JS_STR("onoffline"), JS_GetEventCallback, JS_SetEventCallback);
      instance_t->SetAccessor(JS_STR("ononline"), JS_GetEventCallback, JS_SetEventCallback);
      instance_t->SetAccessor(JS_STR("onpagehide"), JS_GetEventCallback, JS_SetEventCallback);
      instance_t->SetAccessor(JS_STR("onpageshow"), JS_GetEventCallback, JS_SetEventCallback);
      instance_t->SetAccessor(JS_STR("onpause"), JS_GetEventCallback, JS_SetEventCallback);
      instance_t->SetAccessor(JS_STR("onplay"), JS_GetEventCallback, JS_SetEventCallback);
      instance_t->SetAccessor(JS_STR("onplaying"), JS_GetEventCallback, JS_SetEventCallback);
      instance_t->SetAccessor(JS_STR("onpopstate"), JS_GetEventCallback, JS_SetEventCallback);
      instance_t->SetAccessor(JS_STR("onprogress"), JS_GetEventCallback, JS_SetEventCallback);
      instance_t->SetAccessor(JS_STR("onratechange"), JS_GetEventCallback, JS_SetEventCallback);
      instance_t->SetAccessor(JS_STR("onreset"), JS_GetEventCallback, JS_SetEventCallback);
      instance_t->SetAccessor(JS_STR("onresize"), JS_GetEventCallback, JS_SetEventCallback);
      instance_t->SetAccessor(JS_STR("onscroll"), JS_GetEventCallback, JS_SetEventCallback);
      instance_t->SetAccessor(JS_STR("onsearch"), JS_GetEventCallback, JS_SetEventCallback);
      instance_t->SetAccessor(JS_STR("onseeked"), JS_GetEventCallback, JS_SetEventCallback);
      instance_t->SetAccessor(JS_STR("onseeking"), JS_GetEventCallback, JS_SetEventCallback);
      instance_t->SetAccessor(JS_STR("onselect"), JS_GetEventCallback, JS_SetEventCallback);
      instance_t->SetAccessor(JS_STR("onstalled"), JS_GetEventCallback, JS_SetEventCallback);
      instance_t->SetAccessor(JS_STR("onstorage"), JS_GetEventCallback, JS_SetEventCallback);
      instance_t->SetAccessor(JS_STR("onsubmit"), JS_GetEventCallback, JS_SetEventCallback);
      instance_t->SetAccessor(JS_STR("onsuspend"), JS_GetEventCallback, JS_SetEventCallback);
      instance_t->SetAccessor(JS_STR("ontimeupdate"), JS_GetEventCallback, JS_SetEventCallback);
      instance_t->SetAccessor(JS_STR("onunload"), JS_GetEventCallback, JS_SetEventCallback);
      instance_t->SetAccessor(JS_STR("onvolumechange"), JS_GetEventCallback, JS_SetEventCallback);
      instance_t->SetAccessor(JS_STR("onwaiting"), JS_GetEventCallback, JS_SetEventCallback);
      // ... attributes
      instance_t->Set(JS_STR("defaultStatus"), JS_STR(""));
      instance_t->Set(JS_STR("defaultstatus"), JS_STR(""));
      instance_t->Set(JS_STR("undefined"), JS_UNDEF);

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
      return self->getParent()->getScriptContext()->Global();
    }
    JS_END

    JS_GETTER(WindowWrap, Top) // top
    {
      JS_I_UNWRAP_HOLDER(Frame);
      return self->getTop()->getScriptContext()->Global();
    }
    JS_END

    JS_GETTER(WindowWrap, Opener) // opener
    {
      JS_I_UNWRAP_HOLDER(Frame);
      
      if (self->hasOpener())
	return self->getOpener()->getScriptContext()->Global();
      else
	return JS_NULL;
    }
    JS_END

    JS_GETTER(WindowWrap, Closed) // closed
    {
      JS_I_UNWRAP_HOLDER(Frame);
      return JS_BOOL(self->isClosed());
    }
    JS_END

    JS_GETTER(WindowWrap, Status) // status
    {
      JS_I_UNWRAP_HOLDER(Frame);
      return JS_STR(self->getStatus().c_str());
    }
    JS_END

    JS_SETTER(WindowWrap, Status) // status
    {
      JS_I_UNWRAP_HOLDER(Frame);
      self->setStatus(JS_TO_UTF8(value->ToString()));
    }
    JS_END

    JS_GETTER(WindowWrap, Name) // name
    {
      JS_I_UNWRAP_HOLDER(Frame);
      return JS_STR(self->getName().c_str());
    }
    JS_END

    JS_GETTER(WindowWrap, InnerWidth) // innerWidth
    {
      JS_I_UNWRAP_HOLDER(Frame);
      return JS_INT(self->getWidth());
    }
    JS_END
    
    JS_GETTER(WindowWrap, InnerHeight) // innerHeight
    {
      JS_I_UNWRAP_HOLDER(Frame);
      return JS_INT(self->getHeight());
    }
    JS_END
    
    JS_GETTER(WindowWrap, OuterWidth) // outerWidth
    {
      JS_I_UNWRAP_HOLDER(Frame);
      return JS_INT(self->getTop()->getWidth());
    }
    JS_END
    
    JS_GETTER(WindowWrap, OuterHeight) // outerHeight
    {
      JS_I_UNWRAP_HOLDER(Frame);
      return JS_INT(self->getTop()->getHeight());
    }
    JS_END
    
    JS_GETTER(WindowWrap, EventCallback) // on*
    {
      JS_I_UNWRAP_HOLDER(Frame);
      return self->getScriptHandler()->getEventCallback(JS_TO_STR(property));
    }
    JS_END
    
    JS_SETTER(WindowWrap, EventCallback) // on*
    {
      JS_I_UNWRAP_HOLDER(Frame);
      self->getScriptHandler()->setEventCallback(JS_TO_STR(property), value);
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

    JS_FUNCTION(WindowWrap, Print) // print()
    {
      return JS_UNDEF;
    }
    JS_END

    JS_FUNCTION(WindowWrap, Find) // find()
    {
      return JS_FALSE;
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
