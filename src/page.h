#ifndef _MIKE_PAGE_H_
#define _MIKE_PAGE_H_

#include <string>
#include "http/request.h"

namespace mike
{
  using namespace std;
  using namespace http;

  class XmlPage;
  class HtmlPage;
  
  /**
   * Available page types. 
   *
   */
  enum PageType {
    UNKNOWN_PAGE,
    TEXT_PAGE,
    XML_PAGE,
    HTML_PAGE,
    BINARY_PAGE
  };
  
  /**
   * Object of this class represents single page loaded by corresponding request.
   * This is base class for all complex kinds of pages, like HTML, XML, Text page, etc.
   *
   */
  class Page
  {
  public:
    /**
     * Performs given request, and depending on response content type build proper
     * page object for it. Example: 
     *
     * <code>
     *   http::Request* req = http::Request::Get("http://www.mikebrowser.com/");
     *   Page* page = Page::Build(req);
     *   // ...
     *   delete page;
     * </code>
     *
     * Note: you don't need to delete req object, it will be done automatically on page destroy.
     *
     */
    static Page* Build(Request* request);

    /**
     * Constructor.
     *
     */
    explicit Page(Request* request, PageType type);
    
    /**
     * Destructor.
     *
     */
    virtual ~Page();

    /**
     * Returns HTTP request built for this page.
     *
     */
    Request* getRequest();

    /**
     * Returns HTTP response built for this page.
     *
     */
    Response* getResponse();

    /**
     * Requrns request url of this website.
     *
     */
    string getUrl();
    
    /**
     * Returns <code>true</code> when current page is an text page.
     *
     */
    bool isText();

    /**
     * Returns <code>true</code> when current page is an XML or HTML page.
     *
     */
    bool isXml();

    /**
     * Returns <code>true</code> when current page is an HTML page.
     *
     */
    bool isHtml();

    /**
     * Returns <code>true</code> when current page is an binary attachment.
     *
     */
    bool isBinary();

    /**
     * If current page is an XML (or HTML) document then returns <code>XmlPage</code>
     * object, otherwise <code>NULL</code> will be returned.
     *
     */
    XmlPage* toXmlPage();

    /**
     * If current page is an HTML document then returns <code>HtmlPage</code>
     * object, otherwise <code>NULL</code> will be returned.
     *
     */
    HtmlPage* toHtmlPage();
    
    /**
     * Performs request to this page once again and reloads everything.
     *
     */
    void reload();
    
  protected:
    http::Request* request_;
    PageType type_;
  };
}

#endif /* _MIKE_PAGE_H_ */