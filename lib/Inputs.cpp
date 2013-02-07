//
//  Result.cpp
//  logcollectd
//
//  Created by Steffen Sørensen on 12/23/12.
//
//

#include "../include/Result.h"
#include <iostream>
#include <string>

logcollect::Result::Result(std::string data){
	this->data = data;
}

void logcollect::Result::add(std::string field, std::string value){
	std::transform(field.begin(), field.end(), field.begin(), ::tolower);
	this->fields.insert(result_pair(field, value));
}

logcollect::result_map* logcollect::Result::getFields(){
	return &this->fields;
}

const std::string* logcollect::Result::getData(){
	return &this->data;
}

logcollect::Result::~Result(){

}


void logcollect::Result::dump(){
	std::cout << "---------------------------------------------" << std::endl;
	std::cout << "Data:\t" << this->data << std::endl;
	std::cout << "Fields:" << std::endl;
	
	logcollect::result_map::iterator it;
	for(it = this->fields.begin(); it != this->fields.end(); it++){
		std::cout << "\t" << it->first << ":\t" << it->second << std::endl;
	}
	std::cout << "---------------------------------------------" << std::endl;
}
