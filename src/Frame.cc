#include "Browser.h"
#include "Window.h"
#include "Frame.h"
#include "Page.h"
#include "History.h"

namespace mike
{
  /////////////////////////////// PUBLIC ///////////////////////////////////////

  //============================= LIFECYCLE ====================================

  Frame::Frame(Browser* browser)
    : closed_(false)
  {
    parent_  = this;
    browser_ = browser;
  }
  
  Frame::Frame(Frame* parent)
    : closed_(false)
  {
    parent_  = parent;
    browser_ = parent->browser_;
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

  bool Frame::isWindow() const
  {
    return false;
  }

  bool Frame::isClosed() const
  {
    return closed_;
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
}
