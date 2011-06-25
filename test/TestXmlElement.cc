#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <string.h>

#include "utils/CppunitMacros.h"
#include "xml/XmlPage.h"

using namespace std;
using namespace mike;

class MikeXmlElementTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(MikeXmlElementTest);
  CPPUNIT_TEST(testGetAttribute);
  CPPUNIT_TEST(testHasAttribute);
  CPPUNIT_TEST(testGetContent);
  CPPUNIT_TEST(testHasContent);
  CPPUNIT_TEST(testGetName);
  CPPUNIT_TEST(testHasChildren);
  CPPUNIT_TEST_SUITE_END();

protected:

  void testGetAttribute()
  {
    http::Request* request = http::Request::Get("http://localhost:4567/xml-elements.xml");
    XmlPage* page = Page::Build(request)->toXmlPage();
    ASSERT(page);
    XmlElement* elem = page->getElementsByTagName("elem")[0];
    ASSERT_NOT_NULL(elem);
    ASSERT_EQUAL(elem->getAttribute("foo"), "bar");
    ASSERT_EQUAL(elem->getAttribute("foo"), "bar"); // it has to be done twice here, to test attrs cache...
    ASSERT_EQUAL(elem->getAttribute("bar"), "");
    delete elem;
    delete page;
  }

  void testHasAttribute()
  {
    http::Request* request = http::Request::Get("http://localhost:4567/xml-elements.xml");
    XmlPage* page = Page::Build(request)->toXmlPage();
    ASSERT(page);
    XmlElement* elem = page->getElementsByTagName("elem")[0];
    ASSERT_NOT_NULL(elem);
    ASSERT(elem->hasAttribute("foo"));
    ASSERT_NOT(elem->hasAttribute("bar"));
    ASSERT(elem->hasAttribute("foo", "bar"));
    ASSERT_NOT(elem->hasAttribute("foo", "not-like-this"));
    delete elem;
    delete page;
  }

  void testGetContent()
  {
    http::Request* request = http::Request::Get("http://localhost:4567/xml-elements.xml");
    XmlPage* page = Page::Build(request)->toXmlPage();
    ASSERT(page);
    XmlElement* elem = page->getElementsByTagName("elem")[0];
    ASSERT_NOT_NULL(elem);
    ASSERT_EQUAL(elem->getContent(), "Hello Foo!");
    delete elem;
    delete page;
  }

  void testHasContent()
  {
    http::Request* request = http::Request::Get("http://localhost:4567/xml-elements.xml");
    XmlPage* page = Page::Build(request)->toXmlPage();
    ASSERT(page);
    XmlElement* elem = page->getElementsByTagName("elem")[0];
    ASSERT_NOT_NULL(elem);
    ASSERT(elem->hasContent("Hello Foo!"));
    ASSERT_NOT(elem->hasContent("Not like this!"));
    ASSERT(elem->hasContent());
    delete elem;
    delete page;
  }

  void testGetName()
  {
    http::Request* request = http::Request::Get("http://localhost:4567/xml-elements.xml");
    XmlPage* page = Page::Build(request)->toXmlPage();
    ASSERT(page);
    XmlElement* elem = page->getElementsByTagName("elem")[0];
    ASSERT_NOT_NULL(elem);
    ASSERT_EQUAL(elem->getName(), "elem");
    delete elem;
    delete page;
  }

  void testHasChildren()
  {
    http::Request* request = http::Request::Get("http://localhost:4567/anchors.html");
    XmlPage* page = Page::Build(request)->toXmlPage();
    ASSERT(page);
    XmlElement* elem1 = page->getElementsByTagName("a")[0];
    ASSERT_NOT_NULL(elem1);
    ASSERT(elem1->hasChildren());
    XmlElement* elem2 = page->getElementsByTagName("input")[0];
    ASSERT_NOT_NULL(elem2);
    ASSERT_NOT(elem2->hasChildren());
    delete elem1;
    delete elem2;
    delete page;
  }

};

CPPUNIT_TEST_SUITE_REGISTRATION(MikeXmlElementTest);
