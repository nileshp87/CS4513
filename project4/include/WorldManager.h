///
/// The game world manager
///

#ifndef __WORLD_MANAGER_H__
#define __WORLD_MANAGER_H__

#include "Box.h"
#include "Circle.h"
#include "Line.h"
#include "Manager.h"
#include "ObjectList.h"
#include "Position.h"
#include "SceneGraph.h"

#define MAX_ALTITUDE 4          ///< Drawing order 0 to max.

class WorldManager : public Manager {

 private:
  WorldManager();               ///< Private since a singleton.
  WorldManager (WorldManager const&);  ///< Don't allow copy.
  void operator=(WorldManager const&); ///< Don't allow assignment.
  SceneGraph scene_graph;       ///< Storage for all Objects, game and view.
  ObjectList deletions;         ///< List of all Objects to delete.
  Object *p_view_following;     ///< Object view is following.
  Box boundary;                 ///< World boundary.
  Box view;                     ///< Player view of game world.

 public:
  /// Get the one and only instance of the WorldManager.
  static WorldManager &getInstance();
  
  /// Accept all user-defined events (returns true).
  /// Return false for other engine events.
  bool isValid(string event_type);

  /// Startup game world (initialize everything to empty).
  /// Return 0.
  int startUp();

  /// Shutdown game world (delete all game world Objects).
  void shutDown();

  /// Add Object to world.
  /// Return 0 if ok, else -1.
  int insertObject(Object *p_o);

  /// Remove Object from world.
  /// Return 0 if ok, else -1.
  int removeObject(Object *p_o);

  /// Return list of all Objects in world.
  /// If inactive is true, include inactive Objects.
  ObjectList getAllObjects(bool inactive=false);

  /// Indicate Object is to be deleted at end of current game loop.
  /// Return 0 if ok, else -1.
  int markForDelete(Object *p_o);

  /// Update world.
  /// Update positions of Objects based on their velocities.
  /// Lastly, delete Objects marked for deletion.
  void update();

  /// Draw all Objects in view.
  /// Draw bottom up, from -MAX_ALTITUDE to MAX_ALTITUDE.
  void draw();

  /// Move Object. 
  /// If no collision with solid, move ok else don't move Object.
  /// If Object is Spectral, move ok.
  /// If move ok, adjust screen if following this Object.
  /// Return 0 if move ok, else -1 if collision with solid.
  int moveObject(Object *p_o, Position where);

  /// Return list of Objects collided with at Position 'where'.
  /// Collisions only with solid Objects.
  /// Does not consider if p_o is solid or not.
  ObjectList isCollision(Object *p_o, Position where);

  /// Return Object with indicated id.
  /// NULL if Object is not found.
  Object *objectWithId(int id);
  
  /// Return list of all Objects at Position 'where'.  
  /// Does include bounding boxes. Return empty list if none found.
  ObjectList objectsAtPosition(Position where);

  /// Return list of all Objects in Box.
  /// Does include bounding boxes. Return empty list if none found.
  ObjectList objectsInBox(Box box);

  /// Return a list of all Objects on line from point1 to point2.
  /// Does include bounding boxes. Return empty list if none found.
  ObjectList objectsOnLine(Line line);

  /// Return a list of all Objects in circle.
  /// Does include bounding boxes. Return empty list if none found.
  ObjectList objectsInCircle(Circle circle);

  /// Get game world boundary.
  Box getBoundary();

  /// Set game world boundary.
  void setBoundary(Box new_boundary);

  /// Get player view of game world.
  Box getView();

  /// Set player view of game world.
  void setView(Box new_view);

  /// Set view to center screen on Object.
  /// Set to NULL to stop following.
  /// If Object not legit, return -1 else return 0.
  int setViewFollowing(Object *p_new_view_following);

  /// Set view to center screen on Position view_pos.
  /// View edge will not go beyond world boundary.
  void setViewPosition(Position view_pos);

  /// Return reference to SceneGraph.
  SceneGraph &getSceneGraph();
};
 
#endif // __WORLD_MANAGER_H__
