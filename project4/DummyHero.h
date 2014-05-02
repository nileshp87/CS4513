//
// DummyHero.h
//

#include "Object.h"
#include "EventNetwork.h"

class DummyHero : public Object {

 private:
  int fire_slowdown;
  int fire_countdown;
  int nuke_count;
  void net(EventNetwork *network_event);
  void move(int dy);
  void fire();
  void step();
  void nuke();

 public:
  DummyHero();
  ~DummyHero();
  int eventHandler(Event *p_e);
};

