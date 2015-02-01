#include "PlayState.h"
#include "PauseState.h"
#include "CollisionableObject.h"
#include <math.h> /*fabs*/

using namespace Ogre;

template<> PlayState* Ogre::Singleton<PlayState>::msSingleton = 0;

void PlayState::enter ()
{
	_root = Ogre::Root::getSingletonPtr();

	// Se recupera el gestor de escena y la cámara.
	_sceneMgr = _root->getSceneManager("SceneManager");
	_camera = _sceneMgr->getCamera("IntroCamera");
	_viewport = _root->getAutoCreatedWindow()->addViewport(_camera);
	
	//Camera configuration
	_camera->lookAt(Vector3(0, -15, -30));
	_camera->setNearClipDistance(0.1);
	_camera->setFarClipDistance(100);
	
	// Nuevo background colour.
	_viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 1.0));

	//Paddle Initialization
	Ogre::Entity* ent1 = _sceneMgr->createEntity("playerPaddle", "cube.mesh");
	ent1->setQueryFlags(PLAYER);
	_paddle = _sceneMgr->createSceneNode("playerPaddle");
	_paddle->attachObject(ent1);
	_sceneMgr->getRootSceneNode()->addChild(_paddle);
	_paddle->setScale(4,1,1.2);
	_paddle->setPosition(0,-35,-40); 
	
	//Ball initialization
	ent1 = _sceneMgr->createEntity("ball", "Cube.001.mesh");
	_ball = _sceneMgr->createSceneNode("ball");
	_ball->attachObject(ent1);
	_sceneMgr->getRootSceneNode()->addChild(_ball);
	_ball->setScale(1,1,1);
	_ball->setPosition(0,-20,-40);
	playBall = new Ball(_ball);
	
	//Record Manager
	_recordMgr = new RecordManager();

	//Block Manager initialization
	_blockMgr = new BlockContainer(_sceneMgr, _recordMgr, playBall);
	_blockMgr->createBlock(0,-15);
	


	//Inicializar variables
	_DRight = false;
	_DLeft = false;
	
	_yCollisionCheck = -100;
	_yMinBall = 100;
	updateVariables();
	playBall->startMatch(); //FIXME temporary call method
	
	_exitGame = false;
}

void PlayState::exit ()
{
	_sceneMgr->clearScene();
	_root->getAutoCreatedWindow()->removeAllViewports();
}

void PlayState::pause()
{
	_DRight = false;
	_DLeft = false;
}

void PlayState::resume()
{
	// Se restaura el background colour.
	_viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 1.0));
}


//================Frame Control====================

bool PlayState::frameStarted (const Ogre::FrameEvent& evt)
{
	playBall->update(evt); //Ball Movement Logic
	updateVariables();
	_blockMgr->checkCollision(); //All blocks colliding logic with Ball
	
	float _xBall = _ball->getPosition().x;
	
	//FIXME temporary until class PADDLE is finished
	CollisionableObject* obj = new CollisionableObject(_paddle, playBall);
	obj->updateVariables();
	obj->checkCollision();
	
	
	
	
	//Wall Collision
	if(_xBall > XRIGHTWALL){
		playBall->rightCollisionWall();
	}else if (_xBall < XLEFTWALL){
		playBall->leftCollisionWall();
	}
	
	//Top Collision
	if(_yMaxBall > TOP){
		playBall->topCollision();
	}
	
	
	return true;
}

bool PlayState::frameEnded (const Ogre::FrameEvent& evt)
{
	if (_exitGame)
		return false;
	else
		paddleMove();
		

	return true;
}

void PlayState::keyPressed (const OIS::KeyEvent &e)
{
	// Tecla p --> PauseState.
	if (e.key == OIS::KC_P) {
		pushState(PauseState::getSingletonPtr());
	}

	if (e.key == OIS::KC_RIGHT or e.key == OIS::KC_D){
		//_sceneMgr->getSceneNode("playerPaddle")->translate(Vector3(1, 0, 0));
		_DRight = true;
	}
	if (e.key == OIS::KC_LEFT or e.key == OIS::KC_A){
		//_sceneMgr->getSceneNode("playerPaddle")->translate(Vector3(-1, 0, 0));
		_DLeft = true;
	}
	
	if(e.key == OIS::KC_X){
	  	/*Ogre::Entity* boardEntity = static_cast<Ogre::Entity*>(_paddle->getAttachedObject(0));
	  	
		Ogre::AxisAlignedBox charAABB = boardEntity->getWorldBoundingBox();
		Ogre::Vector3 min = charAABB.getMinimum();
		Ogre::Vector3 max = charAABB.getMaximum();
		//Ogre::Vector3 center = charAABB.getCenter();
		Ogre::Vector3 size( fabs( max.x - min.x), fabs( max.y - min.y), fabs( max.z - min.z ) );
		std::cout << size[0] << " " << size[1] << " "<< size[2] <<std::endl;
		
		playBall->bottomCollision();*/
		_recordMgr->keepRecord(_recordMgr->getPoints());
		_recordMgr->getRecords();
	  }
}

void PlayState::keyReleased (const OIS::KeyEvent &e)
{
	if (e.key == OIS::KC_ESCAPE) {
		_exitGame = true;
	}

	if (e.key == OIS::KC_RIGHT or e.key == OIS::KC_D){
		_DRight = false;
	}
	if (e.key == OIS::KC_LEFT or e.key == OIS::KC_A){
		_DLeft = false;
	}
}

void PlayState::mouseMoved (const OIS::MouseEvent &e)
{
	Real posx = e.state.X.rel;
	Ogre::Vector3 vt(0, 0, 0);
	if (posx > 0){
		if (_paddle->getPosition().x < XRIGHTWALL) vt += Ogre::Vector3(1, 0, 0);
		//_DRight = true;
		//_DLeft = false;
	}
	if (posx < 0){
		if (_paddle->getPosition().x > XLEFTWALL) vt += Ogre::Vector3(-1, 0, 0);
		//_DRight = false;
		//_DLeft = true;
	}
	if (posx == 0){
		_DRight = false;
		_DLeft = false;	
	}
	_paddle->translate(vt);

}

void PlayState::mousePressed (const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
}

void PlayState::mouseReleased (const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
}

PlayState* PlayState::getSingletonPtr ()
{
	return msSingleton;
}

PlayState& PlayState::getSingleton ()
{ 
	assert(msSingleton);
	return *msSingleton;
}

void PlayState::paddleMove ()
{
	Ogre::Vector3 vt(0, 0, 0);
	
	
	if (_DRight){
		if (_paddle->getPosition().x < XRIGHTWALL) vt += Ogre::Vector3(1, 0, 0);
	}
	if (_DLeft){
		if (_paddle->getPosition().x > XLEFTWALL) vt += Ogre::Vector3(-1, 0, 0);
	}

	_paddle->translate(vt);

}

void PlayState::updateVariables(){
	//Paddle Max
  	Ogre::Entity* boardEntity = static_cast<Ogre::Entity*>(_paddle->getAttachedObject(0));
	Ogre::AxisAlignedBox charAABB = boardEntity->getWorldBoundingBox();
	Ogre::Vector3 min = charAABB.getMinimum();
	Ogre::Vector3 max = charAABB.getMaximum();
	Ogre::Vector3 size( fabs( max.x - min.x), fabs( max.y - min.y), fabs( max.z - min.z ) );
	_yCollisionCheck = max.y;
	_paddleHalfWidth = size[0] / 2;
	
	//Ball Min
	boardEntity = static_cast<Ogre::Entity*>(_ball->getAttachedObject(0));
	charAABB = boardEntity->getWorldBoundingBox();
	min = charAABB.getMinimum();
	max = charAABB.getMaximum();
	_yMinBall = min.y;
	_yMaxBall = max.y;
	
	//std::cout << "_yColl: " << _yCollisionCheck << " _yMin: " << _yMinBall << std::endl;
}

bool PlayState::checkInRange(float participant, float center, float range){
	return (participant > center - range && participant < center + range);
}



