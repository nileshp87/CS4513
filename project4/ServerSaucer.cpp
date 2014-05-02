//
// ServerSaucer.cpp
//

#include <stdlib.h>		// for random()

// engine includes
#include "EventCollision.h"
#include "EventNuke.h"
#include "EventOut.h"
#include "EventView.h"
#include "LogManager.h"
#include "ResourceManager.h"
#include "WorldManager.h"

// game includes
#include "Explosion.h"
#include "Points.h"
#include "ServerSaucer.h"

ServerSaucer::ServerSaucer() {
  LogManager &log_manager = LogManager::getInstance();
  ResourceManager &resource_manager = ResourceManager::getInstance();

  // setup "saucer" sprite
  Sprite *p_temp_sprite = resource_manager.getSprite("saucer");
  if (!p_temp_sprite) {
    log_manager.writeLog("ServerSaucer::Saucer(): Warning! Sprite '%s' not found", 
			 "saucer");
  } else {
    setSprite(p_temp_sprite);
    setSpriteSlowdown(4);		
  }

  // set object type
  setType("ServerSaucer");

  // set speed in vertical direction
  setXVelocity(-0.25);		// 1 space every 4 frames

  moveToStart();

  // register interest in "nuke" event
  registerInterest(NUKE_EVENT);
}

ServerSaucer::~ServerSaucer() {
  // send "view" event with points to interested ViewObjects
  // add 10 points
  EventView ev(POINTS_STRING, 10, true);
  WorldManager &world_manager = WorldManager::getInstance();
  world_manager.onEvent(&ev);
}

// handle event
// return 0 if ignored, else 1
int ServerSaucer::eventHandler(Event *p_e) {

  if (p_e->getType() == OUT_EVENT) {
    out();
    return 1;
  }

  if (p_e->getType() == COLLISION_EVENT) {
    EventCollision *p_collision_event = static_cast <EventCollision *> (p_e);
    hit(p_collision_event);
    return 1;
  }

  if (p_e->getType() == NUKE_EVENT) {

    // create an explosion
    Explosion *p_explosion = new Explosion;
    p_explosion -> setPosition(this -> getPosition());

    // delete self
    WorldManager &world_manager = WorldManager::getInstance();
    world_manager.markForDelete(this);

    // ServerSaucers appear stay around perpetually
    new ServerSaucer;
  }

  // if we get here, we have ignored this event
  return 0;
}

// if moved off left edge, move back to far right
void ServerSaucer::out() {

  // if we haven't moved off left edge, then nothing to do
  if (getPosition().getX() >= 0)
    return;

  // otherwise, move back to far right
  moveToStart();

  // spawn a new ServerSaucer to make the game get harder
  new ServerSaucer;
}

// if saucer and player collide, mark both for deletion
void ServerSaucer::hit(EventCollision *p_c) {
  // if ServerSaucer on Saucer, ignore
  if ((p_c -> getObject1() -> getType() == "ServerSaucer") &&
      (p_c -> getObject2() -> getType() == "ServerSaucer"))
    return;

  // if Bullet 
  if ((p_c -> getObject1() -> getType() == "Bullet") ||
      (p_c -> getObject2() -> getType() == "Bullet")) {

    // Create an explosion
    Explosion *p_explosion = new Explosion;
    p_explosion -> setPosition(this -> getPosition());

    // ServerSaucers appear stay around perpetually
    new ServerSaucer;
  }

  // if hero, mark both objects for destruction 
  if (((p_c -> getObject1() -> getType()) == "Hero") || 
      ((p_c -> getObject2() -> getType()) == "Hero")) {
    WorldManager &world_manager = WorldManager::getInstance();
    world_manager.markForDelete(p_c -> getObject1());
    world_manager.markForDelete(p_c -> getObject2());
  }
}

// move saucer to starting location on right side of screen
void ServerSaucer::moveToStart() {
  WorldManager &world_manager = WorldManager::getInstance();
  Position temp_pos;

  int world_horiz = world_manager.getBoundary().getHorizontal();
  int world_vert = world_manager.getBoundary().getVertical();

  // x is off the right side of screen
  temp_pos.setX(world_horiz + random()%world_horiz + 3);

  // y is in the vertical range
  temp_pos.setY(random()%(world_vert-4) + 4);

  // if collision, move right slightly until empty space
  ObjectList collision_list = world_manager.isCollision(this, temp_pos);
  while (!collision_list.isEmpty()) {
    temp_pos.setX(temp_pos.getX()+1);
    collision_list = world_manager.isCollision(this, temp_pos);
  }

  world_manager.moveObject(this, temp_pos);
}
