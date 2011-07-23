#ifndef _MIKE_BROWSER_FRAME_H_
#define _MIKE_BROWSER_FRAME_H_

#include <vector>
#include <string>

#include "History.h"

namespace mike
{
  using namespace std;

  class Browser;
  class Window;
  class Page;

  /**
   * Frame has two jobs to do. First, each window contains at least one frame - you can
   * consider it as main window content. Also each page may contain many frames representing
   * DOM 'iframe' and 'frame' elements. In second case frames are attached to main window
   * through frame which contains opened page.
   *
   * \code
   *   Frame* frame = new Frame(window);
   *   frame->setPage(page);
   *   //...
   *   frame->cleanup();
   *   delete frame;
   * \endcode
   *
   * Each frame maintains it's own history container, so you can navigate through all
   * recently opened page.
   */
  class Frame
  {
  public:
    /**
     * Creates instance of frame within given window.
     *
     * \param windwo Window which contains this frame.
     */
    explicit Frame();
    explicit Frame(Window* window);
    explicit Frame(Frame* frame);
    
    /**
     * Destructor.
     */
    virtual ~Frame();

    /**
     * Assigns page to this frame.
     *
     * \param page Page to open in this frame.
     */
    void setPage(Page* page);
    
    /**
     * \return Currently open page. 
     */
    Page* getPage();
    Page* getEnclosedPage();

    /**
     * \return History container for this frame.
     */
    History getHistory() const;

    /**
     * \return Window which contains this frame.
     */
    Window* getWindow() const;

    /**
     * \return Parent frame.
     */
    Frame* getParent() const;
    
    /**
     * Sets name of this frame.
     *
     * \param name New name for this frame.
     */
    void setName(string name);

    /**
     * \return name of this frame.
     */
    string getName() const;

  protected:
    string name_;
    History history_;
    Window* window_;
    Frame* parent_;
  };
}

#endif /* _MIKE_FRAME_H_ */
