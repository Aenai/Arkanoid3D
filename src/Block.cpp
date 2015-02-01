#include "Block.h"



Block::Block (Ogre::SceneNode* newBlock, Ball* ball, RecordManager* recordMgr, int hard):CollisionableObject(newBlock, ball){
	_lifes = hard;
	_recordMgr = recordMgr;
}

void Block::hasCollided(){
//	std::cout << "Alive" << std::endl;
	_recordMgr->addPoints(_lifes*10);
	_lifes--;

	if(_lifes <= 0){
//		std::cout << "Died" << std::endl;
		//_node->getCreator()->destroySceneNode(_node);
		//FIXME temporary hide
		_node->setVisible(false);
		Ogre::Vector3 vt(300 , 300, 0);
		_node->translate(vt);
	}
}
