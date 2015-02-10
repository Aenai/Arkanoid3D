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
	
	void update(const Ogre::FrameEvent& evt, Ogre::SceneNode* blockObjective);
	void keyReleased (const OIS::KeyEvent &e);
	void setBlockObjective(Ogre::SceneNode* node);
	float getPredicted();


  
  protected:
  	Ball* _playBall;
  	GhostBall* _ghostBall;
  	Ogre::SceneNode* _paddle;
  	
  	//Prediction Variables
  	float _predicted; //Where the ball is going to stop
  	float _maxY; //Max Y for the paddle
  	float _xObjective; //Block Objective
  	float _yObjective; //Block Objective
  	
  	bool _controlSpeed;
  	bool activated;
  	
  
  
  
};

#endif
