#include <string.h>
#include <uuid/uuid.h>

#include "http/Request.h"
#include "utils/Helpers.h"
#include "Page.h"
#include "Browser.h"
#include "Config.h"

namespace mike
{
  using namespace http;
  
  /////////////////////////////// PUBLIC ///////////////////////////////////////

  //============================= LIFECYCLE ====================================

  Browser::Browser(string language/*="en"*/, string user_agent/*=""*/,
		   bool cookie_enabled/*=true*/, bool java_enabled/*=true*/)
    : language_(language)
    , customUserAgent_(user_agent)
    , cookieEnabled_(cookie_enabled)
    , javaEnabled_(java_enabled)
    , onLine_(true)
  {
    generateSessionToken();
  }

  Browser::~Browser()
  {
    expectedPopups_.clear();
    delete_all< list<Window*> >(&openedWindows_);
    delete_all< list<Window*> >(&closedWindows_);
  }

  //============================= ACCESS     ===================================

  string Browser::getLanguage() const
  {
    return language_;
  }

  string Browser::getUserAgent() const
  {
    if (customUserAgent_.empty()) {
      string tpl = MIKE_USER_AGENT;
      char agent[255];
      sprintf(agent, tpl.c_str(), OS_TYPE, CPU_ARCH, MIKE_VERSION);
      return agent;
    } else {
      return customUserAgent_;
    }
  }

  string Browser::getSessionToken() const
  {
    return sessionToken_;
  }
    
  bool Browser::isJavaEnabled() const
  {
    return javaEnabled_;
  }

  bool Browser::isJavaScriptEnabled() const
  {
    return isJavaEnabled();
  }

  bool Browser::isCookieEnabled() const
  {
    return cookieEnabled_;
  }

  void Browser::enableCookies()
  {
    cookieEnabled_ = true;
  }

  void Browser::disableCookies()
  {
    cookieEnabled_ = false;
  }

  void Browser::enableJava()
  {
    javaEnabled_ = true;
  }

  void Browser::enableJavaScript()
  {
    enableJava();
  }

  void Browser::disableJava()
  {
    javaEnabled_ = false;
  }

  void Browser::disableJavaScript()
  {
    disableJava();
  }

  void Browser::workOffLine()
  {
    onLine_ = false;
  }

  void Browser::workOnLine()
  {
    onLine_ = true;
  }

  bool Browser::isOnLine()
  {
    return onLine_;
  }
  
  // XXX: in the future it should return read only list...
  list<Window*>& Browser::getWindows()
  {
    return openedWindows_;
  }

  list<Window*>& Browser::getClosedWindows()
  {
    return closedWindows_;
  }
  
  Window* Browser::getWindow(int n)
  {
    if (n < openedWindows_.size()) {
      list<Window*>::iterator it = openedWindows_.begin();
      advance(it, n);
      return *it;
    }

    throw WindowNotExistsError();
  }

  //============================= EXPECTATIONS =================================

  void Browser::expectAlert()
  {
    PopupExpectation e;
    e.kind = kPopupAlert;
    e.flags = kSkipMessage;
    expectedPopups_.push_back(e);
  }

  void Browser::expectAlert(string msg)
  {
    PopupExpectation e;
    e.kind = kPopupAlert;
    e.flags = kMatchMessage;
    e.message = msg;
    expectedPopups_.push_back(e);
  }

  void Browser::expectAlerts(int n)
  {
    while (n-- > 0)
      expectAlert();
  }

  void Browser::expectConfirm(bool choice)
  {
    PopupExpectation e;
    e.kind = kPopupConfirm;
    e.flags = kSkipMessage;
    e.choice = choice ? "true" : "false";
    expectedPopups_.push_back(e);
  }

  void Browser::expectConfirm(string msg, bool choice)
  {
    PopupExpectation e;
    e.kind = kPopupConfirm;
    e.flags = kMatchMessage;
    e.message = msg;
    e.choice = choice ? "true" : "false";
    expectedPopups_.push_back(e);
  }

  void Browser::expectConfirms(int n, bool choice)
  {
    while (n-- > 0)
      expectConfirm(choice);
  }

  void Browser::expectPrompt(string msg, string choice)
  {
    PopupExpectation e;
    e.kind = kPopupPrompt;
    e.flags = kMatchMessage;
    e.message = msg;
    e.choice = choice;
    expectedPopups_.push_back(e);
  }

  void Browser::expectPrompt(string msg, int flags)
  {
    PopupExpectation e;
    e.kind = kPopupPrompt;
    e.flags = kMatchMessage | flags;
    e.message = msg;
    expectedPopups_.push_back(e);
  }

  void Browser::expectPrompt(string choice)
  {
    PopupExpectation e;
    e.kind = kPopupPrompt;
    e.flags = kSkipMessage;
    e.choice = choice;
    expectedPopups_.push_back(e);
  }

  void Browser::expectPrompt(int flags)
  {
    PopupExpectation e;
    e.kind = kPopupPrompt;
    e.flags = kSkipMessage | flags;
    expectedPopups_.push_back(e);
  }

  void Browser::clearExpectations()
  {
    expectedPopups_.clear();
  }
  
  //============================= OPERATIONS ===================================

  Page* Browser::open(string url)
  {
    Window* window = newWindow();
    Page* page = Page::Open(url, cookieEnabled_, sessionToken_);
    window->setPage(page);
    return page;
  }

  Page* Browser::getPage(string url)
  {
    open(url);
  }

  Window* Browser::newWindow()
  {
    return new Window(this);
  }
  
  void Browser::closeAll()
  {
    openedWindows_.clear();
  }

  void Browser::closeAllWindows()
  {
    closeAll();
  }

  void Browser::closeWindow(int n)
  {
    Window* window = getWindow(n);
    closeWindow(window);
  }

  void Browser::closeWindow(Window* window)
  {
    openedWindows_.remove(window);
    closedWindows_.push_back(window);
  }

  /////////////////////////////// PROTECTED  ///////////////////////////////////

  void Browser::generateSessionToken()
  {
    uuid_t token;
    char parsed[UUID_TOKEN_SIZE];
    uuid_generate(token);
    uuid_unparse(token, parsed);
    sessionToken_ = string(parsed, UUID_TOKEN_SIZE);
  }

  list<PopupExpectation>& Browser::getExpectations()
  {
    return expectedPopups_;
  }
}
