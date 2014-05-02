//
//ClientPoints.cpp
//

//gameincludes
#include "ClientPoints.h"

//engineincludes
#include "Event.h"
#include "GraphicsManager.h"
#include "LogManager.h"
#include "EventNetwork.h"

ClientPoints::ClientPoints(){
	setLocation(TOP_RIGHT);
	setViewString(POINTS_STRING);
	setColor(COLOR_YELLOW);
	//needtoupdatescoreeachsecond,socount"step"events
	registerInterest(NETWORK_EVENT);
}

//handleevent
//return0ifignored,else1
int ClientPoints::eventHandler(Event *p_e){

	//Parenthandleseventifscoreupdate
	if(ViewObject::eventHandler(p_e)){
		return 1;
	}
	if(static_cast<EventNetwork*>(p_e)->getTarget()=='P'){

		setValue(getValue()+static_cast<EventNetwork*>(p_e)->getX());
		return 1;
	}
	//ifwegethere,wehaveignoredthisevent
		return 0;
}

