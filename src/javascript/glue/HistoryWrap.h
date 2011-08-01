#ifndef _MIKE_JAVSASCRIPT_HISTORY_WRAP_H_
#define _MIKE_JAVSASCRIPT_HISTORY_WRAP_H_

#include "javascript/glue/ObjectWrap.h"

namespace mike
{
  class Browser;
  
  namespace glue
  {
    class HistoryWrap : public ObjectWrap
    {
    public:
      static Handle<FunctionTemplate> NewTemplate();
      static Handle<Object> New();

      // Properties
      JS_GETTER_D(Length);
      
      // Functions
      JS_FUNCTION_D(Back);
      JS_FUNCTION_D(Forward);
      JS_FUNCTION_D(Go);
    };
  }
}

#endif /* _MIKE_JAVSASCRIPT_HISTORY_WRAP_H_ */
