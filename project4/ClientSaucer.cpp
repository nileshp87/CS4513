//
//ClientSaucer.cpp
//

#include <stdlib.h>		//forrandom()

//engineincludes
#include "EventCollision.h"
#include "EventNuke.h"
#include "EventView.h"
#include "EventOut.h"
#include "LogManager.h"
#include "ResourceManager.h"
#include "WorldManager.h"
#include "Position.h"
//gameincludes
#include "Explosion.h"
#include "Points.h"
#include "ClientSaucer.h"
#include "EventNetwork.h"

ClientSaucer::ClientSaucer(int x,int y){
LogManager &log_manager=LogManager::getInstance();
WorldManager &world_manager=WorldManager::getInstance();
ResourceManager &resource_manager=ResourceManager::getInstance();
Position temp_pos;
temp_pos.setXY(x,y);
//setup"saucer"sprite
Sprite *p_temp_sprite=resource_manager.getSprite("saucer");
if(!p_temp_sprite){
log_manager.writeLog("ClientSaucer::ClientSaucer():Warning!Sprite'%s'notfound",
			"saucer");
}else{
setSprite(p_temp_sprite);
setSpriteSlowdown(4);		
}

//setobjecttype
setType("ClientSaucer");

//setspeedinverticaldirection
setXVelocity(-0.25);		//1spaceevery4frames

world_manager.moveObject(this,temp_pos);

//registerinterestin"nuke"event
registerInterest(NUKE_EVENT);
registerInterest(OUT_EVENT);
}

//handleevent
//return0ifignored,else1
int ClientSaucer::eventHandler(Event *p_e){

if(p_e->getType()==OUT_EVENT){
out();
return 1;
}

if(p_e->getType()==COLLISION_EVENT){
EventCollision *p_collision_event= static_cast <EventCollision *> (p_e);
hit(p_collision_event);
return 1;
}

if(p_e->getType()==NUKE_EVENT){

//createanexplosion
Explosion *p_explosion=new Explosion;
p_explosion->setPosition(this->getPosition());

//deleteself
WorldManager&world_manager=WorldManager::getInstance();
world_manager.markForDelete(this);
}

//ifwegethere,wehaveignoredthisevent
return 0;
}

//ifmovedoffleftedge,movebacktofarright
void ClientSaucer::out(){

WorldManager &world_manager=WorldManager::getInstance();
world_manager.markForDelete(this);
}

//ifsaucerandplayercollide,markbothfordeletion
void ClientSaucer::hit(EventCollision*p_c){
//ifClientSauceronSaucer,ignore

if((p_c->getObject1()->getType()=="ClientSaucer")&&
(p_c->getObject2()->getType()=="ClientSaucer"))
return;

//ifBullet
if((p_c->getObject1()->getType()=="Bullet")||
(p_c->getObject2()->getType()=="Bullet")){

//Createanexplosion
Explosion *p_explosion=new Explosion;
p_explosion->setPosition(this->getPosition());

//ClientSaucersappearstayaroundperpetually
}

//ifhero,markbothobjectsfordestruction
if(((p_c->getObject1()->getType())=="Hero")||
((p_c->getObject2()->getType())=="Hero")){
WorldManager&world_manager=WorldManager::getInstance();
world_manager.markForDelete(p_c->getObject1());
world_manager.markForDelete(p_c->getObject2());
}
}
