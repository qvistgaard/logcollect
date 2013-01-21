//
//  Logger.cpp
//
//
//  Created by Steffen Sørensen on 10/3/12.
//
//
#include <string>
#include <iostream>
#include "../include/Logger.h"

#define WARNING 1;

void logcollect::Logger::critical(const std::string &logline){
    this->_write("CRITICAL", logline);
}

void logcollect::Logger::error(const std::string &logline){
    this->_write("ERROR", logline);
}

void logcollect::Logger::warning(const std::string &logline){
    this->_write("WARNING", logline);
}

void logcollect::Logger::notice(const std::string &logline){
    this->_write("NOTICE", logline);
}

void logcollect::Logger::info(const std::string &logline){
    this->_write("INFO", logline);
}

void logcollect::Logger::debug(const std::string &logline){
    this->_write("DEBUG", logline);
}

void logcollect::Logger::_write(const std::string &level, const std::string &logline){
    std::cout << "LOGGER " << level << ": " << logline << std::endl;
    
}
