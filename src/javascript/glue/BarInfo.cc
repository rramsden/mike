#include "javascript/glue/BarInfo.h"
#include "Browser.h"
#include "Window.h"
#include "Frame.h"
#include "html/HtmlPage.h"

namespace mike {
  namespace glue
  {
    Handle<FunctionTemplate> BarInfo::NewTemplate()
    {
      Handle<FunctionTemplate> t = JS_FUNC_TPL();
      t->SetClassName(JS_STR("BarInfo"));

      // Prototype
      Handle<Template> proto_t = t->PrototypeTemplate();

      // Instance
      Handle<ObjectTemplate> instance_t = t->InstanceTemplate();
      instance_t->SetAccessor(JS_STR("visible"), JS_GetVisible);

      return t;
    }

    Handle<Object> BarInfo::New()
    {
      return NewTemplate()->GetFunction()->NewInstance();
    }

    //============================= PROPERTIES ===================================

    JS_GETTER(BarInfo, Visible) // visible
    {
      return JS_FALSE;
    }
    JS_END
  }
}
