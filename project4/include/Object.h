///
/// The base game world object
///

#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <string>

#include "Box.h"             ///< Objects have a bounding box.
#include "Event.h"           ///< Objects can handle events.
#include "Position.h"        ///< Objects need a location.
#include "Sprite.h"          ///< Objects (often) have sprites.

#define MAX_OBJ_EVENTS 100   ///< Max events Object can be interested in.

using std::string;           // Introduce "string" into namespace
/// Types of solidness of an Object.
enum Solidness {
  HARD,       ///< Object causes collisions and impedes.
  SOFT,       ///< Object causes collision, but doesn't impede.
  SPECTRAL    ///< Object doesn't cause collisions.
};
 
/// Count of number of modified attribute categories.
#define ATTR_COUNT 12

/// Categories of attributes that indicate modification.
enum ObjectAttribute {
  ID, 
  ACTIVE,
  VISIBLE,            
  EVENTS,
  BOX,                   
  POS,               
  TYPE,                
  SPRITE,
  ALTITUDE,               
  SOLIDNESS,        
  NO_SOFT,               
  VELOCITY,           
};

class Object {

 private:
  int id;		      ///< Unique Object id.
  bool is_active;             ///< If false, Object not acted upon.
  bool is_visible;            ///< If true, Object gets drawn.
  int event_count;            ///< Number of events of interest.
  string event_name[MAX_OBJ_EVENTS]; ///< Event names.
  Box box;                    ///< Box for sprite boundary & collisions.
  Position pos;               ///< Position in the game world.
  string type;                ///< User-defined identification.
  string sprite_name;	      ///< Sprite name in ResourceManager.
  Sprite *p_sprite;           ///< Sprite associated with Object.
  bool sprite_center;         ///< True if sprite centered on Object.
  char sprite_transparency;   ///< Sprite transparent character (0 if none).
  int sprite_index;           ///< Current index frame for sprite.
  int sprite_slowdown;        ///< Slowdown rate (1 = no slowdown, 0 = stop).
  int sprite_slowdown_count;  ///< Slowdown counter.
  int altitude;               ///< 0 to MAX_ALTITUDE (lower drawn first).
  Solidness solidness;        ///< Solidness state of Object.
  bool no_soft;               ///< True if won't move on soft Objects.
  float x_velocity;           ///< Horizontal speed in spaces per game step.
  float x_velocity_countdown; ///< Countdown to horizontal movement.
  float y_velocity;           ///< Vertical speed in spaces per game step.
  float y_velocity_countdown; ///< Countdown to vertical movement.
  bool modified[ATTR_COUNT];  ///< Modified attributes since last serialize().

 public:
  Object();

  /// Destroy Object, unregistering for any interested events.
  virtual ~Object();

  /// Set object id.
  void setId(int new_id);

  /// Get object id.
  int getId();
  
  ///< Return True if Object is HARD or SOFT, else False.
  bool isSolid();    

  /// Set Object solidness, with checks for consistency.  
  /// Return 0 if ok, else -1.
  int setSolidness(Solidness new_solid);

  /// Return Object solidness.
  Solidness getSolidness();

  /// Set visibility of Object.  Objects not visible are not drawn.
  /// Return 0 if ok, else -1.
  int setVisible(bool visible);

  /// Return visibility of Object.  Objects not visible are not drawn.
  bool isVisible();

  /// Set activeness of Object.  Objects not active are not acted upon
  /// by engine.
  /// Return 0 if ok, else -1.
  int setActive(bool active);

  /// Return activeness of Object.  Objects not active are not acted upon
  /// by engine.
  bool isActive();
  /// Set Object altitude, with checks for range [0, MAX_ALTITUDE].
  /// Return 0 if ok, else -1.
  int setAltitude(int new_altitude);
   
  /// Return Object altitude.
  int getAltitude();
   
  /// Set x velocity.
  void setXVelocity(float new_x_velocity);

  /// Get x velocity.
  float getXVelocity();

  /// Set y velocity.
  void setYVelocity(float new_y_velocity);

  /// Get y velocity.
  float getYVelocity();

  /// Perform 1 step of velocity in horizontal direction.
  /// Return horizontal distance moved this step.
  int getXVelocityStep();

  /// Perform 1 step of velocity in vertical direction.
  /// Return vertical distance moved this step.
  int getYVelocityStep();

  /// Get "allow soft" setting.
  bool getNoSoft();

  /// Set "allow soft" setting.
  void setNoSoft(bool new_no_soft);

  /// Set bounding box.
  void setBox(Box new_box);

  /// Get bounding box.
  Box getBox();

  /// Set type identifier of Object.
  void setType(string new_type);

  /// Get type identifier of Object.
  string getType();

  /// Set Position of Object.
  void setPosition(Position new_pos);

  /// Get Position of Object.
  Position getPosition();

  /// Handle event (default is to ignore everything).
  /// Return 0 if ignored, else 1.
  virtual int eventHandler(Event *p_event);

  /// Register for interest in event.  
  /// Keeps track of manager and event.  
  /// Return 0 if ok, else -1.
  int registerInterest(string event_type);

  /// Unregister for interest in event.  
  /// Return 0 if ok, else -1.
  int unregisterInterest(string event_type);

  /// Draw single Sprite Frame.
  /// Drawing accounts for: centering, slowdown, advancing Sprite Frame.
  virtual void draw();

  /// Set Object Sprite name (in ResourceManager).
  void setSpriteName(string new_name);

  /// Get Object Sprite name (in ResourceManager).
  string getSpriteName();

  /// Set Object Sprite to new one.
  /// If set_box is true, set bounding box to size of Sprite.
  /// Set sprite index to 0 (first frame).
  void setSprite(Sprite *p_new_sprite, bool set_box=true);

  /// Return pointer to Sprite associated with this Object.
  Sprite *getSprite();

  /// Indicates if sprite is centered at Object position (pos).
  bool isCentered();

  /// Set Sprite to be centered at Object position (pos).
  void setCentered(bool centered);

  /// Get Sprite transparency character (0 means none).
  char getTransparency();

  /// Set Sprite transparency character (0 means none).
  void setTransparency(char transparent=' ');

  /// Return index of current Sprite frame to be displayed.
  int getSpriteIndex();

  /// Set index of current Sprite frame to be displayed.
  void setSpriteIndex(int new_sprite_index);

  /// Slows down sprite animations.
  /// Slowdown value in multiples of WorldManager frame time.
  void setSpriteSlowdown(int new_sprite_slowdown);
  int getSpriteSlowdown();
  void setSpriteSlowdownCount(int new_sprite_slowdown_count);
  int getSpriteSlowdownCount();
  
  /// Serialize Object attributes to single string.
  /// e.g., "id:110,is_active:true, ...
  /// Only modified attributes are serialized (unless all is true).
  /// Clear modified[] array.
  virtual string serialize(bool all = false);

  /// Deserialize string to become Object attributes.
  /// Return 0 if no errors, else -1.  
  virtual int deserialize(string s);

  /// Return true if attribute modified since last serialize.
  virtual bool isModified(enum ObjectAttribute attribute);
  
  /// Return true if any attribute modified since last serialize.
  virtual bool isModified();
};

#endif // __OBJECT_H__
