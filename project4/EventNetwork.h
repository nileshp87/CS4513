//                                                                              
// A "network" event, generated when a network packet arrives.                  
//                                                                              

#ifndef __EVENT_NETWORK_H__
#define __EVENT_NETWORK_H__

#include "Event.h"
  
#define NETWORK_EVENT "__network__"

class EventNetwork : public Event {
  
 private:
  int bytes;                    // number of bytes available                    
  
 public:
  // Default constructor.                                                       
  EventNetwork();

  // Create object with initial bytes.
  EventNetwork(int initial_bytes);

  // Set number of bytes available.                                             
  void setBytes(int new_bytes);

  // Get number of bytes available.                                             
  int getBytes();
};

#endif // __EVENT_NETWORK_H__     