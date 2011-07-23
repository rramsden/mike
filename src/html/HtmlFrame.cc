#include "html/HtmlPage.h"
#include "html/HtmlFrame.h"
#include "Frame.h"

namespace mike
{
  /////////////////////////////// PUBLIC ///////////////////////////////////////

  //============================= LIFECYCLE ====================================
  
  HtmlFrame::HtmlFrame(Frame* parent)
    : Frame(parent)
  {
  }

  //============================= ACCESS     ===================================

  HtmlPage* HtmlFrame::getPage()
  {
    return (HtmlPage*)Frame::getPage();
  }

  HtmlPage* HtmlFrame::getEnclosedPage()
  {
    return getPage();
  }
}
