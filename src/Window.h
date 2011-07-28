#ifndef _MIKE_WINDOW_H_
#define _MIKE_WINDOW_H_

#include <string>
#include <vector>

#include "Frame.h"

namespace mike
{
  using namespace std;

  class Browser;
  class Window;
  class Page;
  
  /**
   * Represents single window opened within browser instance. Window can be related with
   * browser directly or through parent window (eg. for popup simulation). Objects of this
   * class shouldn't be actually created directly, mainly they are kind of anchors which
   * controlls everything under the hood.
   */
  class Window : public Frame
  {
  public:
    // Default window proportions.
    static const int DEFAULT_WIDTH  = 1280;
    static const int DEFAULT_HEIGHT = 1024;

    /**
     * Creates new window.
     *
     * \param browser Browser containing this window.
     */
    explicit Window(Browser* browser);

    /**
     * Creates new window from given parent.
     *
     * \param paren Parent window. 
     */
    explicit Window(Window* parent);

    // override
    virtual void close();
    
    /**
     * \return Window which created this one.
     */
    Window* getParentWindow();
    
  private:
    /**
     * Shared initializer.
     */
    void init();

  protected:
    Window* parentWindow_;
  };
}

#endif /* _MIKE_WINDOW_H_ */
