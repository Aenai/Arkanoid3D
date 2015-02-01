#include "BlockContainer.h"



BlockContainer::BlockContainer (Ogre::SceneManager* sceneMgr,RecordManager* recordMgr, Ball* playBall){
	_sceneMgr = sceneMgr;
	_blocks = new std::vector<Block*>();
	_recordMgr = recordMgr;
	_playBall = playBall;
	_numBlock = 0;
}



void BlockContainer::createBlock (int x, int y){
	
	//Name Management
	std::ostringstream r_string;
	r_string << "block_" << _numBlock;
	_numBlock += 1;
	
	//Generating Block
	Ogre::Entity* ent1 = _sceneMgr->createEntity(r_string.str(), "cube.mesh");
	Ogre::SceneNode* nodeBlock =  _sceneMgr->createSceneNode(r_string.str());
	nodeBlock->attachObject(ent1);
	_sceneMgr->getRootSceneNode()->addChild(nodeBlock);
	nodeBlock->setScale(2,1,1.2);
	nodeBlock->setPosition(x, y, -40);
	
	//Block vector Logic
	Block* newBlock = new Block(nodeBlock, _playBall, _recordMgr, 2);
	_blocks->push_back(newBlock);
	
}


void BlockContainer::checkCollision(){
	for (std::vector<Block*>::iterator it = _blocks->begin() ; it != _blocks->end(); ++it){
		(*it)->updateVariables();
    	(*it)->checkCollision();
    }
}
