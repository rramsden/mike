#ifndef _MIKE_PAGE_H_
#define _MIKE_PAGE_H_

#include <string>
#include <sstream>

#include "http/Request.h"

namespace mike
{
  using namespace std;
  using namespace http;
  
  class Frame;
  class Window;

  class XmlPage;
  class HtmlPage;
  
  /**
   * Available page types.
   */
  enum PageType {
    UNKNOWN_PAGE,
    XML_PAGE,
    HTML_PAGE,
    TEXT_PAGE,
    BINARY_PAGE
  };
  
  /**
   * This is base class for any kind of pages opened in browser. You should never create
   * instances directly, only use the 'Open' method for simple get page, and Factory' method
   * for more complex requests.
   */
  class Page
  {
    friend class Window;
    friend class HtmlPage;
    friend class XmlPage;
    
  public:
    /**
     * Performs GET request on specified URL and returns page object of appropriate type.
     *
     * \code
     *   Page* page = Page::Open("http://www.mikebrowser.com");
     *   assert(page->isHtml());
     *   //...
     *   delete page;
     * \endcode
     *
     * \param url URL to open.
     * \return Page object.
     */
    static Page* Open(string url);
    
    /**
     * Performs given request, and depending on response content type creates new instance of
     * appropriate type. 
     *
     * \code
     *   Request* request = Request::Post("http://www.mypage.com/form");
     *   request->setPostData("foo=bar&spam=eggs");
     *   Page* page = Page::Factory(request);
     *   //...
     *   delete page;
     * \endcode
     *
     * Note: you don't need to delete req object, this will be done automatically on page destroy.
     *
     * \param request Request to perform.
     * \return Page object.
     */
    static Page* Factory(Request* request);

    /**
     * Creates new page instance. Don't call it directly, use 'Factory' or 'Open' instead. 
     *
     * \param request Performed request for this page.
     * \param type Kind of page.
     */
    explicit Page(Request* request, PageType type=UNKNOWN_PAGE);
    
    /**
     * Destructor.
     */
    virtual ~Page();

    /**
     * \return HTTP request built for the page.
     */
    Request* getRequest();

    /**
     * \return HTTP response for the page.
     */
    Response* getResponse();

    /**
     * \requrn URL address of the page.
     */
    string getUrl();

    /**
     * \return Stream with content of the page.
     */
    stringstream* getStream();

    /**
     * \return Content of the page as string.
     */
    string getContent();
   
    /**
     * \return Whether page is an XML document or not.
     */
    bool isXml();

    /**
     * \return Whether page is an HTML document or not.
     */
    bool isHtml();

    /**
     * Reloads page using request passed to constructor.
     */
    virtual void reload();

    /**
     * \return Frame in which current page has been rendered.
     */
    Frame* getEnclosingFrame();

    /**
     * \return Window which contains frame with this page.
     */
    Window* getEnclosingWindow();
    
  protected:
    http::Request* request_;
    PageType type_;
    Frame* frame_;

    /**
     * Opens the page in given frame. For the security reasons each instance can
     * be enclosed only once.
     *
     * \param frame Frame in which this page should be opened.
     */
    virtual void enclose(Frame* frame);
  };
}

#endif /* _MIKE_PAGE_H_ */
