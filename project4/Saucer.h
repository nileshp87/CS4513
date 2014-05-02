//
// Saucer.h
//

#include "Object.h"
#include "EventCollision.h"

class Saucer : public Object {

 private:
  void moveToStart();
  void out();
  void hit(EventCollision *p_c);

 public:
  Saucer();
  ~Saucer();
  int eventHandler(Event *p_e);
};

