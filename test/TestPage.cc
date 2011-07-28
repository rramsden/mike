#include "test/Macros.h"

#include "Page.h"
#include "Browser.h"
#include "Window.h"
#include "Frame.h"

using namespace std;
using namespace mike;
using namespace mike::http;

class MikePageTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(MikePageTest);
  CPPUNIT_TEST(testOpen);
  CPPUNIT_TEST(testFactoryWhenInvalidUrlGiven);
  CPPUNIT_TEST(testGetContent);
  CPPUNIT_TEST(testGetUrl);
  CPPUNIT_TEST(testGetStream);
  CPPUNIT_TEST(testGetEnclosingFrame);
  CPPUNIT_TEST(testGetEnclosingWindow);
  CPPUNIT_TEST_SUITE_END();

protected:

  void testOpen()
  {
    Page* page = Page::Open("http://localhost:4567/simple");
    ASSERT_EQUAL(page->getContent(), "Kukuryku!");
    delete page;
  }
  
  void testFactoryWhenInvalidUrlGiven()
  {
    Request* request = Request::Get("http://thiswebsiteforsure/not/exists");
    ASSERT_THROW(Page::Factory(request), ConnectionError);
  }

  void testGetContent()
  {
    Page* page = Page::Open("http://localhost:4567/simple.txt");
    ASSERT_EQUAL(page->getContent(), "Simple!");
    delete page;
  }

  void testGetUrl()
  {
    Page* page = Page::Open("http://localhost:4567/simple.txt");
    ASSERT_EQUAL(page->getUrl(), "http://localhost:4567/simple.txt");
    delete page;
  }
  
  void testGetStream()
  {
    Page* page = Page::Open("http://localhost:4567/simple.txt");
    ASSERT_NOT_NULL(page->getStream());
    ASSERT_EQUAL(page->getStream()->str(), "Simple!");
    delete page;
  }

  void testGetEnclosingFrame()
  {
    Browser browser;
    Page* page = browser.open("http://localhost:4567/simple.txt");
    ASSERT_EQUAL(page->getEnclosingFrame()->getPage(), page);
  }

  void testGetEnclosingWindow()
  {
    Browser browser = Browser();
    Page* page = browser.open("http://localhost:4567/simple.txt");
    ASSERT_EQUAL(page->getEnclosingWindow(), browser.getWindow(0));
  }

};

CPPUNIT_TEST_SUITE_REGISTRATION(MikePageTest);
