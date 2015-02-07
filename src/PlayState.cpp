#include "PlayState.h"
#include "PauseState.h"
#include "CollisionableObject.h"
#include <math.h> /*fabs*/
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

using namespace Ogre;

template<> PlayState* Ogre::Singleton<PlayState>::msSingleton = 0;

void PlayState::enter ()
{
	initSDL ();
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
	Ogre::Entity* ent1 = _sceneMgr->createEntity("playerPaddle", "block.mesh");
	ent1->setQueryFlags(PLAYER);
	_paddle = _sceneMgr->createSceneNode("playerPaddle");
	_paddle->attachObject(ent1);
	_sceneMgr->getRootSceneNode()->addChild(_paddle);
	_paddle->setScale(4,1,1.2);
	_paddle->setPosition(0,-35,-40);
	//playPaddle = new Paddle (_paddle);
	
	//Ball initialization
	ent1 = _sceneMgr->createEntity("ball", "Cube.001.mesh");
	_ball = _sceneMgr->createSceneNode("ball");
	_ball->attachObject(ent1);
	_sceneMgr->getRootSceneNode()->addChild(_ball);
	_ball->setScale(1,1,1);
	_ball->setPosition(0,-26,-40);
	playBall = new Ball(_ball);
	
	//Record Manager
	_recordMgr = new RecordManager();

	//Block Manager initialization
	_blockMgr = new BlockContainer(_sceneMgr, _recordMgr, playBall);
	//_blockMgr->createBlock(0,-15);
	levelGenerator ();

	
	//Sound Managers
	_pTrackManager = new TrackManager;
  	_pSoundFXManager = new SoundFXManager;
  	(_pTrackManager->load("lightintro.ogg"))->play();

  	//_mainTrack->play();

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
		(_pSoundFXManager->load("all.wav"))->play();
		playBall->rightCollisionWall();
	}else if (_xBall < XLEFTWALL){
		(_pSoundFXManager->load("all.wav"))->play();
		playBall->leftCollisionWall();
	}
	
	//Top Collision
	if(_yMaxBall > TOP){
		(_pSoundFXManager->load("all.wav"))->play();
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

void PlayState::levelGenerator (){
	int posx = -18;
	int posy = -8;
	int Hard1, Hard2, Hard3 = 0;
	int temp = -1;
	srand(time(NULL));

	switch (rand()%6){
	case 0:
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j<10; j++) {
				Hard1 = rand()%5 + 1;	
				_blockMgr->createBlock(posx,posy, Hard1);
				posx = posx + 4;
			}
			posx = -18;
			posy = posy - 3;
		}
		break;
	case 1:
		for (int i = 0; i < 5; i++) {
			while{			
				Hard1 = rand()%5 + 1;	
			}do(Hard1 =! temp);
			for (int j = 0; j<10; j++) {
				_blockMgr->createBlock(posx,posy, Hard1);
				posx = posx + 4;
			}
			posx = -18;
			posy = posy - 3;
			temp = Hard1;
		}
		break;
	case 2:
		for (int i = 0; i <10; i++) {
			while{			
				Hard1 = rand()%5 + 1;	
			}do(Hard1 =! temp);
			for (int j = 0; j<5; j++) {
				_blockMgr->createBlock(posx,posy, Hard1);
				posy = posy - 3;
			}
			posy = -8;
			posx = posx + 4;
			temp =! Hard1;		
		}
		break;
	case 3:	
		Hard1 = rand()%5 + 1;
		Hard2 = rand()%5 + 1;
		Hard3 = rand()%5 + 1;
		//Fila 1			
		for (int j = 0; j<10; j++) {
			_blockMgr->createBlock(posx,posy, Hard1);
			posx = posx + 4;
		}
		posx = -18;
		posy = posy - 3;
		//Fila 2
		for (int j = 0; j<10; j++) {
			_blockMgr->createBlock(posx,posy, Hard2);
			posx = posx + 4;
		}
		posx = -18;
		posy = posy - 3;
		//Fila 3
		for (int j = 0; j<10; j++) {
			_blockMgr->createBlock(posx,posy, Hard3);
			posx = posx + 4;
		}
		posx = -18;
		posy = posy - 3;
		//Fila 4	
		for (int j = 0; j<10; j++) {
			_blockMgr->createBlock(posx,posy, Hard2);
			posx = posx + 4;
		}
		posx = -18;
		posy = posy - 3;
		//Fila 5	
		for (int j = 0; j<10; j++) {
			_blockMgr->createBlock(posx,posy, Hard1);
			posx = posx + 4;
		}
		posx = -18;
		posy = posy - 3;
		break;
	case 4:		
		Hard1 = rand()%5 + 1;
		Hard2 = rand()%5 + 1;
		Hard3 = rand()%5 + 1;
		//Columna 1	
		for (int j = 0; j<5; j++) {
			_blockMgr->createBlock(posx,posy, Hard1);
			posy = posy - 3;
		}
		posy = -8;
		posx = posx + 4;
		//Columna 2	
		for (int j = 0; j<5; j++) {
			_blockMgr->createBlock(posx,posy, Hard2);
			posy = posy - 3;
		}
		posy = -8;
		posx = posx + 4;
		//Columna 3	
		for (int j = 0; j<5; j++) {
			_blockMgr->createBlock(posx,posy, Har3);
			posy = posy - 3;
		}
		posy = -8;
		posx = posx + 4;
		//Columna 4	
		for (int j = 0; j<5; j++) {
			_blockMgr->createBlock(posx,posy, Hard2);
			posy = posy - 3;
		}
		posy = -8;
		posx = posx + 4;
		//Columna 5	
		for (int j = 0; j<5; j++) {
			_blockMgr->createBlock(posx,posy, Hard1);
			posy = posy - 3;
		}
		posy = -8;
		posx = posx + 4;
		//Columna 6	
		for (int j = 0; j<5; j++) {
			_blockMgr->createBlock(posx,posy, Hard1);
			posy = posy - 3;
		}
		posy = -8;
		posx = posx + 4;
		//Columna 7	
		for (int j = 0; j<5; j++) {
			_blockMgr->createBlock(posx,posy, Hard2);
			posy = posy - 3;
		}
		posy = -8;
		posx = posx + 4;
		//Columna 8
		for (int j = 0; j<5; j++) {
			_blockMgr->createBlock(posx,posy, Hard3);
			posy = posy - 3;
		}
		posy = -8;
		posx = posx + 4;
		//Columna 9	
		for (int j = 0; j<5; j++) {
			_blockMgr->createBlock(posx,posy, Hard2);
			posy = posy - 3;
		}
		posy = -8;
		posx = posx + 4;
		//Columna 10	
		for (int j = 0; j<5; j++) {
			_blockMgr->createBlock(posx,posy, Hard1);
			posy = posy - 3;
		}
		posy = -8;
		posx = posx + 4;
		break;
	case 5:	
		Hard1 = rand()%5 + 1;
		Hard2 = rand()%5 + 1;
		Hard3 = rand()%5 + 1;
		for (int i = 0; i < 5; i++) {
			
		_blockMgr->createBlock(posx,posy, Hard1);
		posx = posx + 4;
		_blockMgr->createBlock(posx,posy, Hard2);
		posx = posx + 4;
		_blockMgr->createBlock(posx,posy, Hard3);
		posx = posx + 4;
		_blockMgr->createBlock(posx,posy, Hard1);
		posx = posx + 4;
		_blockMgr->createBlock(posx,posy, Hard2);
		posx = posx + 4;
		_blockMgr->createBlock(posx,posy, Hard2);
		posx = posx + 4;
		_blockMgr->createBlock(posx,posy, Hard1);
		posx = posx + 4;
		_blockMgr->createBlock(posx,posy, Hard3);
		posx = posx + 4;
		_blockMgr->createBlock(posx,posy, Hard2);
		posx = posx + 4;
		_blockMgr->createBlock(posx,posy, Hard1);
		posx = posx + 4;

		posx = -18;
		posy = posy - 3;
		temp = Hard1;
		Hard1 = Hard2;
		Hard2 = Hard3;
		Hard3 = temp;
		}
		break;
	}
}

bool PlayState::checkInRange(float participant, float center, float range){
	return (participant > center - range && participant < center + range);
}

bool PlayState::initSDL () {
    // Inicializando SDL...
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
        return false;
    // Llamar a  SDL_Quit al terminar.
    atexit(SDL_Quit);
 
    // Inicializando SDL mixer...
    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT,MIX_DEFAULT_CHANNELS, 4096) < 0)
      return false;
 
    // Llamar a Mix_CloseAudio al terminar.
    atexit(Mix_CloseAudio);
 
    return true;    
}



