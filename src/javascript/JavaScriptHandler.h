#ifndef _MIKE_JAVASCRIPT_HANDLER_H_
#define _MIKE_JAVASCRIPT_HANDLER_H_

#include <assert.h>
#include <string>
#include <map>
#include <v8.h>

namespace mike
{
  using namespace std;
  using namespace v8;
  
  class HtmlPage;
  class HtmlElement;

  namespace glue
  {
    class WindowWrap;
  }

  class JavaScriptHandler
  {
    friend class HtmlPage;
    friend class glue::WindowWrap;
    
  public:
    template <typename T> static inline void WrapPtr(Handle<Object> handle,T* ptr)
    {
      assert(!handle.IsEmpty());
      assert(handle->InternalFieldCount() > 0);
      handle->SetInternalField(0, External::New((void*)ptr));
    }
  
    JavaScriptHandler(HtmlPage* page);
    ~JavaScriptHandler();

    string evaluate(string source, string fname, unsigned int line);
    Handle<Context> getContext();

    Handle<Value> getEventCallback(string name);
    void setEventCallback(string name, Handle<Value> callback);
    
  protected:
    HtmlPage* page_;
    map< string, Handle<Value> > eventCallbacks_;
    Persistent<Context> context_;
    Persistent<Object> window_;
  };
}

#endif /* _MIKE_JAVASCRIPT_HANDLER_H_ */
