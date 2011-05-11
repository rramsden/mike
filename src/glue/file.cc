#include "glue/file.h"
#include <sys/stat.h>

namespace mike {
  namespace glue {
    namespace file
    {
      using namespace v8;
      using namespace std;

      bool exists_p(string fname)
      {
	struct stat fileinfo;

	if (access(fname.c_str(), 0) == 0) {
	  return (stat(fname.c_str(), &fileinfo) == 0);
	}

	return false;
      }

      bool check_st_mode(string fname, int mode)
      {
	struct stat fileinfo;
	bool result = false;
	  
	if (access(fname.c_str(), 0) == 0) {
	  if (stat(fname.c_str(), &fileinfo) == 0) {
	    return fileinfo.st_mode & mode;
	  }
	}

	return false;
      }

      char* read_contents(char *fname)
      {
	FILE *fp = fopen(fname, "r");
	char *buffer = NULL;
	long lsize;

	if (fp != NULL) {
	  fseek(fp, 0, SEEK_END);
	  lsize = ftell(fp);
	  rewind(fp);
	  buffer = (char*)malloc((sizeof(char)*lsize));

	  if (buffer == NULL || fread(buffer, 1, lsize, fp) != lsize) {
	    buffer = NULL;
	  }
	    
	  fclose(fp);
	}

	return buffer;
      }

      /*
       * Function is checking if given file exists. Example (js):
       * 
       *   File.exists("/path/to/file"); // => true or false
       * 
       */
      Handle<Value> exists(const Arguments &args)
      {
	if (args.Length() == 1) {
	  String::Utf8Value fname(args[0]->ToString());
	  return exists_p(*fname) ? True() : False();
	} else {
	  return Undefined();
	}
      }

      /*
       * This function is checking if given path represents directory. If
       * 
       *   File.isDirectory("/path/to/file"); // => true or false
       *
       */
      Handle<Value> isDirectory(const Arguments &args)
      {
	if (args.Length() == 1) {
	  String::Utf8Value fname(args[0]->ToString());
	  return check_st_mode(*fname, S_IFDIR) ? True() : False();
	} else {
	  return Undefined();
	}
      }

      /*
       * This function is checking if given path represents file. If
       * 
       *   File.isFile("/path/to/file"); // => true or false
       *
       */
      Handle<Value> isFile(const Arguments &args)
      {
	if (args.Length() == 1) {
	  String::Utf8Value fname(args[0]->ToString());
	  return check_st_mode(*fname, S_IFREG) ? True() : False();
	} else {
	  return Undefined();
	}
      }

      /*
       * Reads contents from specified file. If it's unable to read given file
       * then <code>null</code> is returned. 
       *
       *   File.read("/path/to/file"); // contents or null
       *
       */
      Handle<Value> read(const Arguments &args)
      {
	if (args.Length() == 1) {
	  String::Utf8Value fname(args[0]->ToString());
	  char *content = read_contents(*fname);

	  if (content != NULL) {
	    return String::New(content);
	  } else {
	    return Null();
	  }
	} else {
	  return Undefined();
	}
      }
    }

    Handle<Object> FileObject()
    {
      HandleScope scope;
      Handle<Object> fileobj(Object::New());

      fileobj->Set(String::NewSymbol("exists"), FunctionTemplate::New(file::exists)->GetFunction());
      fileobj->Set(String::NewSymbol("isDirectory"), FunctionTemplate::New(file::isDirectory)->GetFunction());
      fileobj->Set(String::NewSymbol("isFile"), FunctionTemplate::New(file::isFile)->GetFunction());
      fileobj->Set(String::NewSymbol("read"), FunctionTemplate::New(file::read)->GetFunction());

      return fileobj;
    }
  }
}
