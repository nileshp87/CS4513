//
// ServerSaucer.cpp.h
//

#include "Object.h"
#include "EventCollision.h"

class ServerSaucer : public Object {

 private:
  void moveToStart();
  void out();
  void hit(EventCollision *p_c);

 public:
  ServerSaucer();
  ~ServerSaucer();
  int eventHandler(Event *p_e);
};

