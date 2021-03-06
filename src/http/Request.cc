#include <algorithm>
#include "http/Request.h"

namespace mike {
  namespace http
  {
    /////////////////////////////// HELPERS //////////////////////////////////////
    
    size_t curlWriteHandler(void *ptr, size_t size, size_t nmemb, void *wdata)
    {
      if (wdata == NULL)
	return 0;

      string buf = string((char*)ptr, size * nmemb);
      stringstream* response = (stringstream*)wdata;
      response->write(buf.c_str(), (streamsize)buf.size());

      return size * nmemb;
    }

    size_t curlHeaderHandler(void *ptr, size_t size, size_t nmemb, void *wdata)
    {
      if (wdata == NULL)
	return 0;

      string line = string((char*)ptr);
      Headers* headers = (Headers*)wdata;
      headers->parseAndAppend(line);
	
      return size * nmemb;
    }
    
    /////////////////////////////// PUBLIC ///////////////////////////////////////

    //============================= LIFECYCLE ====================================

    Request* Request::Get(string url)
    {
      return new Request(url, "GET");
    }

    Request* Request::Post(string url)
    {
      return new Request(url, "POST");
    }
    
    Request::Request(string url, string method/*="GET"*/)
      : url_(url)
      , method_(method)
    {
      response_ = NULL;
      curlHeaders_ = NULL;
      cookieEnabled_ = false;
    }

    Request::~Request()
    {
      curl_slist_free_all(curlHeaders_);
      cleanupResponse();
    }
    
    //============================= ACCESS     ===================================

    Response* Request::getResponse()
    {
      return response_;
    }
    
    string Request::getUrl() const
    {
      return url_;
    }
    
    string Request::getMethod() const
    {
      return method_;
    }

    void Request::setHeader(string header)
    {
      curlHeaders_ = curl_slist_append(curlHeaders_, header.c_str());
    }

    void Request::setReferer(string url)
    {
      setHeader("Referer: " + url);
    }
  
    void Request::setData(string data)
    {
      curlPostData_ = data;
    }

    //============================= OPERATIONS ===================================

    Response* Request::perform()
    {
      cleanupResponse();
      
      if (prepareCurl()) {
	// Assign response headers container
	Headers response_headers;
	curl_easy_setopt(curl_, CURLOPT_WRITEHEADER, &response_headers);

	// Assign response body buffer
	stringstream* buffer = new stringstream();
	curl_easy_setopt(curl_, CURLOPT_WRITEDATA, (void*)buffer);

	if (curl_easy_perform(curl_) == CURLE_OK) {
	  // Extract response code
	  long response_code;
	  curl_easy_getinfo(curl_, CURLINFO_RESPONSE_CODE, &response_code);

	  // Extract content type
	  char* content_type;
	  curl_easy_getinfo(curl_, CURLINFO_CONTENT_TYPE, &content_type);

	  char* effective_url;
	  curl_easy_getinfo(curl_, CURLINFO_EFFECTIVE_URL, &effective_url);

	  // Generate response
	  response_ = new Response(effective_url, response_code, buffer,
				   response_headers.toMap(), content_type);
	}
      }

      cleanupCurl();

      if (response_ == NULL) {
	throw ConnectionError(url_);
      }

      return response_;
    }

    void Request::enableCookieSession(string token)
    {
      cookieEnabled_ = !token.empty();
      sessionToken_  = token;
    }

    void Request::enableCookieSession(bool enable, string token)
    {
      if (enable) {
	enableCookieSession(token);
      }
    }

    /////////////////////////////// PROTECTED  ///////////////////////////////////
    
    string Request::cookieFileName() const
    {
      // TODO: make base path configurable!
      return "/tmp/mike.cookie." + sessionToken_;
    }

    void Request::cleanupResponse()
    {
      delete response_;
      response_ = NULL;
    }

    bool Request::prepareCurl()
    {
      curl_ = curl_easy_init();
      
      if (curl_ != NULL) {
	curl_easy_setopt(curl_, CURLOPT_URL, url_.c_str());
	curl_easy_setopt(curl_, CURLOPT_ERRORBUFFER, curlErrorBuffer_);
	curl_easy_setopt(curl_, CURLOPT_NOPROGRESS, 1L);

	// Auto redirecting and referer discovery
	curl_easy_setopt(curl_, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(curl_, CURLOPT_AUTOREFERER, 1L);

	// Skip SSL verifications
	curl_easy_setopt(curl_, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(curl_, CURLOPT_SSL_VERIFYHOST, 0L);

	// Content and headers callbacks
	curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, curlWriteHandler);
	curl_easy_setopt(curl_, CURLOPT_HEADERFUNCTION, curlHeaderHandler);

	// Enable cookies
	if (cookieEnabled_ && !sessionToken_.empty()) {
	  curl_easy_setopt(curl_, CURLOPT_COOKIEJAR, cookieFileName().c_str());
	  curl_easy_setopt(curl_, CURLOPT_COOKIEFILE, cookieFileName().c_str());
	}
	
	// Set custom post data (if applicable)
	if (method_ == "POST") {
	  curl_easy_setopt(curl_, CURLOPT_POST, 1L);
	  curl_easy_setopt(curl_, CURLOPT_POSTFIELDS, curlPostData_.c_str());
	}

	// Set custom headers
	curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, curlHeaders_);

	return true;
      }

      return false;
    }

    void Request::cleanupCurl()
    {
      curl_easy_cleanup(curl_);
    }
  }
}
