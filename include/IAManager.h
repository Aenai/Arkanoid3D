#ifndef IAManager_H
#define IAManager_H

#include <Ogre.h>
#include <OIS/OIS.h>
#include "Ball.h"
#include "GhostBall.h"

#include "GameState.h"

#define PLAYER 1 << 0  // Mascara para el escenario
#define CUBE1 1 << 1  // Mascara para objetos de tipo 1
#define CUBE2 1 << 2  // Mascara para objetos de tipo 2

class IAManager {
	
 public:
	IAManager (Ball* newBall, GhostBall* ghostBall, Ogre::SceneNode* paddle);
	
	void update(const Ogre::FrameEvent& evt);
	void keyReleased (const OIS::KeyEvent &e);


  
  protected:
  	Ball* _playBall;
  	GhostBall* _ghostBall;
  	Ogre::SceneNode* _paddle;
  	bool _controlSpeed;
  	
  	bool activated;
  	
  
  
  
};

#endif
