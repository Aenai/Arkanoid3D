#include "PauseState.h"
#include "IntroState.h"

template<> PauseState* Ogre::Singleton<PauseState>::msSingleton = 0;

void PauseState::enter ()
{
	_root = Ogre::Root::getSingletonPtr();

	// Se recupera el gestor de escena y la cámara.
	_sceneMgr = _root->getSceneManager("SceneManager");
	_camera = _sceneMgr->getCamera("IntroCamera");
	_viewport = _root->getAutoCreatedWindow()->getViewport(0);
	// Nuevo background colour.
	_viewport->setBackgroundColour(Ogre::ColourValue(0.0, 1.0, 0.0));
	
	pauseMenu();

	_exitGame = false;
}

void PauseState::exit ()
{
}

void PauseState::pause ()
{
}

void PauseState::resume ()
{
}

bool PauseState::frameStarted (const Ogre::FrameEvent& evt)
{
	return true;
}

bool PauseState::frameEnded (const Ogre::FrameEvent& evt)
{
	if (_exitGame)
		return false;

	return true;
}

void PauseState::keyPressed (const OIS::KeyEvent &e) {
	// Tecla p --> Estado anterior.
	if (e.key == OIS::KC_P) {
		CEGUI::MouseCursor::getSingleton().hide( );
		CEGUI::WindowManager::getSingletonPtr()->destroyWindow("PauseWin");
		popState();
	}
}

void PauseState::keyReleased (const OIS::KeyEvent &e)
{
}

void PauseState::mouseMoved (const OIS::MouseEvent &e)
{
	CEGUI::System::getSingleton().injectMouseMove(e.state.X.rel, e.state.Y.rel);
}

void PauseState::mousePressed (const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
	CEGUI::System::getSingleton().injectMouseButtonDown(convertMouseButton(id));
}

void PauseState::mouseReleased (const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
	CEGUI::System::getSingleton().injectMouseButtonUp(convertMouseButton(id));
}

CEGUI::MouseButton PauseState::convertMouseButton(OIS::MouseButtonID id)
{
	CEGUI::MouseButton ceguiId;
	switch(id)
	{
		case OIS::MB_Left:
			ceguiId = CEGUI::LeftButton;
			break;
		case OIS::MB_Right:
			ceguiId = CEGUI::RightButton;
			break;
		case OIS::MB_Middle:
			ceguiId = CEGUI::MiddleButton;
			break;
		default:
			ceguiId = CEGUI::LeftButton;
	}
	return ceguiId;
}

PauseState* PauseState::getSingletonPtr ()
{
	return msSingleton;
}

PauseState& PauseState::getSingleton ()
{ 
	assert(msSingleton);
	return *msSingleton;
}

void PauseState::pauseMenu(){
	
	CEGUI::MouseCursor::getSingleton().show( );

	//Sheet
	CEGUI::Window* sheet = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow","PauseWin");

	//Config Window
	CEGUI::Window* formatWin = CEGUI::WindowManager::getSingleton().loadWindowLayout("MenuPause.layout");

	//Setting Text!
	CEGUI::WindowManager::getSingleton().getWindow("FormatWin/TextPause")->setText("[vert-alignment='centre']PAUSE");

	//Continue Window
	CEGUI::Window* continueButton = CEGUI::WindowManager::getSingleton().getWindow("FormatWin/ContinueButton");
	continueButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&PauseState::contin, this));

	//Init Window
	CEGUI::Window* initButton = CEGUI::WindowManager::getSingleton().getWindow("FormatWin/InitButton");
	initButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&PauseState::intro, this));

	//Close Window
	CEGUI::Window* closeButton = CEGUI::WindowManager::getSingleton().getWindow("FormatWin/CloseButton");
	closeButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&PauseState::close, this));
	
	//Attaching buttons
	sheet->addChildWindow(formatWin);
	CEGUI::System::getSingleton().setGUISheet(sheet);
}

bool PauseState::contin(const CEGUI::EventArgs &e){
	CEGUI::MouseCursor::getSingleton().hide( );
	CEGUI::WindowManager::getSingletonPtr()->destroyWindow("PauseWin");
	popState();
	return true;
}

bool PauseState::intro(const CEGUI::EventArgs &e){
	//CEGUI::MouseCursor::getSingleton().hide( );
	//CEGUI::WindowManager::getSingletonPtr()->destroyWindow("PauseWin");
	changeState(IntroState::getSingletonPtr());
	return true;
}

bool PauseState::close(const CEGUI::EventArgs &e){
	_exitGame = true;
	return true;
}
