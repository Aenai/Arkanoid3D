#ifndef CollisionableObject_H
#define CollisionableObject_H

#include <Ogre.h>
#include <OIS/OIS.h>

#include "GameState.h"
#include "Ball.h"

#define PLAYER 1 << 0  // Mascara para el escenario
#define CUBE1 1 << 1  // Mascara para objetos de tipo 1
#define CUBE2 1 << 2  // Mascara para objetos de tipo 2

class CollisionableObject{
	
 public:
  CollisionableObject (Ogre::SceneNode* node, Ball* ball);
  
  void checkCollision();
  void updateVariables();
  
  void topCollision(float xNewSpeed);
  virtual void hasCollided();

 protected:
  Ogre::SceneNode* _node;  
  Ogre::Entity* _entity;
  Ball* playBall;
  
  Ogre::Timer collDetectDelay;

  
  //Limits
  float _xMinLimit;
  float _xMaxLimit;
  float _yMinLimit;
  float _yMaxLimit;
  
};

bool inRange(float participant, float min, float max);
bool inRange(float participant, float participant2, float min, float max);
bool collCheck(float checking, float toHit, float errorRange);

#endif
