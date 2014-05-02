//                                                                              
// A "network" event, generated when a network packet arrives.                  
//                                                                              

#ifndef __EVENT_NETWORK_H__
#define __EVENT_NETWORK_H__

#include "Event.h"
  
#define NETWORK_EVENT "__network__"

class EventNetwork : public Event {
  
 private:
  char event[2];
  int x;
  int y;
 public:
  // Default constructor.                                                       
  EventNetwork();

  // Create object with initial bytes.
  EventNetwork(char target, char action);

  EventNetwork(char target, char action, int x, int y);


  char getTarget();

  char getAction();

  int getX();

  int getY();
};

#endif // __EVENT_NETWORK_H__     