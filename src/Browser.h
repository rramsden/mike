#ifndef _MIKE_BROWSER_H_
#define _MIKE_BROWSER_H_

#include <string>

namespace mike
{
  using namespace std;

  class BrowserWindow;

  /**
   * Object of this class represents single, separatelly configured instance of
   * Mike's browser. Example:
   *
   * <code>
   *   Browser* browser = new Browser("en", "", true, true); // language, custom user agent, cookies, javascript
   *   BrowserWindow* window = browser->Open("http://www.mypage.com");
   *   // ...
   *   delete browser;
   * </code>
   *
   */
  class Browser
  {
  public:
    /**
     * Constructor.
     *
     */
    explicit Browser(string language="en", string userAgent="", bool enableCookie=true, bool enableJava=true);

    /**
     * Destructor.
     *
     */
    ~Browser();

    /**
     * Returns browser language.
     *
     */
    string getLanguage();

    /**
     * Returns default user agent assigned to browser.
     *
     */
    string getUserAgent();

    /**
     * Returns <code>true</code> when javascript is enabled. 
     *
     */
    bool isJavaEnabled();

    /**
     * Returns <code>true</code> when cookies are enabled.
     *
     */
    bool isCookieEnabled();

    /**
     * Opens specified url in new virtual window, and returns this window.
     *
     * <code>
     *   BrowserWindow* google = browser->Open("http://www.google.com/");
     *   if (google->isLoaded()) {
     *     string content = google->getPage()->getContent();
     *     // ...
     *   }
     *   delete google;
     * </code>
     *
     */
    BrowserWindow* Open(string url);
    
  protected:
    bool javaEnabled_;
    bool cookieEnabled_;
    string language_;
    string customUserAgent_;
  };
}

#endif /* _MIKE_BROWSER_H_ */
