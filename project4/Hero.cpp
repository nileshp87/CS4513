//
// Hero.cpp
//

// engine includes
#include "EventStep.h"
#include "EventView.h"
#include "GraphicsManager.h"
#include "LogManager.h"
#include "ResourceManager.h"
#include "WorldManager.h"

// game includes
#include "Bullet.h"
#include "EventNuke.h"
#include "Explosion.h"
#include "GameOver.h"
#include "GameStart.h"
#include "Hero.h"

Hero::Hero() {

  // link to "ship" sprite
  ResourceManager &resource_manager = ResourceManager::getInstance();
  LogManager &log_manager = LogManager::getInstance();
  Sprite *p_temp_sprite;
  p_temp_sprite = resource_manager.getSprite("ship");
  if (!p_temp_sprite) {
    log_manager.writeLog("Hero::Hero(): Warning! Sprite '%s' not found", "ship");
  } else {
    setSprite(p_temp_sprite);
    setSpriteSlowdown(3);		  // third speed animation
    setTransparency();			  // transparent sprite
  }

  // player controls hero, so register with keyboard
  registerInterest(KEYBOARD_EVENT);

  // need to update fire rate control each step
  registerInterest(STEP_EVENT);

  setType("Hero");

  // set starting location
  WorldManager &world_manager = WorldManager::getInstance();
  Position pos(7, world_manager.getBoundary().getVertical()/2);
  setPosition(pos);

  fire_slowdown = 15;
  fire_countdown = fire_slowdown;

  nuke_count = 1;
}

Hero::~Hero() {

  // create GameOver object
  GameOver *p_go = new GameOver;

  // make big explosion
  for (int i=-8; i<=8; i+=5) {
    for (int j=-5; j<=5; j+=3) {
      Position temp_pos = this->getPosition();
      temp_pos.setX(this->getPosition().getX() + i);
      temp_pos.setY(this->getPosition().getY() + j);
      Explosion *p_explosion = new Explosion;
      p_explosion -> setPosition(temp_pos);
    }
  }
}

// handle event
// return 0 if ignored, else 1
int Hero::eventHandler(Event *p_e) {

  if (p_e->getType() == KEYBOARD_EVENT) {
    EventKeyboard *p_keyboard_event = static_cast <EventKeyboard *> (p_e);
    kbd(p_keyboard_event);
    return 1;
  }

  if (p_e->getType() == STEP_EVENT) {
    step();
    return 1;
  }

  // if we get here, we have ignored this event
  return 0;
}

// call move (or do nothing) according to key pressed
void Hero::kbd(EventKeyboard *p_keyboard_event) {
  WorldManager &world_manager = WorldManager::getInstance();

  switch(p_keyboard_event->getKey()) {
  case KEY_UP:			// up arrow
    move(-1);
    break;
  case KEY_DOWN:		// down arrow
    move(+1);
    break;
  case ' ':			// fire
    fire();
    break;
  case 13:			// nuke!
    nuke();
    break;
  case 'q':			// quit
    world_manager.markForDelete(this);
    break;
  };
  return;
}

// move up or down
void Hero::move(int dy) {
  WorldManager &world_manager = WorldManager::getInstance();
  Position new_pos(getPosition().getX(), getPosition().getY() + dy);

  // if stays on screen, allow move
  if ((new_pos.getY() > 3) && 
      (new_pos.getY() < world_manager.getBoundary().getVertical()))
    world_manager.moveObject(this, new_pos);
}

// fire a bullet
void Hero::fire() {
  if (fire_countdown > 0)
    return;
  fire_countdown = fire_slowdown;
  new Bullet(getPosition());
}

// decrease fire restriction
void Hero::step() {
  fire_countdown--;
  if (fire_countdown < 0)
    fire_countdown = 0;
}

// send nuke event to all objects
void Hero::nuke() {

  // check if nukes left
  if (!nuke_count) 
    return;
  nuke_count--;

  // create a "nuke" event and send to interested 
  WorldManager &world_manager = WorldManager::getInstance();
  EventNuke nuke;
  world_manager.onEvent(&nuke);

  // send "view" event with nukes to interested ViewObjects
  EventView ev("Nukes", -1, true);
  world_manager.onEvent(&ev);
}
