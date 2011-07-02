#ifndef _MIKE_HTML_PAGE_H_
#define _MIKE_HTML_PAGE_H_

#include <libxml/HTMLparser.h>
#include <string>
#include <vector>

#include "xml/XmlPage.h"
#include "html/HtmlElement.h"
#include "html/HtmlFrame.h"
#include "utils/Pector.h"

namespace mike
{
  typedef Pector<HtmlElement> HtmlElementSet;

  class Window;
  class Browser;
  
  /**
   * HTML page representation.
   */
  class HtmlPage : public XmlPage
  {
  public:
    explicit HtmlPage(Request* request);
    virtual ~HtmlPage();

    // override
    HtmlElementSet* getElementsByXpath(string xpath);
    HtmlElement* getElementByXpath(string xpath);
    HtmlElementSet* getElementsByTagName(string tag);
    virtual void enclose(Frame* frame);
    virtual void reload();

    /**
     * Returns set of elements matching given CSS rule.
     *
     * \code
     *   HtmlElementSet* items = page->getElementsByCss("ul#items li");
     * \endcode
     *
     * \param rule CSS rule.
     * \return Matching elements.
     */
    HtmlElementSet* getElementsByCss(string rule);

    /**
     * Returns all elements with given class name.
     *
     * \code
     *   HtmlElementSet* tags = page->getElementsByClassName("tag");
     * \endcode
     *
     * \param klass Class name to find.
     * \return Elements with specified class name.
     */
    HtmlElementSet* getElementsByClassName(string klass);

    /**
     * Returns first found element with given ID. If no elements found then ElementNotFoundError
     * will be thrown.
     *
     * \code
     *   HtmlElement* header = page->getElementById("header");
     * \endcode
     *
     * \param id ID to find.
     * \return Element with specified id. 
     */
    HtmlElement* getElementById(string id);
    
    /**
     * Locates link or button on a page. It can be find by id, name or text.
     *
     * \code
     *   HtmlElement* link = page->getLinkOrButton("Click me!");
     *   HtmlElement* button = page->getLinkOrButton("submit_button");
     *   HtmlElement* named_link = page->getLinkOrButton("foo-link");
     * \endcode
     *
     * \param locator Name, ID or text of the link/button.
     * \return Element matching given locator.
     */
    // TODO: change result to HtmlClicableElement or smth...
    HtmlElement* getLinkOrButton(string locator);

    /**
     * Locates link on the page. It can be find by id, name or text.
     *
     * \code
     *   HtmlElement* link = page->getLink("Click me!");
     *   HtmlElement* named_link = page->getLink("foo-link");
     * \endcode
     *
     * \param locator Name, ID or text of the link.
     * \return Link matching given locator.
     */
    // TODO: change result to HtmlAnchorElement
    HtmlElement* getLink(string locator);

    /**
     * Locates button on the page. It can be find by id, name or text.
     *
     * \code
     *   HtmlElement* button = page->getButton("Click me!");
     *   HtmlElement* submit_button = page->getButton("submit");
     * \endcode
     *
     * \param locator Name, ID or text of the button.
     * \return Button matching given locator.
     */
    // TODO: change result to HtmlButtonElement
    HtmlElement* getButton(string locator);

    /**
     * Locates form field on the page. It can be find by id, name or label text.
     *
     * \code
     *   HtmlElement* user_name_field = page->getField("user[name]");
     *   HtmlElement* password_field = page->getField("Password");
     * \endcode
     *
     * \param locator Name, ID or label of the field.
     * \return Field matching given locator.
     */
    // TODO: change result to HtmlFormFieldElement or smth...
    HtmlElement* getField(string locator);

    /**
     * \return Page title text.
     */
    string getTitle();
    string getTitleText();

    /**
     * \return All frames from within this page.
     */
    vector<HtmlFrame*> getFrames();

    /**
     * If number given then returns frame from given index on the list, if string
     * then tries to find named frame.
     *
     * \param n Frame index.
     * \param name Frame name.
     * \return Specified frame.
     */
    HtmlFrame* getFrame(int n);
    HtmlFrame* getFrame(string name);
    HtmlFrame* getNamedFrame(string name);
    
  protected:
    htmlDocPtr doc_;
    vector<HtmlFrame*> frames_;
    
    // override
    virtual void parseDocument();

    /**
     * Loads all iframes and frames, and opens them in virtual frames.
     */
    virtual void loadFrames();

    /**
     * Removes all frames.
     */
    virtual void clearFrames();
  };
}


#endif /* _MIKE_HTML_PAGE_H_ */
