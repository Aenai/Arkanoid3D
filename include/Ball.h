
#ifndef Ball_H
#define Ball_H

#include <Ogre.h>
#include <OIS/OIS.h>

#include "GameState.h"

#define PLAYER 1 << 0  // Mascara para el escenario
#define CUBE1 1 << 1  // Mascara para objetos de tipo 1
#define CUBE2 1 << 2  // Mascara para objetos de tipo 2

class Ball{
	
 public:
  Ball (Ogre::SceneNode* newBall);
  
  void update(const Ogre::FrameEvent& evt);
  void startMatch();
  
  void collisionPaddle(float newSpeedX);

 protected:
  Ogre::SceneNode* _ball;   // Nodo de la bola
  
  float _xSpeed;
  float _ySpeed;
  
};

#endif
