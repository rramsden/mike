#ifndef _MIKE_JAVSASCRIPT_WINDOW_WRAP_H_
#define _MIKE_JAVSASCRIPT_WINDOW_WRAP_H_

#include "javascript/glue/ObjectWrap.h"

namespace mike
{
  class Window;
  class Browser;
  
  namespace glue
  {
    class WindowWrap : public ObjectWrap
    {
    public:
      static Handle<FunctionTemplate> NewTemplate();
      static void Fulfill(Handle<Object> window);
      
      // Properties

      JS_GETTER_D(Window);
      JS_GETTER_D(Length);
      JS_GETTER_D(Parent);
      JS_GETTER_D(Top);
      JS_GETTER_D(Opener);
      JS_GETTER_D(Closed);
      JS_GETTER_D(InnerWidth);
      JS_GETTER_D(InnerHeight);
      JS_GETTER_D(OuterWidth);
      JS_GETTER_D(OuterHeight);
      JS_ACCESSOR_D(Status);
      JS_ACCESSOR_D(Name);
      JS_ACCESSOR_D(EventCallback);

      // Functions
      
      JS_FUNCTION_D(Close);
      JS_FUNCTION_D(Print);
      JS_FUNCTION_D(Find);
      JS_FUNCTION_D(AToB);
      JS_FUNCTION_D(BToA);
      JS_FUNCTION_D(Blur);
      JS_FUNCTION_D(MoveTo);
      JS_FUNCTION_D(MoveBy);
      JS_FUNCTION_D(Stop);
      JS_FUNCTION_D(Alert);
      JS_FUNCTION_D(Confirm);
      JS_FUNCTION_D(Prompt);
    };
  }
}

#endif /* _MIKE_JAVSASCRIPT_WINDOW_WRAP_H_ */
