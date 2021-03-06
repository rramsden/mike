#ifndef _MIKE_FRAME_H_
#define _MIKE_FRAME_H_

#include <v8.h>
#include <vector>
#include <string>
#include <list>

#include "History.h"

namespace mike
{
  using namespace std;
  using namespace v8;

  class Browser;
  class Page;
  class JavaScriptHandler;

  namespace glue
  {
    class WindowWrap;
  }

  /**
   * Frame is a base class for all window objects. Also manages iframe and frameset
   * elements within HTML page.
   *
   * Each frame maintains it's own history container, so you can navigate through all
   * recently opened page.
   */
  class Frame
  {
    friend class glue::WindowWrap;
    
  public:
    /**
     * Creates instance of frame.
     */
    explicit Frame(Browser* browser);
    
    /**
     * Creates instance of frame within another one.
     *
     * \param frame Parent frame.
     */
    explicit Frame(Frame* parent);

    /**
     * Destructor.
     */
    virtual ~Frame();
    
    /**
     * Assigns page to this frame.
     * 
     * TODO: move to protected!
     *
     * \param page Page to open in this frame.
     */
    void setPage(Page* page);
    
    /**
     * \return Currently open page. 
     */
    Page* getPage();

    /**
     * \return URL of currently open page.
     */
    string getUrl();
    
    /**
     * Sets name of this frame.
     *
     * TODO: move to protected!
     *
     * \param name New name for this frame.
     */
    void setName(string name);

    /**
     * \return Name of this frame.
     */
    string getName();

    /**
     * Sets numeric index of this frame.
     *
     * \param index New index of this frame
     */
    void setIndex(int index);

    /**
     * \return Index of this frame.
     */
    int getIndex();

    /**
     * Sets initial size of this frame. This method does not fire any 'onresize'
     * window events.
     *
     * \param width Initial width.
     * \param height Initial height.
     */
    void setSize(int width, int height);

    /**
     * \return Inner width.
     */
    int getWidth();

    /**
     * Set new width.
     *
     * \param width New width.
     */
    void setWidth(int width);
    
    /**
     * \return Inner height.
     */
    int getHeight();

    /**
     * Set new height.
     *
     * \param width New height.
     */
    void setHeight(int height);
    
    /**
     * \return Instance of browser within which this window has been created.
     */
    Browser* getBrowser() const;
    
    /**
     * \return History container for this frame.
     */
    History& getHistory();

    /**
     * Returns the window that contains this one. If this is top level window then it
     * will return itself.
     *
     * \return Parent frame/window.
     */
    Frame* getParent() const;

    /**
     * Returns the top level window that contains this one. If this is top level window 
     * then it will return itself.
     *
     * \return Top level frame/window.
     */
    Frame* getTop() const;

    /**
     * \return Wheather this frame has opener or not.
     */
    bool hasOpener();

    /**
     * \return Opener window.
     */
    Frame* getOpener();

    /**
     * \return Status text.
     */
    string getStatus();

    /**
     * Set new status text.
     *
     * \param value New status text.
     */
    void setStatus(string value);
    
    /**
     * \return Wheather this frame is an window or not.
     */
    virtual bool isWindow() const;

    /**
     * \return Wheather this frame is closed or not.
     */
    bool isClosed() const;

    /**
     * \return Wheather this frame contains any page or not.
     */
    bool isBlank() const;

    /**
     * \return Wheather this frame is top level or not. 
     */
    bool isTop() const;

    /**
     * \return Wheather this frame has parent.
     */ 
    bool hasParent() const;

    /**
     * If frame is an top level window then closes it, otherwise no effect.
     */
    virtual void close();

  private:
    void init();
    
  protected:
    int index_;
    int width_;
    int height_;
    string name_;
    bool closed_;
    string status_;
    History history_;
    Browser* browser_;
    Frame* parent_;
    Frame* opener_;
    list<Frame*> opened_;

    void setOpener(Frame* opener);
    void removeOpener();

    Handle<Context> getScriptContext();
    JavaScriptHandler* getScriptHandler();
  };
}

#endif /* _MIKE_FRAME_H_ */
