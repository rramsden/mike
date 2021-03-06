#include <stdlib.h>
#include "History.h"
#include "utils/Helpers.h"
#include "Page.h"

namespace mike
{
  /////////////////////////////// PUBLIC ///////////////////////////////////////

  //============================= LIFECYCLE ====================================

  History::History()
  {
    current_ = NULL;
  }

  History::~History()
  {
    delete_all< list<Page*> >(&back_);
    delete_all< list<Page*> >(&forward_);
    delete current_;
  }

  //============================= ACCESS     ===================================

  int History::size()
  {
    return back_.size() + forward_.size() + (current_ == NULL ? 0 : 1);
  }

  Page* History::getCurrent()
  {
    return current_;
  }

  //============================= OPERATIONS ===================================

  void History::push(Page* page)
  {
    if (current_)
      back_.push_back(current_);
    
    current_ = page;
    delete_all< list<Page*> >(&forward_);
  }

  void History::goBack()
  {
    go(-1);
  }

  void History::goForward()
  {
    go(1);
  }

  void History::go(int distance)
  {
    list<Page*>::iterator it;

    // Um, almost as ugly as predator, but equally fast... so don't mess with it
    // or it's gonna turn your skull into trophy! :]
    if (distance < 0) {
      if ((distance = abs(distance) <= back_.size() ? distance : -back_.size()) != 0) {
	it = back_.end();
	advance(it, distance+1);
	forward_.push_front(current_);
	forward_.splice(forward_.begin(), back_, it, back_.end());
	current_ = back_.back();
	back_.pop_back();
      }
    } else if (distance > 0) {
      if ((distance = distance <= forward_.size() ? distance : forward_.size()) != 0) {
	it = forward_.begin();
	advance(it, distance-1);
	back_.push_back(current_);
	back_.splice(back_.end(), forward_, forward_.begin(), it);
	current_ = forward_.front();
	forward_.pop_front();
      }
    }

    reloadCurrent();
  }
  
  /////////////////////////////// PROTECTED  ///////////////////////////////////

  void History::reloadCurrent()
  {
    if (current_) {
      current_->reload();
    }
  }
}
