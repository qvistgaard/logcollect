//
//  Pattern.cpp
//  
//
//  Created by Steffen Sørensen on 10/3/12.
//
//
#include <string>
#include <map>
#include <iostream>
#include <pcre.h>
#include "../include/Patterns.h"
#include "../include/Pattern.h"
#include "../include/Result.h"


logcollect::Patterns::Patterns(logcollect::Logger *log){
	this->log = log;
}


bool logcollect::Patterns::addPattern(Pattern *pattern){
   
    logcollect::pattern_map::iterator pattern_it = this->patterns.find(*pattern->getName());
    
    if(pattern_it == this->patterns.end()){
        this->patterns.insert(pattern_pair(*pattern->getName(), pattern));
		this->log->debug("Added pattern: " + (*pattern->getName()) + " - " + (*pattern->getPattern()));
		return this->_expand();
		return true;
    } else {
		this->log->error("Pattern allready exists: " + (*pattern->getName()));
        return false;
    }
}


logcollect::Pattern* logcollect::Patterns::getPattern(const std::string &name){

    logcollect::pattern_map::iterator pattern_it = this->patterns.find(name);
	
    if(pattern_it != this->patterns.end()){
        return pattern_it->second;
    } else {
        return nullptr;
    }
}

logcollect::Result* logcollect::Patterns::match(std::string pattern_name, std::string data){
	Pattern *pattern = this->getPattern(pattern_name);
	// this->log->info("Matching '" + data + "' against pattern: '" + pattern_name + "'");
	if(pattern){
//		this->log->debug(" - Match OK!");
//		return new Result(data);
		return pattern->match(data);
	} else {
		throw "Pattern does not exists: " + pattern_name;
	}
}

bool logcollect::Patterns::_expand(){
    logcollect::pattern_map::iterator pattern_it;
	
	this->log->debug("Expanding patterns");
	try {
		for(pattern_it = this->patterns.begin(); pattern_it != this->patterns.end(); pattern_it++){
			this->log->debug(" - Expanding pattern: " + (*(*pattern_it->second).getName()));
			pattern_it->second->expand(this);
		}
	} catch (std::string pattern){
		this->log->error("Expansion for pattern failed: " + pattern);
		return false;
	}
	return true;

}

