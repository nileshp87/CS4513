//                                                                              
// A "network" event, generated when a network packet arrives.                  
//                                                                              

#include "Event.h"
#include "EventNetwork.h"

char event[2];
int x;
int y;
EventNetwork::EventNetwork(){
  EventNetwork('\0','\0');
}

// Create object with initial bytes.
EventNetwork::EventNetwork(char target, char action){
  EventNetwork(target, action, 0, 0);
}

EventNetwork::EventNetwork(char target, char action, int xp, int yp){
  setType(NETWORK_EVENT);
  event[0] = target;
  event[1] = action;
  x = xp;
  y = yp;
}

char EventNetwork::getTarget(){
  return event[0];
}

char EventNetwork::getAction(){
  return event[1];
}

int EventNetwork::getX(){
  return x;
}

int EventNetwork::getY(){
  return y;
}