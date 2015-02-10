
#ifndef Block_H
#define Block_H

#include <Ogre.h>
#include <OIS/OIS.h>

#include "GameState.h"
#include "CollisionableObject.h"
#include "RecordManager.h"
#include "GhostBall.h"

#define PLAYER 1 << 0  // Mascara para el escenario
#define CUBE1 1 << 1  // Mascara para objetos de tipo 1
#define CUBE2 1 << 2  // Mascara para objetos de tipo 2

class Block : public CollisionableObject {
	
 public:
  Block (Ogre::SceneNode* newBlock, Ball* ball, GhostBall* ghostBall ,RecordManager* recordMgr , int hard);
  
  void hasCollided();
  bool isDead();

  

 protected:
	
	RecordManager* _recordMgr;
	
	int _lifes;
  
  
};

#endif
