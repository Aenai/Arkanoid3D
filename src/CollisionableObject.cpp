
#include "CollisionableObject.h"




CollisionableObject::CollisionableObject (Ogre::SceneNode* node, Ball* ball, GhostBall* ghostBall){
	
	playBall = ball;
	_ghostBall = ghostBall;
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
	checkCollision(playBall);
	checkCollision(_ghostBall);
}

void CollisionableObject::checkCollision(Ball* ball){
	
	float xBall = ball->getX();
	float yBall = ball->getY();
	float error = 1.5;
	bool collided = false;
	
	//Top Collision
	if(collCheck(ball->getMinY(), _yMaxLimit, error)){

		float _xPaddle = _node->getPosition().x;
		
		if(inRange(ball->getMinX(),ball->getMaxX(), _xMinLimit,_xMaxLimit)){
			topCollision((xBall - _xPaddle) * 2, ball);
			collided = true;
			std::cout << collDetectDelay.getMilliseconds() << std::endl;
		}
	}
	
	//Bottom Collision
	if(collCheck(ball->getMaxY(), _yMinLimit, error)){

		float _xPaddle = _node->getPosition().x;
		
		if(inRange(ball->getMinX(),ball->getMaxX(), _xMinLimit,_xMaxLimit)){
			ball->topCollision();
			collided = true;
		}
	}
	
	//Right Collision
	if(collCheck(ball->getMinX(),_xMaxLimit, error)){
		
		if(inRange(ball->getMinY(),ball->getMaxY(), _yMinLimit,_yMaxLimit)){
			ball->leftCollisionWall();
			collided = true;
		}
	}
	
	//Left Collision
	if(collCheck(ball->getMaxX(),_xMinLimit, error)){
		
		if(inRange(ball->getMinY(),ball->getMaxY(), _yMinLimit,_yMaxLimit)){
			ball->rightCollisionWall();
			collided = true;
		}
	}
	
	if(collided && collDetectDelay.getMilliseconds() > 100){
		collDetectDelay = Ogre::Timer();
		if(!ball->getGhost())
			hasCollided();
	}
	
	
	
}

void CollisionableObject::topCollision(float xNewSpeed, Ball* ball){
//	if(!(_node->getName().compare("playerPaddle") && ball->getGhost())){
		ball->collisionPaddle(xNewSpeed);
//	}
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

const Ogre::Vector3& CollisionableObject::getPosition(){
	return _node->getPosition();
}

Ogre::SceneNode* CollisionableObject::getNode(){
	return _node;
}


