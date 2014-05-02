//
// Points.h
// 

#include "ViewObject.h"
#include "Event.h"		

#define POINTS_STRING "Points"

class Points : public ViewObject {

 public:
  Points();
  int eventHandler(Event *p_e);
};

