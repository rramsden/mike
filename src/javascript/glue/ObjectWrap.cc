#include "javascript/glue/ObjectWrap.h"
#include "Frame.h"

namespace mike {
  namespace glue
  {
    Handle<Value> ObjectWrap::Unwrap(Handle<Object> handle, int field/*=0*/)
    {
      assert(!handle.IsEmpty());
      assert(handle->InternalFieldCount() > field);
      return handle->GetInternalField(field);
    }

    void ObjectWrap::Wrap(Handle<Object> handle, Handle<Value> value, int field/*=0*/)
    {
      assert(!handle.IsEmpty());
      assert(handle->InternalFieldCount() > field);
      handle->SetInternalField(field, value);
    }

    void ObjectWrap::NotSupported(string what)
    {
      //printf("JS: %s is not supported\n", what.c_str());
    }

    void ObjectWrap::StopExecution()
    {
      Handle<Object> err(JS_OBJ());
      err->Set(JS_STR("stop"), JS_INT(1));
      JS_THROW_OBJ(err);
    }
  }
}
