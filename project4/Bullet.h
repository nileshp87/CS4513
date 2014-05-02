//
// Bullet.h
//

#include "Object.h"
#include "EventCollision.h"

class Bullet : public Object {

 private:
  void out();
  void hit(EventCollision *p_c);

 public:
  Bullet(Position hero_pos);
  int eventHandler(Event *p_e);
};

