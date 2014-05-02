//
// Star.cpp
//

#include <stdlib.h>		// for random

#include "Star.h"
#include "WorldManager.h"
#include "EventOut.h"
#include "GraphicsManager.h"

Star::Star() {
  setType("Star");
  setSolidness(SPECTRAL);
  setXVelocity(-1.0 / (random()%10 + 1));
  setAltitude(0);		// make them in the background
  WorldManager &world_manager = WorldManager::getInstance();
  Position pos(random()%world_manager.getBoundary().getHorizontal(),
	       random()%world_manager.getBoundary().getVertical());
  setPosition(pos);
}

void Star::draw() {
  GraphicsManager &graphics_manager = GraphicsManager::getInstance();
  graphics_manager.drawCh(getPosition(), STAR_CHAR); 
}

// handle event
// return 0 if ignored, else 1
int Star::eventHandler(Event *p_e) {

  if (p_e->getType() == OUT_EVENT) {
    out();
    return 1;
  }

  // if we get here, we have ignored this event
  return 0;
}

// if star moved off screen, move back to far right
void Star::out() {
  WorldManager &world_manager = WorldManager::getInstance();
  Position pos(world_manager.getBoundary().getHorizontal() + random()%20,
	       random() % world_manager.getBoundary().getVertical());
  setPosition(pos);
  setXVelocity(-1.0 / (random()%10 + 1));
}

