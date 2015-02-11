#include "IAManager.h"
#include <OgreMath.h>

IAManager::IAManager (Ball* newBall, GhostBall* ghostBall, Ogre::SceneNode* paddle){
	
	_playBall = newBall;
	_ghostBall = ghostBall;
	_paddle = paddle;
	_controlSpeed = false;
	//FIXME temporary
	activated = true;
	_predicted = 0;
	
	Ogre::AxisAlignedBox charAABB = (static_cast<Ogre::Entity*>(_paddle->getAttachedObject(0)))->getWorldBoundingBox();
	Ogre::Vector3 max = charAABB.getMaximum();
	
	_maxY = paddle->getPosition().y+1;
	
}


void IAManager::update(const Ogre::FrameEvent& evt, Ogre::SceneNode* blockObjective){
	
	if(blockObjective != NULL){
		float ySpeed = _playBall->getYSpeed();
		float ySpace = blockObjective->getPosition().y - _paddle->getPosition().y;
		float time = fabs(ySpeed / ySpace);
	
	
	

	
		float xSpace = blockObjective->getPosition().x - _predicted;


		if(_playBall->getYSpeed() > 0 && _controlSpeed){
			_controlSpeed=false;
			_ghostBall->setSpeed(_playBall->getXSpeed()*2, _playBall->getYSpeed()*2);
			_ghostBall->setPosition(_playBall->getPosition());
		}
		if(_playBall->getYSpeed() < 0){
			_controlSpeed=true;
			_predicted= _ghostBall->getPosition().x;
		}
	
		if(_ghostBall->getY()-2 <= _maxY && _ghostBall->getYSpeed() < 0){
			_ghostBall->setSpeed(0,0);
		}
		
		std::cout << _ghostBall->getY() << " " << _maxY << std::endl;
	
		float speed = xSpace / time;
		if(_predicted != 0)
			_paddle->setPosition(_predicted - speed/2 , _paddle->getPosition().y, _paddle->getPosition().z);
	}
	
}

void IAManager::setBlockObjective(Ogre::SceneNode* node){
	
	_xObjective = node->getPosition().x; //Block Objective
  	_yObjective = node->getPosition().y; //Block Objective
	
}

float IAManager::getPredicted(){
	return _predicted;
}






