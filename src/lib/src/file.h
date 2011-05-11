#ifndef _MIKE_LIB_FILE_H_
#define _MIKE_LIB_FILE_H_

#include <v8.h>
#include "lib/src/lib.h"

namespace mike {
  namespace glue {
    namespace file {
      using namespace std;
      using namespace v8;
      
      Handle<Value> read(const Arguments &args);        // read(path, modeloptions) -> String or false
      Handle<Value> exists(const Arguments &args);      // exists(path) -> Boolean
      Handle<Value> isFile(const Arguments &args);      // isFile(path) -> Boolean
      Handle<Value> isDirectory(const Arguments &args); // isDirectory(path) -> Boolean
      Handle<Value> isReadable(const Arguments &args);  // isReadable(path) -> Boolean
    }
  }
}

#endif /* _MIKE_LIB_FILE_H_ */