///
/// Pause
/// 

#ifndef __PAUSE_H__
#define __PAUSE_H__

#include "Object.h"	

#define PAUSE_KEY 274		// Default is F10.

class Pause: public Object {
 private:
  int pause_key;		///< The pause key.

 public:
  Pause(int init_pause_key = PAUSE_KEY);
  int eventHandler(Event *p_e);
};
 
#endif // __PAUSE_H__
