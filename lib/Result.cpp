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
#include <time.h>

logcollect::Result::Result(std::string data){
	this->data = data;
	this->timestamp = time(NULL);
}

void logcollect::Result::add(std::string field, std::string value){
	std::transform(field.begin(), field.end(), field.begin(), ::tolower);
	this->fields.insert(result_pair(field, value));
}

void logcollect::Result::setTimestamp(time_t timestamp){
	this->timestamp = timestamp;
}

time_t logcollect::Result::getTimestamp(){
	return this->timestamp;
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
	
	char strtime[1024];
	struct tm* tm = localtime(&this->timestamp);
	strftime(strtime, 1024, "%FT%T%z", tm);
	
	std::cout << "---------------------------------------------" << std::endl;
	std::cout << "Data:\t" << this->data << std::endl;
	std::cout << "Timestamp: " << this->timestamp << " " << strtime << std::endl;
	std::cout << "Fields:" << std::endl;
	
	
	logcollect::result_map::iterator it;
	for(it = this->fields.begin(); it != this->fields.end(); it++){
		std::cout << "\t" << it->first << ":\t" << it->second << std::endl;
	}
	std::cout << "---------------------------------------------" << std::endl;
}
