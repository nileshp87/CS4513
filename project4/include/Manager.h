///
/// The base manager
///

#ifndef __MANAGER_H__
#define __MANAGER_H__

#include "Event.h"
#include "Object.h"
#include "ObjectList.h"

#define MAX_EVENTS 100		     ///< Max number of different events.

class Manager {

 private:
  int event_count;                   ///< Number of events.
  string event_name[MAX_EVENTS];     ///< List of events.
  ObjectList obj_list[MAX_EVENTS];   ///< Objects interested in event.
  bool is_started;                   ///< True if startUp() succeeded
 
  /// Check if event is handled by this Manager.
  /// If handled, return true else false.
  /// (Base Manager always returns false.)
  virtual bool isValid(string event_type);

 public:
  Manager();
  virtual ~Manager();

  /// Startup Manager. 
  /// Return 0 if ok, else negative number.
  virtual int startUp();

  /// Shutdown Manager.  
  virtual void shutDown();

  /// Return status of is_started (true when startUp() was successful).
  bool isStarted();

  /// Send event to all interested objects.
  /// Return count of number of events sent.
  int onEvent(Event *p_event);
	 
  /// Indicate interest in event.
  /// Return 0 if ok, else -1.
  /// (Note, doesn't check to see if Object is already registered.)
  int registerInterest(Object *p_o, string event_type);

  /// Indicate no more interest in event. 
  /// Return 0 if ok, else -1.
  int unregisterInterest(Object *p_o, string event_type);

};

#endif /// __MANAGER_H__
