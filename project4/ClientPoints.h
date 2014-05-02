//
//ClientPoints.h
//

#include "ViewObject.h"
#include "Event.h"		

#define POINTS_STRING "Points"

class ClientPoints : public ViewObject{

public:
	ClientPoints();
	int eventHandler(Event *p_e);
};

