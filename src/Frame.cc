#include <assert.h>

#include "Browser.h"
#include "Window.h"
#include "Frame.h"
#include "Page.h"
#include "History.h"
#include "html/HtmlPage.h"
#include "javascript/JavaScriptHandler.h"

namespace mike
{
  /////////////////////////////// PUBLIC ///////////////////////////////////////

  //============================= LIFECYCLE ====================================

  Frame::Frame(Browser* browser)
  {
    init();
    parent_ = this;
    browser_ = browser;
  }
  
  Frame::Frame(Frame* parent)
  {
    init();
    // TODO: setup frame proportions!
    parent_ = parent;
    browser_ = parent->browser_;
  }

  Frame::~Frame()
  {
    for (list<Frame*>::iterator it = opened_.begin(); it != opened_.end(); it++)
      (*it)->removeOpener();
  }

  //============================= ACCESS     ===================================
  
  void Frame::setPage(Page* page)
  {
    history_.push(page);
    page->enclose(this);
  }
  
  Page* Frame::getPage()
  {
    return history_.getCurrent();
  }

  string Frame::getUrl()
  {
    return isBlank() ? "about:blank" : getPage()->getUrl();
  }

  void Frame::setName(string name)
  {
    name_ = name;
  }

  string Frame::getName()
  {
    return name_;
  }

  void Frame::setIndex(int index)
  {
    index_ = index;
  }

  int Frame::getIndex()
  {
    return index_;
  }

  void Frame::setSize(int width, int height)
  {
    width_  = width;
    height_ = height;
  }

  int Frame::getWidth()
  {
    return width_;
  }

  void Frame::setWidth(int width)
  {
    // TODO: fire onresize event!
    width_ = width;
  }

  int Frame::getHeight()
  {
    return height_;
  }

  void Frame::setHeight(int height)
  {
    // TODO: fire onresize event!
    height_ = height;
  }

  Browser* Frame::getBrowser() const
  {
    return browser_;
  }
  
  History Frame::getHistory() const
  {
    return history_;
  }

  Frame* Frame::getParent() const
  {
    return parent_;
  }

  Frame* Frame::getTop() const
  {
    Frame* top = getParent();

    while (top != top->getParent())
      top = top->getParent();

    return top;
  }

  Frame* Frame::getOpener()
  {
    return isTop() ? opener_ : getTop()->getOpener();
  }

  string Frame::getStatus()
  {
    return status_;
  }

  void Frame::setStatus(string value)
  {
    status_ = value;
  }
  
  bool Frame::isWindow() const
  {
    return false;
  }

  bool Frame::isClosed() const
  {
    return closed_;
  }

  bool Frame::isTop() const
  {
    return (this == getTop());
  }

  bool Frame::hasParent() const
  {
    return (this != getParent());
  }

  bool Frame::hasOpener()
  {
    return (opener_ != NULL);
  }

  bool Frame::isBlank() const
  {
    return false; //(getPage() == NULL);
  }

  //============================= OPERATIONS ===================================

  void Frame::close()
  {
    // TODO: ...
  }

  /////////////////////////////// PROTECTED ////////////////////////////////////

  void Frame::setOpener(Frame* opener)
  {
    opener_ = opener;
    opener_->opened_.push_back(this);
  }

  void Frame::removeOpener()
  {
    if (opener_) {
      opener_->opened_.remove(this);
      opener_ = NULL;
    }
  }
  
  Handle<Context> Frame::getScriptContext()
  {
    assert(!isBlank());
    assert(getPage()->isHtml());
    return getPage()->asHtml()->getScriptContext();
  }

  JavaScriptHandler* Frame::getScriptHandler()
  {
    assert(!isBlank());
    assert(getPage()->isHtml());
    return getPage()->asHtml()->getScriptHandler();
  }

  /////////////////////////////// PRIVATE //////////////////////////////////////

  void Frame::init()
  {
    name_   = "";
    status_ = "";
    closed_ = false;
    opener_ = NULL;
  }
}
