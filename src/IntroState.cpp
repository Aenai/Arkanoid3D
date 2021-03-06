#include "IntroState.h"
#include "PlayState.h"

template<> IntroState* Ogre::Singleton<IntroState>::msSingleton = 0;

void IntroState::enter ()
{
	_root = Ogre::Root::getSingletonPtr();

	_sceneMgr = _root->createSceneManager(Ogre::ST_GENERIC, "SceneManager");
	_camera = _sceneMgr->createCamera("IntroCamera");
	_viewport = _root->getAutoCreatedWindow()->addViewport(_camera);
	_viewport->setBackgroundColour(Ogre::ColourValue(1.0, 1.0, 1.0));

	//Bottom initialization
	Ogre::Entity* ent2 = _sceneMgr->createEntity("bottom", "cube.mesh");
  	ent2->setMaterialName("bottom");
  	Ogre::SceneNode* bottom = _sceneMgr->createSceneNode("bottom");
	bottom->attachObject(ent2);
	_sceneMgr->getRootSceneNode()->addChild(bottom);
	bottom->setScale(60,60,1);
	bottom->setPosition(0,-15,-55);

	renderer = &CEGUI::OgreRenderer::bootstrapSystem();
	createGUI();
	_initGameControl = true;
	initMenu();

	_exitGame = false;
}

void IntroState::exit()
{
	_sceneMgr->clearScene();
	_root->getAutoCreatedWindow()->removeAllViewports();
}

void IntroState::pause ()
{
}

void IntroState::resume ()
{
}

bool IntroState::frameStarted (const Ogre::FrameEvent& evt) 
{
	_timeSinceLastFrame = evt.timeSinceLastFrame;
	CEGUI::System::getSingleton().injectTimePulse(_timeSinceLastFrame);

	
	return true;
}

bool IntroState::frameEnded (const Ogre::FrameEvent& evt)
{
	if (_exitGame)
		return false;

	return true;
}

void IntroState::keyPressed (const OIS::KeyEvent &e)
{
	CEGUI::System::getSingleton().injectKeyDown(e.key);
	CEGUI::System::getSingleton().injectChar(e.text);

	// Transición al siguiente estado.
	// Espacio --> PlayState
	if (e.key == OIS::KC_SPACE) {
		if (_initGameControl == true){
			CEGUI::MouseCursor::getSingleton().hide( );
			//CEGUI::WindowManager::getSingletonPtr()->destroyAllWindows();
			CEGUI::WindowManager::getSingletonPtr()->destroyWindow("MenuWin");
			PlayState::getSingletonPtr()->newGame(true);
			changeState(PlayState::getSingletonPtr());
		}
	}
}

void IntroState::keyReleased (const OIS::KeyEvent &e )
{
	CEGUI::System::getSingleton().injectKeyUp(e.key);

	if (e.key == OIS::KC_ESCAPE) {
		_exitGame = true;
	}
}

void IntroState::mouseMoved (const OIS::MouseEvent &e)
{
	CEGUI::System::getSingleton().injectMouseMove(e.state.X.rel, e.state.Y.rel);
}

void IntroState::mousePressed (const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
	CEGUI::System::getSingleton().injectMouseButtonDown(convertMouseButton(id));
}

void IntroState::mouseReleased (const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
	CEGUI::System::getSingleton().injectMouseButtonUp(convertMouseButton(id));
}

CEGUI::MouseButton IntroState::convertMouseButton(OIS::MouseButtonID id)
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

IntroState* IntroState::getSingletonPtr ()
{
	return msSingleton;
}

IntroState& IntroState::getSingleton ()
{ 
	assert(msSingleton);
	return *msSingleton;
}

void IntroState::createGUI(){
	//CEGUI

	//renderer = &CEGUI::OgreRenderer::bootstrapSystem();
	CEGUI::Scheme::setDefaultResourceGroup("Schemes");
	CEGUI::Imageset::setDefaultResourceGroup("Imagesets");
	CEGUI::Font::setDefaultResourceGroup("Fonts");
	CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");

	CEGUI::SchemeManager::getSingleton().create("ArkaGraf.scheme");
	CEGUI::ImagesetManager::getSingleton().create("ArkaGraf.imageset");
	//CEGUI::System::getSingleton().setDefaultFont("DejaVuSans-10");
	if(! CEGUI::FontManager::getSingleton().isDefined( "DejaVuSans-10" ) )
	CEGUI::FontManager::getSingleton().createFreeTypeFont( "DejaVuSans-10", 10, true, "DejaVuSans.ttf", "Fonts" );
	CEGUI::System::getSingleton().setDefaultFont( "DejaVuSans-10" );
	CEGUI::System::getSingleton().setDefaultMouseCursor("ArkaGraf","MouseArrow");
	CEGUI::MouseCursor::getSingleton().show( );
}

void IntroState::initMenu(){
	//Sheet
	CEGUI::Window* sheet = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow","MenuWin");

	//Config Window
	CEGUI::Window* formatWin = CEGUI::WindowManager::getSingleton().loadWindowLayout("MenuInit.layout");

	//Game Window
	CEGUI::Window* gameButton = CEGUI::WindowManager::getSingleton().getWindow("FormatWin/GameButton");
	gameButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&IntroState::initGame, this));

	//Load Window
	CEGUI::Window* loadButton = CEGUI::WindowManager::getSingleton().getWindow("FormatWin/LoadButton");
	loadButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&IntroState::load, this));

	//Option Window
	CEGUI::Window* optionButton = CEGUI::WindowManager::getSingleton().getWindow("FormatWin/OptionButton");
	optionButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&IntroState::option, this));

	//Exit Window
	CEGUI::Window* exitButton = CEGUI::WindowManager::getSingleton().getWindow("FormatWin/ExitButton");
	exitButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&IntroState::quit, this));
	
	//Attaching buttons
	sheet->addChildWindow(formatWin);
	CEGUI::System::getSingleton().setGUISheet(sheet);
}

void IntroState::loadMenu(){
	//Sheet
	CEGUI::Window* sheet = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow","LoadWin");

	//Config Window
	CEGUI::Window* formatWin = CEGUI::WindowManager::getSingleton().loadWindowLayout("MenuLoad.layout");



	//Back Window
	CEGUI::Window* backButton = CEGUI::WindowManager::getSingleton().getWindow("FormatWin/BackButton");
	_cMenu = 0;
	backButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&IntroState::back, this));
	
	//Attaching buttons
	sheet->addChildWindow(formatWin);
	CEGUI::System::getSingleton().setGUISheet(sheet);
}


void IntroState::optionMenu(){
	//Sheet
	CEGUI::Window* sheet = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow","OptionWin");

	//Config Window
	CEGUI::Window* formatWin = CEGUI::WindowManager::getSingleton().loadWindowLayout("MenuOption.layout");

	//Command Window
	CEGUI::Window* commandButton = CEGUI::WindowManager::getSingleton().getWindow("FormatWin/CommandButton");
	commandButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&IntroState::commands, this));

	//Credit Window
	CEGUI::Window* creditButton = CEGUI::WindowManager::getSingleton().getWindow("FormatWin/CreditsButton");
	creditButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&IntroState::credits, this));

	//Back Window
	CEGUI::Window* backButton = CEGUI::WindowManager::getSingleton().getWindow("FormatWin/BackButton");
	_cMenu = 0;
	backButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&IntroState::back, this));
	
	//Attaching buttons
	sheet->addChildWindow(formatWin);
	CEGUI::System::getSingleton().setGUISheet(sheet);
}

void IntroState::commandMenu(){
	//Sheet
	CEGUI::Window* sheet = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow","CommandWin");

	//Config Window
	CEGUI::Window* formatWin = CEGUI::WindowManager::getSingleton().loadWindowLayout("MenuCommands.layout");

	//Setting Text!
	CEGUI::WindowManager::getSingleton().getWindow("FormatWin/Text3")->setText(" [vert-alignment='centre'][image-size='w:36 h:36'][image='set:ArkaGraf image:ImgKeyP'] --> Detener el juego");
	CEGUI::WindowManager::getSingleton().getWindow("FormatWin/Text6")->setText(" [vert-alignment='centre'][image-size='w:36 h:36'][image='set:ArkaGraf image:ImgKeyX'] --> Activar la IA");
	CEGUI::WindowManager::getSingleton().getWindow("FormatWin/Text7")->setText(" [vert-alignment='centre'][image-size='w:36 h:36'][image='set:ArkaGraf image:ImgKeyZ'] --> Activar una bola 'fantasma'");
	CEGUI::WindowManager::getSingleton().getWindow("FormatWin/Text2")->setText(" [vert-alignment='centre'][image-size='w:36 h:36'][image='set:ArkaGraf image:ImgKeyEsc'] --> Cerrar el juego");
	CEGUI::WindowManager::getSingleton().getWindow("FormatWin/Text1")->setText(" [vert-alignment='centre'][image-size='w:120 h:36'][image='set:ArkaGraf image:ImgKeySpace'] --> Iniciar juego nuevo");
	CEGUI::WindowManager::getSingleton().getWindow("FormatWin/Text4")->setText(" [vert-alignment='centre'][image-size='w:36 h:36'][image='set:ArkaGraf image:ImgKeyLeft'] o [image-size='w:40 h:40'][image='set:ArkaGraf image:ImgMouseLeft'] --> Moverse a la izquierda");
	CEGUI::WindowManager::getSingleton().getWindow("FormatWin/Text5")->setText(" [vert-alignment='centre'][image-size='w:36 h:36'][image='set:ArkaGraf image:ImgKeyRight'] o [image-size='w:40 h:40'][image='set:ArkaGraf image:ImgMouseRight'] --> Moverse a la derecha");

	//Back Window
	CEGUI::Window* backButton = CEGUI::WindowManager::getSingleton().getWindow("FormatWin/BackButton");
	_cMenu = 1;
	backButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&IntroState::back, this));
	
	//Attaching buttons
	sheet->addChildWindow(formatWin);
	CEGUI::System::getSingleton().setGUISheet(sheet);
}


void IntroState::creditMenu(){
	//Sheet
	CEGUI::Window* sheet = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow","CreditWin");

	//Config Window
	CEGUI::Window* formatWin = CEGUI::WindowManager::getSingleton().loadWindowLayout("MenuCredit.layout");

	//Setting Text!
	CEGUI::WindowManager::getSingleton().getWindow("FormatWin/Text1")->setText("[vert-alignment='centre']Diseñado por:");
	CEGUI::WindowManager::getSingleton().getWindow("FormatWin/Text2")->setText("[vert-alignment='centre']   - Juan Carlos Fernandez Duran");
	CEGUI::WindowManager::getSingleton().getWindow("FormatWin/Text3")->setText("[vert-alignment='centre']   - Ivan Martinez Heras");

	//Back Window
	CEGUI::Window* backButton = CEGUI::WindowManager::getSingleton().getWindow("FormatWin/BackButton");
	_cMenu = 1;
	backButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&IntroState::back, this));
	
	//Attaching buttons
	sheet->addChildWindow(formatWin);
	CEGUI::System::getSingleton().setGUISheet(sheet);
}

bool IntroState::initGame(const CEGUI::EventArgs &e){
	CEGUI::MouseCursor::getSingleton().hide( );
	//CEGUI::WindowManager::getSingletonPtr()->destroyAllWindows();
	CEGUI::WindowManager::getSingletonPtr()->destroyAllWindows();
	PlayState::getSingletonPtr()->newGame(true);
	changeState(PlayState::getSingletonPtr());

	return true;
}

bool IntroState::load(const CEGUI::EventArgs &e){
	_initGameControl=false;
	CEGUI::WindowManager::getSingletonPtr()->destroyWindow("MenuWin");
	CEGUI::MouseCursor::getSingleton().hide( );
        PlayState::getSingletonPtr()->newGame(false);
	changeState(PlayState::getSingletonPtr());
	return true;
}

bool IntroState::option(const CEGUI::EventArgs &e){
	_initGameControl=false;
	CEGUI::WindowManager::getSingletonPtr()->destroyWindow("MenuWin");
	optionMenu();
	return true;
}

bool IntroState::commands(const CEGUI::EventArgs &e){
	_initGameControl=false;
	CEGUI::WindowManager::getSingletonPtr()->destroyWindow("OptionWin");
	commandMenu();
	return true;
}

bool IntroState::credits(const CEGUI::EventArgs &e){
	_initGameControl=false;
	CEGUI::WindowManager::getSingletonPtr()->destroyWindow("OptionWin");
	creditMenu();
	return true;
}

bool IntroState::back(const CEGUI::EventArgs &e){
	_initGameControl=false;
	CEGUI::WindowManager::getSingletonPtr()->destroyAllWindows();

	switch(_cMenu)
	{
		case 0:
			_initGameControl=true;
			initMenu();
			break;
		case 1:
			optionMenu();
			break;
	}
	return true;
}

bool IntroState::quit(const CEGUI::EventArgs &e){
	_exitGame = true;
	return true;
}


