#include <libxml/uri.h>
#include "Page.h"
#include "Frame.h"
#include "Window.h"
#include "xml/XmlPage.h"
#include "html/HtmlPage.h"

namespace mike
{
  /////////////////////////////// PUBLIC ///////////////////////////////////////

  //============================= LIFECYCLE ====================================

  Page* Page::Open(string url, bool cookie_enabled/*=false*/, string session_token/*=""*/,
		   bool force_base/*=false*/)
  {
    Request* request = Request::Get(url);
    request->enableCookieSession(cookie_enabled, session_token);
    return Factory(request, force_base);
  }
  
  Page* Page::Factory(Request* request, bool force_base/*=false*/)
  {
    if (request) {
      Response* response = NULL;

      try {
	response = request->perform();
      } catch (ConnectionError err) {
	delete request;
	throw err;
      }
      
      if (response) {
	if (!force_base) {
	  if (response->isHtml())
	    return new HtmlPage(request);
	  else if (response->isXml())
	    return new XmlPage(request);
	}
	
	return new Page(request);
      }
    }

    throw "Invalid request."; // just in case, should never appear...
  }

  static int num = 0;
  
  Page::Page(Request* request, PageType type/*=kUnknownPage*/)
    : type_(type)
  {
    request_ = request;
    frame_ = NULL;
    printf("CRE: %d\n", ++num);
  }

  Page::~Page()
  {
    printf("DEL: %d\n", --num);
    delete request_;
  }

  //============================= ACCESS     ===================================
  
  Request* Page::getRequest()
  {
    return request_;
  }

  Response* Page::getResponse()
  {
    return request_->getResponse();
  }

  string Page::getUrl()
  {
    return getResponse()->getUrl();
  }

  stringstream* Page::getStream()
  {
    return getResponse()->getStream();
  }

  string Page::getContent()
  {
    return getResponse()->getBody();
  }

  bool Page::isXml()
  {
    return (type_ == kXmlPage || type_ == kHtmlPage);
  }

  bool Page::isHtml()
  {
    return type_ == kHtmlPage;
  }

  Frame* Page::getEnclosingFrame()
  {
    return frame_;
  }

  Window* Page::getEnclosingWindow()
  {
    return frame_ ? static_cast<Window*>(frame_->getTop()) : NULL;
  }

  string Page::getUrlFor(string url)
  {
    xmlChar* xuri = xmlCharStrdup(url.c_str());
    xmlChar* xbase = xmlCharStrdup(getUrl().c_str());
    xmlChar* xresult = xmlBuildURI(xuri, xbase);

    string result = (char*)xresult;

    xmlFree(xuri);
    xmlFree(xbase);
    xmlFree(xresult);

    return result;
  }

  XmlPage* Page::asXml()
  {
    if (isXml())
      return static_cast<XmlPage*>(this);
    else
      throw PageConversionError("This is not an XML page");
  }

  HtmlPage* Page::asHtml()
  {
    if (isHtml())
      return static_cast<HtmlPage*>(this);
    else
      throw PageConversionError("This is not an HTML page");
  }
  
  
  //============================= OPERATIONS ===================================
  
  void Page::reload()
  {
    request_->perform();
  }

  void Page::enclose(Frame* frame)
  {
    if (frame_ == NULL)
      frame_ = frame;
  }
}
