#include "javascript/glue/NavigatorWrap.h"
#include "Browser.h"
#include "Window.h"
#include "Frame.h"
#include "html/HtmlPage.h"

namespace mike {
  namespace glue
  {
    Handle<FunctionTemplate> NavigatorWrap::NewTemplate()
    {
      Handle<FunctionTemplate> t = JS_FUNC_TPL();
      t->SetClassName(JS_STR("Navigator"));

      // Prototype
      Handle<Template> proto_t = t->PrototypeTemplate();

      // Instance
      Handle<ObjectTemplate> instance_t = t->InstanceTemplate();
      instance_t->SetAccessor(JS_STR("appCodeName"), JS_GetAppCodeName);
      instance_t->SetAccessor(JS_STR("cookieEnabled"), JS_GetCookieEnabled);
      instance_t->SetAccessor(JS_STR("language"), JS_GetLanguage);
      instance_t->SetAccessor(JS_STR("onLine"), JS_GetOnLine);

      return t;
    }

    Handle<Object> NavigatorWrap::New()
    {
      return NewTemplate()->GetFunction()->NewInstance();
    }

    //============================= PROPERTIES ===================================

    JS_GETTER(NavigatorWrap, AppCodeName) // appCodeName
    {
      return JS_STR("Mozilla");
    }
    JS_END

    JS_GETTER(NavigatorWrap, CookieEnabled) // cookieEnabled
    {
      JS_UNWRAP_WINDOW;
      return JS_BOOL(window->getBrowser()->isCookieEnabled());
    }
    JS_END

    JS_GETTER(NavigatorWrap, Language) // language
    {
      JS_UNWRAP_WINDOW;
      return JS_STR(window->getBrowser()->getLanguage().c_str());
    }
    JS_END

    JS_GETTER(NavigatorWrap, OnLine) // onLine
    {
      JS_UNWRAP_WINDOW;
      return JS_BOOL(window->getBrowser()->isOnLine());
    }
    JS_END

    //============================= FUNCTIONS  ===================================

    // ...
  }
}
