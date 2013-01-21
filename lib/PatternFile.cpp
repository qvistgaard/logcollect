//
//  Pattern.cpp
//  
//
//  Created by Steffen Sørensen on 10/3/12.
//
//
#include <string>
#include <fstream>
#include <iostream>
#include <new>

#include "../include/Pattern.h"
#include "../include/PatternFile.h"
#include "../include/Logger.h"
// #include "Patterns.h"

#define MAX_PATTERN_LINE_LENGTH 4096
#define COMMENT_PREFIX "#"

logcollect::PatternFile::PatternFile(Logger *log, const std::string &filename){
    this->log = log;
    this->stream.open(filename.c_str());
    
    if(this->stream.is_open()){
        this->log->info("Opened patternfile: " + filename);
    } else {
        this->log->error("Unable to open patterfile: " + filename);
    }
}

logcollect::Pattern* logcollect::PatternFile::_buildPattern(const char *line){
    
    std::string pattern_name, pattern, pattern_line = line;
    
    // Check and see if the first character is a COMMENT_PREFIX, if it
    // is treat line as a comment and ignore.
    if((!strncmp(line, COMMENT_PREFIX, 1) == 0) && strlen(line) > 0){
        // Find first occurence of a space in order to find the macro name
        char *str_count;
        str_count = strchr(line, ' ');
        
        // Read position for first space, this is done by finding the difference between the
        // two pointers.
        int position = (int) (str_count - line);
        
        pattern_name = pattern_line.substr(0, position);
        pattern = pattern_line.substr(position+1, pattern_line.length());
        
        
        // Return a new Pattern object pointer because the rule was not a comment, and a valid row
        return new Pattern(pattern_name, pattern);
    } else {
        // Return false if it is invalid
        return false;
    }

}

void logcollect::PatternFile::addPatterns(Patterns *patterns){
    
    if(this->stream.is_open()){
        
        char pattern_line[MAX_PATTERN_LINE_LENGTH];
        
        while(this->stream.good()){
            this->stream.getline(pattern_line, sizeof(pattern_line));
            
            logcollect::Pattern *pattern = this->_buildPattern(pattern_line);
            
            if(pattern){
                bool result = false;
                result = patterns->addPattern(pattern);
                if(result){

                } else {
                    this->log->warning("Could not add pattern: " + (*pattern->getName()));
                }
            }
        }
    }
}
