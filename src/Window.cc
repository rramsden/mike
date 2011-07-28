#include "Browser.h"
#include "Window.h"
#include "Page.h"
#include "html/HtmlPage.h"

namespace mike
{
  /////////////////////////////// PUBLIC ///////////////////////////////////////

  //============================= LIFECYCLE ====================================

  Window::Window(Browser* browser)
    : Frame(browser)
  {
    init();
  }

  Window::Window(Window* parent_window)
    : Frame(parent_window->browser_)
  {
    init();
    parentWindow_ = parent_window;
  }

  //============================= ACCESS     ===================================
  
  Window* Window::getParentWindow()
  {
    return parentWindow_;
  }

  //============================= OPERATIONS ===================================
  
  void Window::close()
  {
    browser_->closeWindow(this);
  }

  /////////////////////////////// PROTECTED ////////////////////////////////////

  void Window::init()
  {
    setSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    browser_->windows_.push_back(this);
  }
}
