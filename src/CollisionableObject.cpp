
#include "CollisionableObject.h"




CollisionableObject::CollisionableObject (Ogre::SceneNode* node, Ball* ball){
	
	playBall = ball;
	_node = node;
	_entity = static_cast<Ogre::Entity*>(_node->getAttachedObject(0));
	collDetectDelay = Ogre::Timer();
	updateVariables();
}


//Should be Called if the node is dynamic
void CollisionableObject::updateVariables(){
	
	Ogre::AxisAlignedBox charAABB = _entity->getWorldBoundingBox();
	Ogre::Vector3 min = charAABB.getMinimum();
	Ogre::Vector3 max = charAABB.getMaximum();
	//Ogre::Vector3 size( fabs( max.x - min.x), fabs( max.y - min.y), fabs( max.z - min.z ) );
	
	_xMinLimit = min.x;
	_xMaxLimit = max.x;
	_yMinLimit = min.y;
	_yMaxLimit = max.y;
}


void CollisionableObject::checkCollision(){
	
	float xBall = playBall->getX();
	float yBall = playBall->getY();
	float error = 0.5;
	bool collided = false;
	
	//Top Collision
	if(collCheck(playBall->getMinY(), _yMaxLimit, error)){

		float _xPaddle = _node->getPosition().x;
		
		if(inRange(playBall->getMinX(),playBall->getMaxX(), _xMinLimit,_xMaxLimit)){
			topCollision((xBall - _xPaddle) * 2);
			collided = true;
			std::cout << collDetectDelay.getMilliseconds() << std::endl;
		}
	}
	
	//Bottom Collision
	if(collCheck(playBall->getMaxY(), _yMinLimit, error)){

		float _xPaddle = _node->getPosition().x;
		
		if(inRange(playBall->getMinX(),playBall->getMaxX(), _xMinLimit,_xMaxLimit)){
			playBall->topCollision();
			collided = true;
		}
	}
	
	//Right Collision
	if(collCheck(playBall->getMinX(),_xMaxLimit, error)){
		
		if(inRange(playBall->getMinY(),playBall->getMaxY(), _yMinLimit,_yMaxLimit)){
			playBall->leftCollisionWall();
			collided = true;
		}
	}
	
	//Left Collision
	if(collCheck(playBall->getMaxX(),_xMinLimit, error)){
		
		if(inRange(playBall->getMinY(),playBall->getMaxY(), _yMinLimit,_yMaxLimit)){
			playBall->rightCollisionWall();
			collided = true;
		}
	}
	
	if(collided && collDetectDelay.getMilliseconds() > 100){
		collDetectDelay = Ogre::Timer();
		hasCollided();
	}
	
	
	
}

void CollisionableObject::topCollision(float xNewSpeed){
	playBall->collisionPaddle(xNewSpeed);
}

void CollisionableObject::hasCollided(){
	std::cout << "Father" << std::endl;
}

bool inRange(float participant, float min, float max){
	return (participant > min && participant < max);
}

bool inRange(float participant, float participant2, float min, float max){
	return (participant > min && participant < max) || (participant2 > min && participant2 < max) || 
	((participant+participant2)/2 > min && (participant+participant2)/2 < max);
}

bool collCheck(float checking, float toHit, float errorRange){
	return (checking <= toHit && fabs(checking - toHit) < errorRange && toHit != 1);
}




