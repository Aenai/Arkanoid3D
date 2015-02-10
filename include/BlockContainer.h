#ifndef BlockContainer_H
#define BlockContainer_H

#include <Ogre.h>
#include <OIS/OIS.h>

#include "GameState.h"
#include "Block.h"
#include "RecordManager.h"
#include "GhostBall.h"

#define PLAYER 1 << 0  // Mascara para el escenario
#define CUBE1 1 << 1  // Mascara para objetos de tipo 1
#define CUBE2 1 << 2  // Mascara para objetos de tipo 2

class BlockContainer {
	
 public:
  BlockContainer (Ogre::SceneManager* sceneMgr, RecordManager* recordMgr, Ball* playBall, GhostBall* ghostBall);
  
  
  void createBlock (int x, int y, int hard);
  void startMatch ();
  void checkCollision();
  void levelGenerator(int level);
  

  

 protected:

	Ogre::SceneManager* _sceneMgr;
	RecordManager* _recordMgr;
	
	std::vector<Block*>* _blocks;
	Ball* _playBall;
	GhostBall* _ghostBall;
	
	int _numBlock;
	int _currentBlocks;
  
  
};

#endif