//
// ClientHero.h
//

#include "Object.h"
#include "EventKeyboard.h"

class ClientHero : public Object {

 private:
  int fire_slowdown;
  int fire_countdown;
  int nuke_count;
  void kbd(EventKeyboard *keyboard_event);
  void move(int dy);
  void fire();
  void step();
  void nuke();

 public:
  ClientHero();
  ~ClientHero();
  int eventHandler(Event *p_e);
};

