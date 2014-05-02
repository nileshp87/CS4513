//
// ServerPoints.h
// 

#include "ViewObject.h"
#include "Event.h"		

#define POINTS_STRING "Points"

class ServerPoints : public ViewObject {

 public:
  ServerPoints();
  int eventHandler(Event *p_e);
};

