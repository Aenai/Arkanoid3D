
#include "RecordManager.h"

#include <iostream>
#include <fstream>



bool basicSort (int i,int j) { return (i<j); }


std::vector<std::string> RecordManager::getRecords(){
	
	std::vector<std::string> records;
	
	
	//Opens file and get all ifnfo
	std::string line;
	std::ifstream myfile ("records.txt");
	if (myfile.is_open()){
		while ( getline (myfile,line) ){
			records.push_back(line);
		}
	myfile.close();
	}
	
	//Gets top 3 numbers
	int actualRecord;
	std::vector<int> top;

	for(unsigned int i=0; i<records.size(); i++){
		actualRecord= atoi(records.at(i).c_str());
		top.push_back(actualRecord);
	}
	
	std::sort (top.begin(), top.end(), basicSort);
	
	for (std::vector<int>::iterator it=top.begin(); it!=top.end(); ++it)
    	std::cout << ' ' << *it;
	

	
	return records;
	
}

void RecordManager::keepRecord(int points){
	
	std::ofstream myfile ("records.txt", std::ios::app);
	if (myfile.is_open()){
		myfile << points << std::endl;
		myfile.close();
	}
	else std::cout << "Unable to open file";
	
}


RecordManager::RecordManager(){
	_points = 0;
}

int RecordManager::getPoints(){
	return _points;
}

void RecordManager::addPoints(int points){
	_points += points;
}






