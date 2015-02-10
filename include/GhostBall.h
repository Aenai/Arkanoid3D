#ifndef GhostBall_H
#define GhostBall_H

#include <Ogre.h>
#include <OIS/OIS.h>
#include "Ball.h"

#include "GameState.h"

#define PLAYER 1 << 0  // Mascara para el escenario
#define CUBE1 1 << 1  // Mascara para objetos de tipo 1
#define CUBE2 1 << 2  // Mascara para objetos de tipo 2

class GhostBall : public Ball {
	
 public:
  GhostBall (Ogre::SceneNode* newBall);
  
  
  
  
};

#endif
