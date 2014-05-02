//
// ClientSaucer.cpp.h
//

#include "Object.h"
#include "EventCollision.h"

class ClientSaucer : public Object {

 private:
  void moveToStart();
  void out();
  void hit(EventCollision *p_c);

 public:
  ClientSaucer(int x, int y);
  int eventHandler(Event *p_e);
};

