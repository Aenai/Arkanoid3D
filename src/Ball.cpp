
#include "Ball.h"


Ball::Ball (Ogre::SceneNode* newBall){
	
	_ball = newBall;
	_xSpeed = 0;
  	_ySpeed = 0;
}

void Ball::update(const Ogre::FrameEvent& evt){
	
	//Delta Time
	float _timeSinceLastFrame = evt.timeSinceLastFrame;
	
	//Applying translation based on speed
	Ogre::Vector3 vt(_xSpeed * _timeSinceLastFrame , _ySpeed * _timeSinceLastFrame, 0);
	_ball->translate(vt);
}

void Ball::startMatch(){
	
	_ySpeed=-3;
	
}

void Ball::collisionPaddle(float newSpeedX){
	
	if(_ySpeed < 0){
		_ySpeed = -_ySpeed;
		_xSpeed = newSpeedX;
	}
}
