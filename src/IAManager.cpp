#include "IAManager.h"
#include <OgreMath.h>

IAManager::IAManager (Ball* newBall, GhostBall* ghostBall, Ogre::SceneNode* paddle){
	
	_playBall = newBall;
	_ghostBall = ghostBall;
	_paddle = paddle;
	_controlSpeed = false;
	//FIXME temporary
	activated = false;
	_showGhost=false;
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
	
	
	
		_ghostBall->getNode()->setVisible(_showGhost);
	
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
		
	
		float speed = xSpace / time;
		
		if(activated){
			if(_predicted != 0){
				//_paddle->setPosition(_predicted - speed/2 , _paddle->getPosition().y, _paddle->getPosition().z);
				float posx = _predicted - speed/2;
				
				Ogre::AxisAlignedBox charAABB = 
				(static_cast<Ogre::Entity*>(_paddle->getAttachedObject(0)))->getWorldBoundingBox();
				Ogre::Vector3 max = charAABB.getMaximum();
				Ogre::Vector3 min = charAABB.getMinimum();
				float xSize= max.x - min.x;
				
				if(speed > xSize/2)
					speed= xSize/3;
				else if (speed < -xSize/2)
					speed= -xSize/3;
					
				posx = _predicted - speed/2;
				std::cout << speed << " Speed" << std::endl;
					
				float distance = fabs(_paddle->getPosition().x - posx);
				Ogre::Vector3 vt(0, 0, 0);
				if (posx > _paddle->getPosition().x){
					vt += Ogre::Vector3(1 + distance, 0, 0);
					//_DRight = true;
					//_DLeft = false;
				}else
				if (posx < _paddle->getPosition().x){
					vt += Ogre::Vector3(-1 - distance, 0, 0);
					//_DRight = false;
					//_DLeft = true;
				}
				_paddle->translate(vt*0.05);
			}
		}
	}
	
}

void IAManager::setBlockObjective(Ogre::SceneNode* node){
	
	_xObjective = node->getPosition().x; //Block Objective
  	_yObjective = node->getPosition().y; //Block Objective
	
}

float IAManager::getPredicted(){
	return _predicted;
}


void IAManager::keyReleased (const OIS::KeyEvent &e){
	if (e.key == OIS::KC_X){
		activated = !activated;
	}
	if (e.key == OIS::KC_Z){
		_showGhost = !_showGhost;
	}
}




