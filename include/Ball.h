
#ifndef Ball_H
#define Ball_H

#include <Ogre.h>
#include <OIS/OIS.h>

#include "GameState.h"

#define PLAYER 1 << 0  // Mascara para el escenario
#define CUBE1 1 << 1  // Mascara para objetos de tipo 1
#define CUBE2 1 << 2  // Mascara para objetos de tipo 2

class Ball {
	
 public:
  Ball (Ogre::SceneNode* newBall);
  
  //Game Logic
  void update(const Ogre::FrameEvent& evt);
  void startMatch();
  
  //Collisions
  void collisionPaddle(float newSpeedX);
  void rightCollisionWall();
  void leftCollisionWall();
  void topCollision();
  void bottomCollision();
  
  //Getters
  Ogre::Entity* getEntity();
  float getX();
  float getY();
  float getMinX();
  float getMinY();
  float getMaxX();
  float getMaxY();

 protected:
  Ogre::SceneNode* _ball;   	
 	
  //Speed
  float _xSpeed;
  float _ySpeed;
  
  //Ball Limits
  float _bXMinLimit;
  float _bXMaxLimit;
  float _bYMinLimit;
  float _bYMaxLimit;
  
  
  
};

#endif
