//
//  Inputs.cpp
//  logcollectd
//
//  Created by Steffen Sørensen on 12/23/12.
//
//

#include <iostream>
#include <fstream>
#include <libconfig.h++>

// Load c headers
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cerrno>
#include <sys/uio.h>
#include <unistd.h>

// Kqueue stuff
#include <sys/event.h>
#include <sys/time.h>


#include "../include/Inputs.h"


logcollect::Inputs::Input::Input(){ }

logcollect::Inputs::Input::~Input(){
	delete this->index;
	delete this->converter;
	delete this->pattern;
}

/*
 Missing stuff:
 
	- Load patterns
    - Do Matching
*/
bool logcollect::Inputs::Input::setConfig(libconfig::Setting* config){
	this->config = config;
	return true;
}

bool logcollect::Inputs::Input::setIndex(logcollect::CluceneIndex* index){
	this->index = index;
	return true;
}

bool logcollect::Inputs::Input::setPattern(logcollect::Pattern* pattern){
	this->pattern = pattern;
	return true;
}

bool logcollect::Inputs::Input::setConverter(logcollect::DateConversion* converter){
	this->converter = converter;
	return true;
}


bool logcollect::Inputs::Input::indexLine(const std::string* line){
	return this->indexLine(line->c_str());
}

bool logcollect::Inputs::Input::indexLine(const char* line){
	this->r = this->pattern->match(line);
	if(r){
		this->r->dump();
		this->index->index(this->r, this->converter);
	}
	delete this->r;
	return true;
}

logcollect::Inputs::File::~File(){
	Input::~Input();
	delete[] this->file;
}


bool logcollect::Inputs::File::setConfig(libconfig::Setting* config){
	Input::setConfig(config);
	
	if(config->exists("file")){
		// Read defined file.
		this->file = (*config)["file"];
	} else {
		throw "'file' has not been defined for input source.";
	}
	return true;
}

void logcollect::Inputs::File::run(){
	int kq = kqueue(), fp = -1, nev = -1;
	if(kq == -1){
		perror("kqueue");
	} else {
		for (;;) {
			
			
			struct kevent kv[1], kvc[1];

			// Check and see if file is open, if the file is'nt open try and open it.
			if(lseek(fp, 0, SEEK_CUR) == -1){
				fp = open(this->file, O_RDONLY);
				if(fp == -1){
					printf ("Tried to open file (%s , %d): %s\n", this->file, fp, strerror(errno));
				} else {
					EV_SET(&kv[0], fp, EVFILT_READ, EV_ADD, 0, 0, 0);
				}
  			}
			
			nev = kevent(kq, kv, 1, kvc, 1, NULL);
			if (nev == -1) {
				perror("kevent()");
				exit(EXIT_FAILURE);
			} else if (nev > 0) {
				for (int i = 0; i < nev; i++) {
					// Create buffer for data and read the amount of incoming data recieved
					char buf[kvc[i].data+1];
					read(kvc[i].ident, buf, kvc[i].data);
					
					// Resize line, so wee don't have to do it more than once
					std::string line;
					line.resize(kvc[i].data+1);
					line.erase();

					// Go through data and append to line string
					// should we find ascii code 10 (line feed)
					// we will index the previous line and empty the string
					// making it ready for the next line
					for(int ci = 0; ci <= kvc[i].data; ci++){
						if((int) buf[ci] != 10){
							line += buf[ci];
						} else {
							this->indexLine(&line);
							line.erase();
						}
					}
				}
			}
			
		}
	}
}
