ACLOCAL_AMFLAGS = -I build-m4

SOURCE_FILES = \
	src/Page.cc \
	src/History.cc \
	src/Browser.cc \
	src/Window.cc \
	src/Frame.cc \
	src/utils/Helpers.cc \
	src/utils/Base64.cc \
	src/http/Response.cc \
	src/http/Request.cc \
	src/http/Headers.cc \
	src/xml/XmlElement.cc \
	src/xml/XmlPage.cc \
	src/html/HtmlPage.cc \
	src/html/HtmlElement.cc \
	src/html/HtmlAnchorElement.cc \
	src/javascript/glue/ObjectWrap.cc \
	src/javascript/glue/WindowWrap.cc \
	src/javascript/glue/NavigatorWrap.cc \
	src/javascript/glue/HistoryWrap.cc \
	src/javascript/glue/BarInfo.cc \
	src/javascript/JavaScriptHandler.cc

# Build this part only if unit/integration testing is enabled.
if HAVE_CPPUNIT

TEST_FILES = \
	test/TestHttp.cc \
	test/TestPage.cc \
	test/TestXmlPage.cc \
	test/TestXmlElement.cc \
	test/TestHtmlPage.cc \
	test/TestHtmlElement.cc \
	test/TestHistory.cc \
	test/TestBrowser.cc \
	test/TestUtils.cc \
	test/TestPector.cc \
	test/TestJavaScriptWindow.cc \
	test/TestJavaScriptNavigator.cc \
	test/TestJavaScriptHistory.cc \
	test/Main.cc

# Don't install this program but run as test suite.
noinst_PROGRAMS = test-suite
check_PROGRAMS  = test-suite

test_suite_SOURCES = \
	$(SOURCE_FILES) \
	$(TEST_FILES)

test_suite_CPPFLAGS = $(CPPUNIT_FLAGS) -Isrc
test_suite_LDFLAGS  = $(CPPUNIT_LIBS)

TESTS = test-suite

endif # HAVE_CPPUNIT

# Custom targets.

tmp-dir:
	@test -d tmp || mkdir tmp

dummy-stop:
	@test -f tmp/dummy.pid && kill -9 $$(cat tmp/dummy.pid) &>/dev/null; true
	@rm -f tmp/dummy.pid

dummy-start: tmp-dir
	@chmod u+x ./test/fixtures/dummy
	./test/fixtures/dummy &>tmp/dummy.log & echo $$! >tmp/dummy.pid

dummy-restart: dummy-stop dummy-start

test: test-suite dummy-restart
	@sleep 5
	./test-suite
