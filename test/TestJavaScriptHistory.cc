#include "test/Macros.h"

#include "html/HtmlPage.h"
#include "Window.h"
#include "Browser.h"

using namespace std;
using namespace mike;

class MikeJavaScriptHistoryTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(MikeJavaScriptHistoryTest);
  CPPUNIT_TEST(testWindowHistory);
  CPPUNIT_TEST(testLength);
  CPPUNIT_TEST(testBack);
  CPPUNIT_TEST(testForward);
  CPPUNIT_TEST(testGo);
  CPPUNIT_TEST_SUITE_END();

protected:

  Browser browser;

  void testWindowHistory()
  {
    PageRef<HtmlPage> page = browser.open("http://localhost:4567/simple.html")->asHtml();
    ASSERT_EQUAL(page->evaluate("history"), "[object History]");
    ASSERT_EQUAL(page->evaluate("history == window.history"), "true");
  }

  void testLength()
  {
    PageRef<HtmlPage> page = browser.open("http://localhost:4567/history.html")->asHtml();
    ASSERT_EQUAL(page->evaluate("history.length"), "1");
    page->clickLink("Next");
    ASSERT_EQUAL(page->evaluate("history.length"), "2");
    page->clickLink("Next");
    ASSERT_EQUAL(page->evaluate("history.length"), "3");
  }

  void testBack()
  {
    PageRef<HtmlPage> page = browser.open("http://localhost:4567/history.html")->asHtml();
    page->clickLink("Next");
    ASSERT_EQUAL(page->getUrl(), "http://localhost:4567/history/next1.html");
    page->evaluate("history.back()");
    ASSERT_EQUAL(page->getUrl(), "http://localhost:4567/history.html");
  }

  void testForward()
  {
    PageRef<HtmlPage> page = browser.open("http://localhost:4567/history.html")->asHtml();
    page->clickLink("Next");
    page->evaluate("history.back()");
    ASSERT_EQUAL(page->getUrl(), "http://localhost:4567/history.html");
    page->evaluate("history.forward()");
    ASSERT_EQUAL(page->getUrl(), "http://localhost:4567/history/next1.html");
  }

  void testGo()
  {
    PageRef<HtmlPage> page = browser.open("http://localhost:4567/history.html")->asHtml();
    page->clickLink("Next");
    page->clickLink("Next");
    page->clickLink("Next");
    page->evaluate("history.go(-2)");
    ASSERT_EQUAL(page->getUrl(), "http://localhost:4567/history/next1.html");
    page->evaluate("history.go(1)");
    ASSERT_EQUAL(page->getUrl(), "http://localhost:4567/history/next2.html");
  }
};

CPPUNIT_TEST_SUITE_REGISTRATION(MikeJavaScriptHistoryTest);
