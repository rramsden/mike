#include "test/Macros.h"

#include "html/HtmlPage.h"
#include "Window.h"
#include "Browser.h"

using namespace std;
using namespace mike;

class MikeJavaScriptNavigatorTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(MikeJavaScriptNavigatorTest);
  CPPUNIT_TEST(testWindowNavigator);
  CPPUNIT_TEST(testAppCodeName);
  CPPUNIT_TEST(testDefaultLanguage);
  CPPUNIT_TEST(testDefaultOnline);
  CPPUNIT_TEST_SUITE_END();

protected:

  Browser browser;

  void testWindowNavigator()
  {
    PageRef<HtmlPage> page = browser.open("http://localhost:4567/simple.html")->asHtml();
    ASSERT_EQUAL(page->evaluate("navigator"), "[object Navigator]");
    ASSERT_EQUAL(page->evaluate("navigator == window.navigator"), "true");
  }

  void testAppCodeName()
  {
    PageRef<HtmlPage> page = browser.open("http://localhost:4567/simple.html")->asHtml();
    ASSERT_EQUAL(page->evaluate("navigator.appCodeName"), "Mozilla");
  }

  void testDefaultLanguage()
  {
    PageRef<HtmlPage> page = browser.open("http://localhost:4567/simple.html")->asHtml();
    ASSERT_EQUAL(page->evaluate("navigator.language"), "en");
  }

  void testDefaultOnline()
  {
    PageRef<HtmlPage> page = browser.open("http://localhost:4567/simple.html")->asHtml();
    ASSERT_EQUAL(page->evaluate("navigator.onLine"), "true");
  }
};

CPPUNIT_TEST_SUITE_REGISTRATION(MikeJavaScriptNavigatorTest);
