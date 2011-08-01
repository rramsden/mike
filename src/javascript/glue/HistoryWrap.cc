#include "javascript/glue/HistoryWrap.h"
#include "Browser.h"
#include "Window.h"
#include "Frame.h"
#include "html/HtmlPage.h"

namespace mike {
  namespace glue
  {
    Handle<FunctionTemplate> HistoryWrap::NewTemplate()
    {
      Handle<FunctionTemplate> t = JS_FUNC_TPL();
      t->SetClassName(JS_STR("History"));

      // Prototype
      Handle<Template> proto_t = t->PrototypeTemplate();
      proto_t->Set(JS_STR("back"), JS_FUNC_TPL(JS_Back));
      proto_t->Set(JS_STR("forward"), JS_FUNC_TPL(JS_Forward));
      proto_t->Set(JS_STR("go"), JS_FUNC_TPL(JS_Go));
      
      // Instance
      Handle<ObjectTemplate> instance_t = t->InstanceTemplate();
      instance_t->SetAccessor(JS_STR("length"), JS_GetLength);

      return t;
    }

    Handle<Object> HistoryWrap::New()
    {
      return NewTemplate()->GetFunction()->NewInstance();
    }

    //============================= PROPERTIES ===================================

    JS_GETTER(HistoryWrap, Length) // length
    {
      JS_UNWRAP_WINDOW;
      return JS_INT(window->getHistory().size());
    }
    JS_END

    //============================= FUNCTIONS  ===================================

    JS_FUNCTION(HistoryWrap, Back)
    {
      JS_UNWRAP_WINDOW;
      window->getHistory().goBack();
      StopExecution();
      return JS_UNDEF;
    }
    JS_END

    JS_FUNCTION(HistoryWrap, Forward)
    {
      JS_UNWRAP_WINDOW;
      window->getHistory().goForward();
      StopExecution();
      return JS_UNDEF;
    }
    JS_END

    JS_FUNCTION(HistoryWrap, Go)
    {
      JS_UNWRAP_WINDOW;
      JS_ARG_INT(jump, 0);
      window->getHistory().go(jump);
      StopExecution();
      return JS_UNDEF;
    }
    JS_END
  }
}
