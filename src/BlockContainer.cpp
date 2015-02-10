#include "BlockContainer.h"



BlockContainer::BlockContainer (Ogre::SceneManager* sceneMgr,RecordManager* recordMgr, Ball* playBall, GhostBall* ghostBall){
	_sceneMgr = sceneMgr;
	_blocks = new std::vector<Block*>();
	_recordMgr = recordMgr;
	_playBall = playBall;
	_ghostBall = ghostBall;
	_numBlock = 0;
	_currentBlocks = 0;
}



void BlockContainer::createBlock (int x, int y, int hard){
	
	//Name Management
	std::ostringstream r_string;
	r_string << "block_" << _numBlock;
	_numBlock += 1;
	_currentBlocks++;
	
	//Generating Block
	Ogre::Entity* ent1 = _sceneMgr->createEntity(r_string.str(), "cube.mesh");
	Ogre::SceneNode* nodeBlock =  _sceneMgr->createSceneNode(r_string.str());
	nodeBlock->attachObject(ent1);
	_sceneMgr->getRootSceneNode()->addChild(nodeBlock);
	nodeBlock->setScale(2,1,1.2);
	nodeBlock->setPosition(x, y, -40);
	
	//Block vector Logic
	Block* newBlock = new Block(nodeBlock, _playBall, _ghostBall, _recordMgr, hard);
	_blocks->push_back(newBlock);
	
}


void BlockContainer::checkCollision(){
	for (std::vector<Block*>::iterator it = _blocks->begin() ; it != _blocks->end(); ++it){
		(*it)->updateVariables();
    	(*it)->checkCollision();
    	if((*it)->isDead()){
    		_blocks->erase(it);
    		_currentBlocks--;
    	}
    }
}

void BlockContainer::levelGenerator(int level){
	int posx = -18;
	int posy = -8;
	int valHard = 0;
	srand(time(NULL));

	switch (rand()%3){
	case 0:
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j<10; j++) {
				valHard = rand()%5 + 1;	
				createBlock(posx,posy, valHard);
				posx = posx + 4;
			}
			posx = -18;
			posy = posy - 3;
		}
		break;
	case 1:
		for (int i = 0; i < 5; i++) {
			valHard = rand()%5 + 1;	
			for (int j = 0; j<10; j++) {
				createBlock(posx,posy, valHard);
				posx = posx + 4;
			}
			posx = -18;
			posy = posy - 3;
		}
		break;
	case 2:
		for (int i = 0; i <10; i++) {
			valHard = rand()%5 + 1;
			for (int j = 0; j<5; j++) {
				createBlock(posx,posy, valHard);
				posy = posy - 3;
			}
			posy = -8;
			posx = posx + 4;
		}
		break;
	
	}
}