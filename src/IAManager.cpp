#include "IAManager.h"

IAManager::IAManager (Ball* newBall, GhostBall* ghostBall, Ogre::SceneNode* paddle){
	
	_playBall = newBall;
	_ghostBall = ghostBall;
	_paddle = paddle;
	_controlSpeed = false;
	//FIXME temporary
	activated = true;
	
}


void IAManager::update(const Ogre::FrameEvent& evt){
	
	//Basic IA
//	if(activated){
//		_paddle->setPosition(_playBall->getX(), _paddle->getPosition().y, _paddle->getPosition().z);
//	}

	if(_playBall->getYSpeed() > 0 && _controlSpeed){
		_controlSpeed=false;
		_ghostBall->setSpeed(_playBall->getXSpeed()*2, _playBall->getYSpeed()*2);
		_ghostBall->setPosition(_playBall->getPosition());
	}
	if(_playBall->getYSpeed() < 0){
		_controlSpeed=true;
	}
	

	//_ghostBall->update(evt);
	
}
