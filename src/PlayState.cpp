#include "PlayState.h"
#include "PauseState.h"

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
	_paddle->setPosition(0,-30,-40); 
	
	//Ball initialization
	ent1 = _sceneMgr->createEntity("ball", "Cube.001.mesh");
	_ball = _sceneMgr->createSceneNode("ball");
	_ball->attachObject(ent1);
	_sceneMgr->getRootSceneNode()->addChild(_ball);
	_ball->setScale(1,1,1);
	_ball->setPosition(0,-25,-40);

	//Inicializar variables
	_DRight = false;
	_DLeft = false;
	   	

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

bool PlayState::frameStarted (const Ogre::FrameEvent& evt)
{
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
		if (_paddle->getPosition().x < 20) vt += Ogre::Vector3(1, 0, 0);
	}
	if (_DLeft){
		if (_paddle->getPosition().x > -20) vt += Ogre::Vector3(-1, 0, 0);
	}

	_paddle->translate(vt);

}
