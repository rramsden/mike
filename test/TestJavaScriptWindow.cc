#include "test/Macros.h"

#include "html/HtmlPage.h"
#include "Window.h"
#include "Browser.h"

using namespace std;
using namespace mike;

class MikeJavaScriptWindowTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(MikeJavaScriptWindowTest);
  CPPUNIT_TEST(testWindowAndThisObject);
  CPPUNIT_TEST(testAnyExpectedAlert);
  CPPUNIT_TEST(testSpecificExpectedAlert);
  CPPUNIT_TEST(testUnexpectedAlerts);
  CPPUNIT_TEST(testAnyExpectedConfirm);
  CPPUNIT_TEST(testSpecificExpectedConfirm);
  CPPUNIT_TEST(testUnexpectedConfirm);
  CPPUNIT_TEST(testAnyExpectedPrompt);
  CPPUNIT_TEST(testSpecificExpectedPrompt);
  CPPUNIT_TEST(testUnexpectedPrompt);
  CPPUNIT_TEST(testCancelPrompt);
  CPPUNIT_TEST(testNotFullyMetExpectations);
  CPPUNIT_TEST(testFramesLength);
  CPPUNIT_TEST(testFrames);
  CPPUNIT_TEST(testParentInTopFrame);
  CPPUNIT_TEST(testParentInInternalFrame);
  CPPUNIT_TEST(testTopInTopFrame);
  CPPUNIT_TEST(testTopInInternalFrame);
  CPPUNIT_TEST(testClose);
  CPPUNIT_TEST(testClosed);
  CPPUNIT_TEST(testStatus);
  CPPUNIT_TEST(testDefaultStatus);
  CPPUNIT_TEST(testEval);
  CPPUNIT_TEST(testPrint);
  CPPUNIT_TEST(testFind);
  CPPUNIT_TEST(testBarInfo);
  CPPUNIT_TEST(testOpener);
  CPPUNIT_TEST(testName);
  CPPUNIT_TEST_SUITE_END();

protected:

  Browser browser;
  
  void testWindowAndThisObject()
  {
    PageRef<HtmlPage> page = browser.open("http://localhost:4567/simple.html")->asHtml();
    ASSERT_EQUAL(page->evaluate("window == this"), "true");
    ASSERT_EQUAL(page->evaluate("window = 'cantoverwrite'; window == this;"), "true");
    ASSERT_EQUAL(page->evaluate("window == window.window"), "true");
    ASSERT_EQUAL(page->evaluate("window == frames"), "true");
    ASSERT_EQUAL(page->evaluate("window.constructor.toString()"), "function DOMWindow() { [native code] }");
  }

  void testAnyExpectedAlert()
  {
    browser.expectAlert();
    PageRef<HtmlPage> page = browser.open("http://localhost:4567/alert.html")->asHtml();
  }

  void testSpecificExpectedAlert()
  {
    browser.expectAlert("Hello Alert!");
    PageRef<HtmlPage> page = browser.open("http://localhost:4567/alert.html")->asHtml();
  }

  void testUnexpectedAlerts()
  {
    ASSERT_THROW(browser.open("http://localhost:4567/alert.html"), UnexpectedPopupError);
    browser.expectAlert("Hello Other Alert!");
    ASSERT_THROW(browser.open("http://localhost:4567/alert.html"), UnexpectedPopupError);
  }

  void testAnyExpectedConfirm()
  {
    browser.expectConfirm(true);
    PageRef<HtmlPage> page = browser.open("http://localhost:4567/confirm.html")->asHtml();
    ASSERT_EQUAL(page->evaluate("res"), "true");
  }

  void testSpecificExpectedConfirm()
  {
    browser.expectConfirm("Are you sure?", false);
    PageRef<HtmlPage> page = browser.open("http://localhost:4567/confirm.html")->asHtml();
    ASSERT_EQUAL(page->evaluate("res"), "false");
  }

  void testUnexpectedConfirm()
  {
    ASSERT_THROW(browser.open("http://localhost:4567/confirm.html"), UnexpectedPopupError);
    browser.expectConfirm("Foobar!", true);
    ASSERT_THROW(browser.open("http://localhost:4567/confirm.html"), UnexpectedPopupError);
    browser.expectAlert("Bla!");
    ASSERT_THROW(browser.open("http://localhost:4567/confirm.html"), UnexpectedPopupError);
  }

  void testAnyExpectedPrompt()
  {
    browser.expectPrompt("foobar");
    PageRef<HtmlPage> page = browser.open("http://localhost:4567/prompt.html")->asHtml();
    ASSERT_EQUAL(page->evaluate("res"), "foobar");
  }

  void testSpecificExpectedPrompt()
  {
    browser.expectPrompt("What's your name?", "Chris");
    PageRef<HtmlPage> page = browser.open("http://localhost:4567/prompt.html")->asHtml();
    ASSERT_EQUAL(page->evaluate("res"), "Chris");
  }

  void testUnexpectedPrompt()
  {
    ASSERT_THROW(browser.open("http://localhost:4567/prompt.html"), UnexpectedPopupError);
    browser.expectPrompt("Foobar!", "Bla!");
    ASSERT_THROW(browser.open("http://localhost:4567/prompt.html"), UnexpectedPopupError);
    browser.expectAlert("Bla!");
    ASSERT_THROW(browser.open("http://localhost:4567/prompt.html"), UnexpectedPopupError);
  }

  void testCancelPrompt()
  {
    browser.expectPrompt("What's your name?", kCancelPrompt);
    PageRef<HtmlPage> page = browser.open("http://localhost:4567/prompt.html")->asHtml();
    ASSERT_EQUAL(page->evaluate("res"), "null");
  }
  
  void testNotFullyMetExpectations()
  {
    browser.expectConfirm("Foobar!");
    browser.expectAlert("Foobar!");
    ASSERT_THROW(browser.open("http://localhost:4567/confirm.html"), ExpectationNotMetError);
  }

  void testFramesLength()
  {
    PageRef<HtmlPage> page = browser.open("http://localhost:4567/simple.html")->asHtml();
    ASSERT_EQUAL(page->evaluate("window.length"), "0");
    page = browser.open("http://localhost:4567/iframes.html")->asHtml();
    ASSERT_EQUAL(page->evaluate("window.length"), "2");
  }

  void testFrames()
  {
    PageRef<HtmlPage> page = browser.open("http://localhost:4567/simple.html")->asHtml();
    ASSERT_EQUAL(page->evaluate("window[0]"), "undefined");
    page = browser.open("http://localhost:4567/iframes.html")->asHtml();
    ASSERT_NOT_EQUAL(page->evaluate("window[0]"), "undefined");
    ASSERT_NOT_EQUAL(page->evaluate("window[1]"), "undefined");
    ASSERT_EQUAL(page->evaluate("window['foo'] == window[1]"), "true");
    ASSERT_EQUAL(page->evaluate("window[0].window == window[0]"), "true");
    ASSERT_EQUAL(page->evaluate("frames == window"), "true");
  }

  void testParentInTopFrame()
  {
    PageRef<HtmlPage> page = browser.open("http://localhost:4567/simple.html")->asHtml();
    ASSERT_EQUAL(page->evaluate("window.parent == window"), "true");
  }

  void testParentInInternalFrame()
  {
    PageRef<HtmlPage> page = browser.open("http://localhost:4567/iframes.html")->asHtml();
    ASSERT_EQUAL(page->evaluate("window[0].parent == window"), "true");
  }

  // TODO: add tests for parent and top in deeper frames.

  void testTopInTopFrame()
  {
    PageRef<HtmlPage> page = browser.open("http://localhost:4567/simple.html")->asHtml();
    ASSERT_EQUAL(page->evaluate("window.top == window"), "true");
  }

  void testTopInInternalFrame()
  {
    PageRef<HtmlPage> page = browser.open("http://localhost:4567/iframes.html")->asHtml();
    ASSERT_EQUAL(page->evaluate("window[0].top == window"), "true");
  }

  void testClose()
  {
    int win_size = browser.getWindows().size();
    PageRef<HtmlPage> page = browser.open("http://localhost:4567/simple.html")->asHtml();
    ASSERT_EQUAL(win_size, browser.getWindows().size()-1);
    ASSERT_EQUAL(page->evaluate("window.close();"), "");
    ASSERT_EQUAL(win_size, browser.getWindows().size());
  }

  void testClosed()
  {
    PageRef<HtmlPage> page = browser.open("http://localhost:4567/simple.html")->asHtml();
    ASSERT_EQUAL(page->evaluate("window.closed"), "false");
    ASSERT_EQUAL(page->evaluate("window.close();"), "");
    ASSERT_EQUAL(page->evaluate("window.closed"), "true");
  }

  void testStatus()
  {
    PageRef<HtmlPage> page = browser.open("http://localhost:4567/simple.html")->asHtml();
    ASSERT_EQUAL(page->evaluate("window.status"), "");
    page->evaluate("window.status = 'hello';");
    ASSERT_EQUAL(page->evaluate("window.status"), "hello");
    ASSERT_EQUAL(page->getEnclosingFrame()->getStatus(), "hello");
  }

  void testDefaultStatus()
  {
    PageRef<HtmlPage> page = browser.open("http://localhost:4567/simple.html")->asHtml();
    ASSERT_EQUAL(page->evaluate("window.defaultstatus"), "");
    ASSERT_EQUAL(page->evaluate("window.defaultStatus"), "");
  }

  void testEval()
  {
    PageRef<HtmlPage> page = browser.open("http://localhost:4567/simple.html")->asHtml();
    ASSERT_EQUAL(page->evaluate("window.eval('var a = 1'); a;"), "1");
  }

  void testPrint()
  {
    PageRef<HtmlPage> page = browser.open("http://localhost:4567/simple.html")->asHtml();
    ASSERT_EQUAL(page->evaluate("window.print()"), "undefined");
  }

  void testFind()
  {
    PageRef<HtmlPage> page = browser.open("http://localhost:4567/simple.html")->asHtml();
    ASSERT_EQUAL(page->evaluate("window.find()"), "false");
  }

  void testBarInfo()
  {
    PageRef<HtmlPage> page = browser.open("http://localhost:4567/simple.html")->asHtml();
    ASSERT_EQUAL(page->evaluate("window.menubar.visible"), "false");
    ASSERT_EQUAL(page->evaluate("window.toolbar.visible"), "false");
    ASSERT_EQUAL(page->evaluate("window.locationbar.visible"), "false");
    ASSERT_EQUAL(page->evaluate("window.personalbar.visible"), "false");
    ASSERT_EQUAL(page->evaluate("window.scrollbars.visible"), "false");
    ASSERT_EQUAL(page->evaluate("window.statusbar.visible"), "false");
  }

  void testOpener()
  {
    // TODO: test it when window.open will be implemented
  }

  void testName()
  {
    PageRef<HtmlPage> page = browser.open("http://localhost:4567/iframes.html")->asHtml();
    ASSERT_EQUAL(page->evaluate("window.foo.name"), "foo");
  }
};

CPPUNIT_TEST_SUITE_REGISTRATION(MikeJavaScriptWindowTest);
