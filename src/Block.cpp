#include "Block.h"



Block::Block (Ogre::SceneNode* newBlock, Ball* ball, GhostBall* ghostBall, RecordManager* recordMgr, int hard):CollisionableObject(newBlock, ball, ghostBall){
	_lifes = hard;
	_recordMgr = recordMgr;
}

void Block::hasCollided(){
//	std::cout << "Alive" << std::endl;
	if(!playBall->getGhost()){
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
}

bool Block::isDead(){
	return _lifes <= 0;
}

