#ifndef _MIKE_BROWSER_H_
#define _MIKE_BROWSER_H_

#include <string>
#include <list>
#include <exception>

#include "javascript/glue/WindowWrap.h"

namespace mike
{
  using namespace std;

  class Page;
  class HtmlPage;
  class Window;
  class Frame;

  enum PopupType {
    kPopupAlert = 1,
    kPopupConfirm = 2,
    kPopupPrompt = 4
  };

  enum PopupExpectationFlag {
    kMatchMessage = 1,
    kSkipMessage = 2,
    kCancelPrompt = 4
  };
  
  typedef struct popup_expectation {
    int flags;
    PopupType kind;
    string message;
    string choice;
  } PopupExpectation;
  
  /**
   * Error raised when trying to get window which is not open in particular browser instance.
   */
  class WindowNotExistsError : public exception
  {
  public:
    explicit WindowNotExistsError() {};
    virtual ~WindowNotExistsError() throw() {};
    virtual const char* what() const throw() { return "Window index out of range"; }
  };

  /**
   * Base class for popup expectations. Don't use directly!
   */
  class ExpectationError : public exception
  {
  public:
    explicit ExpectationError(PopupType type, string msg) : type_(type), msg_(msg) {}
    virtual ~ExpectationError() throw() {};
    virtual const char* what() const throw() =0;
    const string type() const
    {
      switch (type_) {
      case kPopupAlert:   return "alert";
      case kPopupConfirm: return "confirm";
      case kPopupPrompt:  return "prompt";
      default:
	return "unknown popup";
      }
    }
  protected:
    PopupType type_;
    string msg_;
  };

  /**
   * Error raised when some unexpected popup appears.
   */
  class UnexpectedPopupError : public ExpectationError
  {
  public:
    explicit UnexpectedPopupError(PopupType type, string msg) : ExpectationError(type, msg) {}
    virtual ~UnexpectedPopupError() throw() {}
    virtual const char* what() const throw() { return ("Unexpected " + type() + ": " + string(msg_)).c_str(); }
  };

  /**
   * Error raised when some defined expectation has not been met during page processing.
   */
  class ExpectationNotMetError : public ExpectationError
  {
  public:
    explicit ExpectationNotMetError(PopupType type, string msg) : ExpectationError(type, msg) {}
    virtual ~ExpectationNotMetError() throw() {};
    virtual const char* what() const throw() { return ("Expectated " + type() + " not met: " + string(msg_)).c_str(); }
  };

  /**
   * Instance of this class represents single, separatelly configured browser. You can
   * configure browser by passing various parameters to constructor (configurable language,
   * user agent string and cookies/javascript availability).
   *
   * \code
   *   Browser browser = Browser("en", "", true, true); // language, custom user agent, cookies, javascript
   *   PageRef<HtmlPage> page = browser.open("http://www.mypage.com").asHtml();
   *   assert(page->getTitle() == "Hello My Page!");
   *   browser.closeAll();
   * \endcode
   */
  class Browser
  {
    friend class glue::WindowWrap;
    friend class HtmlPage;
    friend class Frame;
    friend class Window;
    
  public:
    /**
     * Creates configured browser instance.
     *
     * \param language The language used by this browser instance.
     * \param user_agent Custom user agent string.
     * \param enable_cookie Should this instance allow to store cookies?
     * \param enable_javascript Should this instance allow to execute javascript?
     */
    explicit Browser(string language="en", string user_agent="",
		     bool enable_cookie=true, bool enable_java=true);

    /**
     * Destructor.
     */
    ~Browser();

    /**
     * \return Language used by browser.
     */
    string getLanguage() const;

    /**
     * \return User agent string assigned to this browser.
     */
    string getUserAgent() const;

    /**
     * \return Unique token of this session.
     */
    string getSessionToken() const;

    /**
     * Creates new empty window and returns it.
     *
     * \return New window.
     */
    Window* newWindow();
    
    /**
     * Returns list of all currently opened windows.
     *
     * \code
     *   browser->open("http://www.mikebrowser.com/");
     *   browser->open("http://www.cuboxsa.com");
     *   list<Window*> windows = browser->getWindows();
     *   assert(windows.size() == 2);
     * \endcode
     */
    list<Window*>& getWindows();

    /**
     * Returns specified window if opened, otherwise throws 'WindowNotFound' error.
     *
     * \code
     *   browser->open("http://www.cuboxsa.com/");
     *   Window* window = browser->getWindow(0);
     *   assert(window->getUrl(), "http://www.cuboxsa.com");
     * \endcode
     *
     * \param n Index of window to get.
     */
    Window* getWindow(int n);
    
    /**
     * \return True when javascript is enabled. 
     */
    bool isJavaEnabled() const;
    bool isJavaScriptEnabled() const;

    /**
     * \return True when cookies are enabled.
     */
    bool isCookieEnabled() const;

    /**
     * Enables cookies handling.
     */
    void enableCookies();

    /**
     * Disables cookies handling.
     */
    void disableCookies();

    /**
     * Enables javascript processing.
     */
    void enableJava();
    void enableJavaScript();

    /**
     * Disables javascript processing.
     */
    void disableJava();
    void disableJavaScript();

    /**
     * Enables offline mode.
     */
    void workOffLine();

    /**
     * Enables online mode.
     */
    void workOnLine();

    /**
     * \return Whether browser works in on line or off line mode.
     */
    bool isOnLine();
    
    /**
     * Closes all windows opened within current instance (called automatically when browser object
     * is deleted).
     *
     * \code
     *   Browser* browser = new Browser();
     *   browser->open("http://www.cuboxsa.com");
     *   browser->open("http://www.mikebrowser.com");
     *   //...
     *   browser->closeAll();
     *   browser->open("http://www.cuboxsa.com");
     *   //...
     *   delete browser; // closeAll is called here automatically...
     * \endcode
     */
    void closeAll();
    void closeAllWindows();

    /**
     * Closes window beeing under specified index on the list. If specified window doesn't
     * exist then nothing happen. Second version closes and removes specified window from the
     * list of opened.
     *
     * \code
     *   browser->open("http://www.mikebrowser.com/");
     *   browser->open("http://www.cuboxsa.com/");
     *   browser->closeWindow(1);
     *   browser->closeWindow(browser->getWindow(0));
     * \endcode
     *
     * \param n Index of window to close.
     * \param window Window instance to close.
     */
    void closeWindow(int n);
    void closeWindow(Window* window);
    
    /**
     * Opens specified url in new virtual window, and returns loaded page. You can also use
     * the getPage() alias.
     * 
     * \code
     *   HtmlPage* page = (HtmlPage*)browser->open("http://www.google.com/");
     *   assert(page->getTitle() == "Google Search");
     *   assert(page->getContent() == "...");
     *   // ...
     * \endcode
     *
     * \param url Address of page to open. 
     */
    Page* open(string url);
    Page* getPage(string url);

    /**
     * Sets expectation of one alert box with any message.
     */
    void expectAlert();

    /**
     * Sets expectation of one alert box with given message.
     *
     * \param msg Expected message.
     */
    void expectAlert(string msg);

    /**
     * Sets expectation of given number of alerts with any message.
     *
     * \param n Number of expected alerts.
     */
    void expectAlerts(int n);

    /**
     * Sets expectation of one confirmation box with any message, and sets prefered choice
     * for it.
     *
     * \param choice Prefered choice (true = Yes, false = No).
     */
    void expectConfirm(bool choice);
    
    /**
     * Sets expectation of one confirmation box with given message, and sets prefered choice
     * for it.
     *
     * \param msg Expected message.
     * \param choice Prefered choice.
     */
    void expectConfirm(string text, bool choice);

    /**
     * Sets expectation of given number of confirmationswith given message, and sets prefered
     * choice for all of them.
     *
     * \param n Number of expected windows.
     * \param choice Prefered choice.
     */
    void expectConfirms(int n, bool choice);

    /**
     * Sets expectation of one prompt box with given message, and sets prefered choice
     * for it. If no choice given then prompt will be cancelled.
     *
     * \param msg Expected message.
     * \param choice Given text.
     * \param flags Should be always 'kCancelPrompt'.
     */
    void expectPrompt(string msg, string choice);
    void expectPrompt(string msg, int flags);
      
    /**
     * Sets expectation of one prompt box with any message, and sets prefered choice
     * for it. If no choice given then prompt will be cancelled.
     *
     * \param choice Given text.
     * \param flags Should be always 'kCancelPrompt'.
     */
    void expectPrompt(string choice);
    void expectPrompt(int flags);
    
    /**
     * Removes all declared expectations.
     */
    void clearExpectations();
    
  protected:
    bool javaEnabled_;
    bool cookieEnabled_;
    string language_;
    string customUserAgent_;
    string sessionToken_;
    bool onLine_;
    list<Window*> windows_;
    list<PopupExpectation> expectedPopups_;

    /**
     * Generates UUID token for current browser instance.
     */
    void generateSessionToken();
  };
}

#endif /* _MIKE_BROWSER_H_ */
