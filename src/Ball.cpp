
#include "Ball.h"


Ball::Ball (Ogre::SceneNode* newBall, bool ghost){
	
	_ball = newBall;
	_xSpeed = 0;
  	_ySpeed = 0;
  	
	_bXMinLimit = 0;
	_bXMaxLimit = 0;
	_bYMinLimit = 0;
	_bYMaxLimit = 0;
	
	_isGhost=ghost;
}

Ball::Ball (Ogre::SceneNode* newBall){
	
	_ball = newBall;
	_xSpeed = 0;
  	_ySpeed = 0;
  	
	_bXMinLimit = 0;
	_bXMaxLimit = 0;
	_bYMinLimit = 0;
	_bYMaxLimit = 0;
	
	
	
	_isGhost=false;
}

void Ball::update(const Ogre::FrameEvent& evt){
	
	//Delta Time
	float _timeSinceLastFrame = evt.timeSinceLastFrame;
	
	//Applying translation based on speed
	Ogre::Vector3 vt(_xSpeed * _timeSinceLastFrame , _ySpeed * _timeSinceLastFrame, 0);
	_ball->translate(vt);
	
	//Getting new info
	Ogre::AxisAlignedBox charAABB = getEntity()->getWorldBoundingBox();
	Ogre::Vector3 min = charAABB.getMinimum();
	Ogre::Vector3 max = charAABB.getMaximum();
	//Ogre::Vector3 size( fabs( max.x - min.x), fabs( max.y - min.y), fabs( max.z - min.z ) );
	
	_bXMinLimit = min.x;
	_bXMaxLimit = max.x;
	_bYMinLimit = min.y;
	_bYMaxLimit = max.y;
}

void Ball::startMatch(int level){
	
	_ySpeed=-10-1*level*0.5;
	_ball->setPosition(0,-26,-40);
	_xSpeed= 1;
	
}

void Ball::collisionPaddle(float newSpeedX){
	
	if(_ySpeed < 0){
		_ySpeed = -_ySpeed;
		_xSpeed = newSpeedX;
	}
}

void Ball::rightCollisionWall(){
	if(_xSpeed > 0){
		_xSpeed = -_xSpeed;
	}
}

void Ball::leftCollisionWall(){
	if(_xSpeed < 0){
		_xSpeed = -_xSpeed;
	}
}

void Ball::topCollision(){
	if(_ySpeed > 0){
		_ySpeed = -_ySpeed;
	}
}

void Ball::bottomCollision(){
	if(_ySpeed < 0){
		_ySpeed = -_ySpeed;
	}
}

Ogre::Entity* Ball::getEntity(){
	return static_cast<Ogre::Entity*>(_ball->getAttachedObject(0));
}

float Ball::getX(){
	return _ball->getPosition().x;
}

float Ball::getY(){
	return _ball->getPosition().y;
}

float Ball::getMinX(){
	return _bXMinLimit;
}

float Ball::getMinY(){
	return _bYMinLimit;
}

float Ball::getMaxX(){
	return _bXMaxLimit;
}

float Ball::getMaxY(){
	return _bYMaxLimit;
}

bool Ball::getGhost(){
	return _isGhost;
}

float Ball::getXSpeed(){
	return _xSpeed;
}

float Ball::getYSpeed(){
	return _ySpeed;
}

void Ball::setSpeed(float x, float y){
	_xSpeed = x;
	_ySpeed = y;
}

Ogre::SceneNode* Ball::getNode(){
	return _ball;
}

void Ball::setPosition(const Ogre::Vector3& pos){
	_ball->setPosition(pos);
}

const Ogre::Vector3& Ball::getPosition(){
	return _ball->getPosition();
}









