#include <list>
#include <string.h>
#include <libxml/uri.h>

#include "utils/Helpers.h"
#include "html/HtmlPage.h"
#include "Browser.h"
#include "Window.h"

namespace mike
{
  /////////////////////////////// CONSTS  //////////////////////////////////////

  // links/buttons xpaths
  static const string kLinkXpath = \
    "//a[text()='%L' or @id='%L' or @name='%L']";
  static const string kButtonXpath = \
    "//button[text()='%L' or @id='%L' or @name='%L']";
  static const string kInputButtonXpath = \
    "//input[@type='submit' or @type='reset' or @type='image' or @type='button'][@value='%L' or @id='%L' or @name='%L']";

  // form fields xpaths
  static const string kSelectFieldXpath = \
    "//select[@name='%L' or @id='%L' or @id=//label[text()='%L']/@for]";
  static const string kTextareaXpath = \
    "//textarea[@name='%L' or @id='%L' or @id=//label[text()='%L']/@for]";
  static const string kInputFieldXpath = \
    "//input[@type!='hidden' and @type!='reset' and @type!='submit' and @type!='image'][@name='%L' or @id='%L' or @id=//label[text()='%L']/@for]";

  // complex locators
  static const string kLinkLocators[] = {
    kLinkXpath
  };
  static const string kButtonLocators[] = {
    kInputButtonXpath,
    kButtonXpath
  };
  static const string kLinkOrButtonLocators[] = {
    kLinkXpath,
    kButtonXpath,
    kInputButtonXpath
  };
  static const string kFormFieldLocators[] = {
    kInputFieldXpath,
    kSelectFieldXpath,
    kTextareaXpath
  };
  
  /////////////////////////////// HELPERS //////////////////////////////////////

  string buildXpathForLocator(const string parts[], int size, string locator)
  {
    locator = xpathSanitize(locator);
    string result[size];
    
    for (int i = 0, pos = 0; i < size; i++) {
      result[i] = parts[i];
      
      while ((pos = result[i].find("%L")) > 0)
	result[i] = result[i].substr(0, pos) + locator + result[i].substr(pos+2);
    }

    return strjoin(result, size, " | ");
  }

  /////////////////////////////// PUBLIC ///////////////////////////////////////

  //============================= LIFECYCLE ====================================
  
  HtmlPage::HtmlPage(Request* request)
    : XmlPage(request)
  {
    type_ = kHtmlPage;
    javaScriptHandler_ = NULL;
    framesCounter_ = 0;
  }

  HtmlPage::~HtmlPage()
  {
    delete javaScriptHandler_;
    delete_all< vector<Frame*> >(&frames_);
  }

  //============================= ACCESS     ===================================
  
  vector<HtmlElement*> HtmlPage::getElementsByXpath(string xpath)
  {
    return vector_cast<XmlElement,HtmlElement>(XmlPage::getElementsByXpath(xpath));
  }

  HtmlElement* HtmlPage::getElementByXpath(string xpath)
  {
    return (HtmlElement*)XmlPage::getElementByXpath(xpath);
  }

  HtmlElement* HtmlPage::getElementByPath(string path)
  {
    return (HtmlElement*)XmlPage::getElementByPath(path);
  }

  vector<HtmlElement*> HtmlPage::getElementsByTagName(string tag)
  {
    return vector_cast<XmlElement,HtmlElement>(XmlPage::getElementsByTagName(tag));
  }
  
  vector<HtmlElement*> HtmlPage::getElementsByCss(string rule)
  {
    // TODO: implement this someday...
    throw "HtmlPage::getElementsByCss is not implemented yet.";
  }
  
  HtmlElement* HtmlPage::getElementByCss(string rule)
  {
    // TODO: ...
    throw "HtmlPage::getElementByCss is not implemented yet.";
  }

  vector<HtmlElement*> HtmlPage::getElementsByClassName(string klass)
  {
    // TODO: write it without using xpath... we neeeed speeeeeed!
    klass = xpathSanitize(klass);
    return getElementsByXpath("//*[contains(concat(' ', @class, ' '), ' " + klass + " ')]");
  }
    
  HtmlElement* HtmlPage::getElementById(string id)
  {
    // TODO: the same as above, write without xpath!
    id = xpathSanitize(id);
    return getElementByXpath("//*[@id='" + id + "'][1]");
  }

  HtmlElement* HtmlPage::getLinkOrButton(string locator)
  {
    string xpath = buildXpathForLocator(kLinkOrButtonLocators, 3, locator);
    return getElementByXpath(xpath);
  }

  HtmlElement* HtmlPage::getLink(string locator)
  {
    string xpath = buildXpathForLocator(kLinkLocators, 1, locator);
    return getElementByXpath(xpath);
  }

  HtmlElement* HtmlPage::getButton(string locator)
  {
    string xpath = buildXpathForLocator(kButtonLocators, 2, locator);
    return getElementByXpath(xpath);
  }

  HtmlElement* HtmlPage::getField(string locator)
  {
    string xpath = buildXpathForLocator(kFormFieldLocators, 3, locator);
    return getElementByXpath(xpath);
  }

  HtmlElement* HtmlPage::getElement(LocatorType type, string locator)
  {
    switch (type) {
    case kByXpath:
      return getElementByXpath(locator);
    case kByCss:
      return getElementByCss(locator);
    case kById:
      return getElementById(locator);
    case kByPath:
      return getElementByPath(locator);
    }
  }
  
  HtmlElement* HtmlPage::getElement(string locator)
  {
    return getLinkOrButton(locator);
  }
  
  string HtmlPage::getTitle()
  {
    try {
      HtmlElement* title = getElementByXpath("//html/head/title");
      return title->getContent();
    } catch (ElementNotFoundError err) {
      return "";
    }
  }

  string HtmlPage::getTitleText()
  {
    return getTitle();
  }

  vector<Frame*>& HtmlPage::getFrames()
  {
    return frames_;
  }

  Frame* HtmlPage::getFrame(int n)
  {
    if (n < frames_.size()) {
      return frames_[n];
    } else {
      throw FrameNotExistsError();
    }
  }

  Frame* HtmlPage::getFrame(string name)
  {
    for (vector<Frame*>::iterator it = frames_.begin(); it != frames_.end(); it++) {
      if ((*it)->getName() == name) {
	return *it;
      }
    }

    throw NamedFrameNotExistsError(name);
  }

  Frame* HtmlPage::getNamedFrame(string name)
  {
    return getFrame(name);
  }

  //============================= OPERATIONS ===================================
  
  void HtmlPage::reload()
  {
    Page::reload();
    delete_all< vector<Frame*> >(&frames_);

    if (frame_)
      enclose(frame_);
  }

  void HtmlPage::stop()
  {
    Page::stop();
  }
  
  string HtmlPage::evaluate(string script)
  {
    try {
      // TODO: replace 0 with eval line number
      return javaScriptHandler_->evaluate(script, "<eval>", 0);
    } catch (CloseWindow cw) {
      return "";
    }
  }

  void HtmlPage::click(string locator)
  {
    clickLinkOrButton(locator);
  }
  
  void HtmlPage::click(LocatorType type, string locator)
  {
    getElement(type, locator)->click();
  }

  void HtmlPage::clickLinkOrButton(string locator)
  {
    getLinkOrButton(locator)->click();
  }

  void HtmlPage::clickLink(string locator)
  {
    getLink(locator)->click();
  }

  void HtmlPage::pressButton(string locator)
  {
    getButton(locator)->click();
  }

  void HtmlPage::clickButton(string locator)
  {
    pressButton(locator);
  }
  
  /////////////////////////////// PROTECTED ////////////////////////////////////

  void HtmlPage::enclose(Frame* frame)
  {
    Page::enclose(frame);

    // We have to initialize javascript handler before frames will be loaded,
    // because they are going to inherit owner's context.
    delete javaScriptHandler_;
    javaScriptHandler_ = new JavaScriptHandler(this);
    
    loadFrames();
    processScripts();

    // Checking user expectations. 
    checkExpectations();
  }

  void HtmlPage::checkExpectations()
  {
    Browser* browser = getEnclosingFrame()->getBrowser();
    list<PopupExpectation>& expects = browser->getExpectations();

    if (!expects.empty()) {
      PopupExpectation first = expects.front();
      ExpectationNotMetError err(first.kind, first.message);
      browser->clearExpectations();
      throw err;
    }
  }

  void HtmlPage::parseDocument()
  {
    xmlChar* body = xmlCharStrdup(getResponse()->getBody().c_str());
    doc_ = htmlParseDoc(body, NULL);
    xmlFree(body);
  }

  XmlElement* HtmlPage::newElement(xmlNodePtr node)
  {
    return (XmlElement*)HtmlElement::Factory(this, node);
  }

  void HtmlPage::processScripts()
  {
    if (frame_ && frame_->getBrowser()->isJavaEnabled()) {
      // Removing all noscript sections.
      removeNoScriptNodes();

      // Finding all java scripts.
      vector<HtmlElement*> scripts = getElementsByXpath("//script[@type='text/javascript' or not(@type)]");

      for (vector<HtmlElement*>::iterator it = scripts.begin(); it != scripts.end(); it++) {
	if (isStopped())
	  return;
	
	HtmlElement* script = *it;
	string content, filename;
	unsigned int line = 0;
	  
	if (script->hasAttribute("src")) {
	  filename = getUrlFor(script->getAttribute("src"));
	  
	  if (!loadAsset(filename, &content)) {
	    // TODO: debug info...
	    continue;
	  }
	} else {
	  filename = getUrl();
	  content = script->getContent();
	  line = xmlGetLineNo(script->node_);
	}

	try {
	  javaScriptHandler_->evaluate(content, filename, line);
	} catch (CloseWindow cw) {
	  // TODO: run onclose callbacks!...
	  return;
	}
      }
    }
  }

  bool HtmlPage::loadAsset(string uri, string* content)
  {
    try {
      Browser* browser = frame_->getBrowser();
      Page* asset = Page::Open(uri, browser->isCookieEnabled(), browser->getSessionToken(), true);
      *content = asset->getContent();
      delete asset;
      return true;
    } catch (ConnectionError err) {
      return false;
    }
  }

  void HtmlPage::removeNoScriptNodes()
  {
    vector<HtmlElement*> nodes = getElementsByTagName("noscript");

    for (vector<HtmlElement*>::iterator it = nodes.begin(); it != nodes.end(); it++) {
      xmlNodePtr noscript = (*it)->node_;
      xmlNodePtr node = noscript->children;
      noscript->children = NULL;
      
      while (node) {
	xmlNodePtr tmp = node;
	node = node->next;
	xmlUnlinkNode(tmp);
	xmlFree(tmp);
      }
    }
  }
  
  // XXX: add infinity loop prevention for frames opening!
  void HtmlPage::loadFrames()
  {
    if (frame_) {
      delete_all< vector<Frame*> >(&frames_);
      
      vector<HtmlElement*> frames = getElementsByXpath("//iframe | //frameset//frame");
      Browser* browser = frame_->getBrowser();
      framesCounter_ = 0;
      
      for (vector<HtmlElement*>::iterator it = frames.begin(); it != frames.end(); it++) {
	HtmlElement* elem = *it;
	
	if (elem->hasAttribute("src")) {
	  try {
	    string uri = getUrlFor(elem->getAttribute("src"));
	    Page* page = Page::Open(uri, browser->isCookieEnabled(), browser->getSessionToken());

	    Frame* frame = new Frame(frame_);
	    frames_.push_back(frame);

	    if (elem->hasAttribute("name")) {
	      frame->setName(elem->getAttribute("name"));
	    }

	    frame->setIndex(framesCounter_++);
	    frame->setPage(page);
	  } catch (ConnectionError err) {
	    // TODO: debug info...
	  }
	}
      }
    }
  }

  Handle<Context> HtmlPage::getScriptContext()
  {
    return javaScriptHandler_->getContext();
  }

  JavaScriptHandler* HtmlPage::getScriptHandler()
  {
    return javaScriptHandler_;
  }
}
