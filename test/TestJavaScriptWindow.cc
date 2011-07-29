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
  CPPUNIT_TEST_SUITE_END();

protected:

  Browser browser;
  
  void testWindowAndThisObject()
  {
    PageRef<HtmlPage> page = (HtmlPage*)browser.open("http://localhost:4567/simple.html");
    ASSERT_EQUAL(page->evaluate("window == this"), "true");
    ASSERT_EQUAL(page->evaluate("window = 'cantoverwrite'; window == this;"), "true");
    ASSERT_EQUAL(page->evaluate("window == window.window"), "true");
    ASSERT_EQUAL(page->evaluate("window == frames"), "true");
    ASSERT_EQUAL(page->evaluate("window.constructor.toString()"), "function DOMWindow() { [native code] }");
  }

  void testAnyExpectedAlert()
  {
    browser.expectAlert();
    PageRef<HtmlPage> page = (HtmlPage*)browser.open("http://localhost:4567/alert.html");
  }

  void testSpecificExpectedAlert()
  {
    browser.expectAlert("Hello Alert!");
    PageRef<HtmlPage> page = (HtmlPage*)browser.open("http://localhost:4567/alert.html");
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
    PageRef<HtmlPage> page = (HtmlPage*)browser.open("http://localhost:4567/confirm.html");
    ASSERT_EQUAL(page->evaluate("res"), "true");
  }

  void testSpecificExpectedConfirm()
  {
    browser.expectConfirm("Are you sure?", false);
    PageRef<HtmlPage> page = (HtmlPage*)browser.open("http://localhost:4567/confirm.html");
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
    PageRef<HtmlPage> page = (HtmlPage*)browser.open("http://localhost:4567/prompt.html");
    ASSERT_EQUAL(page->evaluate("res"), "foobar");
  }

  void testSpecificExpectedPrompt()
  {
    browser.expectPrompt("What's your name?", "Chris");
    PageRef<HtmlPage> page = (HtmlPage*)browser.open("http://localhost:4567/prompt.html");
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
    PageRef<HtmlPage> page = (HtmlPage*)browser.open("http://localhost:4567/prompt.html");
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
};

CPPUNIT_TEST_SUITE_REGISTRATION(MikeJavaScriptWindowTest);
