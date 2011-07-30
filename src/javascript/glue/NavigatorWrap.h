#ifndef _MIKE_JAVSASCRIPT_NAVIGATOR_WRAP_H_
#define _MIKE_JAVSASCRIPT_NAVIGATOR_WRAP_H_

#include "javascript/glue/ObjectWrap.h"

namespace mike
{
  class Browser;
  
  namespace glue
  {
    class NavigatorWrap : public ObjectWrap
    {
    public:
      static Handle<FunctionTemplate> NewTemplate();
      static Handle<Object> New();

      // Properties
      JS_GETTER_D(AppCodeName);
      JS_GETTER_D(CookieEnabled);
      JS_GETTER_D(Language);
      JS_GETTER_D(OnLine);
      
      // Functions
      
    };
  }
}

#endif /* _MIKE_JAVSASCRIPT_NAVIGATOR_WRAP_H_ */
