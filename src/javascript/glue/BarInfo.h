#ifndef _MIKE_JAVSASCRIPT_BAR_INFO_H_
#define _MIKE_JAVSASCRIPT_BAR_INFO_H_

#include "javascript/glue/ObjectWrap.h"

namespace mike
{
  class Browser;
  
  namespace glue
  {
    class BarInfo : public ObjectWrap
    {
    public:
      static Handle<FunctionTemplate> NewTemplate();
      static Handle<Object> New();

      // Properties
      JS_GETTER_D(Visible);
    };
  }
}

#endif /* _MIKE_JAVSASCRIPT_BAR_INFO_H_ */
