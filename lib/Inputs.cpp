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
#include <map>

// Load c headers
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cerrno>
#include <sys/uio.h>
#include <unistd.h>
#include <glob.h>
#include <err.h>

// Kqueue stuff
#include <sys/event.h>
#include <sys/time.h>


#include "../include/Inputs.h"
#include "../include/Patterns.h"


logcollect::Inputs::Input::Input(){ }

logcollect::Inputs::Input::~Input(){
	delete this->index;
	delete this->converter;
}

bool logcollect::Inputs::Input::setConfig(libconfig::Setting* config){
	this->config = config;
	
	
	// Get config root
	libconfig::Setting& root = this->config->getParent().getParent();
	
	
	// Set datadir
	root.lookupValue("datadir", this->datadir);
	
	
	// Initialize datadir
	if(this->datadir.at(this->datadir.length()-1) == '/'){
		this->datadir.erase(this->datadir.length()-1, 1);
	}
	
	
	// create index
	std::string index_location = this->datadir + "/" + this->config->getName() + "/";
	this->index = new CluceneIndex(index_location);
	
	
	// Create dataformet converter
	this->converter = new logcollect::DateConversion();
	if(config->exists("date_formats")){
		libconfig::Setting& formats = (*config)["date_formats"];
		for(int di = 0; di < formats.getLength(); di++){
			const char *s = formats[di];
			this->converter->addFormat(s);
		}
	}
	return true;
}


bool logcollect::Inputs::Input::setPatterns(logcollect::Patterns* patterns){
	this->patterns = patterns;
	
	std::string pattern;
	this->config->lookupValue("format", pattern);
	this->pattern = this->patterns->getPattern(pattern);
	if(!this->pattern){
		throw "Could not find pattern: " + pattern;
	}
	return true;
}


bool logcollect::Inputs::Input::indexLine(const std::string* line){
	return this->indexLine(line->c_str());
}

bool logcollect::Inputs::Input::indexLine(const char* line){
	this->r = this->pattern->match(line, this->converter);
	
	if(r){
		this->r->dump();
		this->index->index(this->r, this->converter);
	}
	delete this->r;
	 
	return true;
}

logcollect::Inputs::File::File(){
	this->kq = kqueue();
}
logcollect::Inputs::File::~File(){
	Input::~Input();
 	delete this->files;
}


bool logcollect::Inputs::File::setConfig(libconfig::Setting* config){
	Input::setConfig(config);

	if(config->exists("file")){
		// Read defined file.
		this->files = (*config)["file"];
//		this->_updateFileList();
	} else {
		throw "'file' has not been defined for input source.";
	}

	// Initialize fileinfo cache
	this->fileinfo_cache = this->datadir + "/" + this->config->getName() + ".dat";
	char* fullpath = realpath(this->fileinfo_cache.c_str(), NULL);
	
	// Check if a old config exists, if it does load it
	struct stat st;
	this->fileinfo_config = new libconfig::Config();
	if(stat(fullpath, &st) == 0){
		this->fileinfo_config->readFile(this->fileinfo_cache.c_str());
	} else {
		perror("stat");

	}
	this->fileinfo = &this->fileinfo_config->getRoot();
	
	delete[] fullpath;
	return true;
}

void logcollect::Inputs::File::_updateFileList(){
	glob_t files;
	glob(this->files, GLOB_BRACE, NULL, &files);
	
	if(files.gl_matchc > 0){
		for (int i = 0; i < files.gl_matchc; i++) {
			std::string filename = files.gl_pathv[i];
			if(this->list.find(filename) == this->list.end()){
				struct file_status* status = new struct file_status();
				status->filename = filename;
				this->list.insert(file_pair(filename, status));
				
				if(lseek(status->fp, 0, SEEK_CUR) == -1){
					status->fp = open(filename.c_str(), O_RDONLY);
					if(status->fp == -1){
						printf ("Tried to open file (%s , %d): %s\n", filename.c_str(), status->fp, strerror(errno));
					} else {
						int pos = this->_getFilePos(&filename);
 						lseek(status->fp, pos, SEEK_SET);
						
						EV_SET(&status->event, status->fp, EVFILT_VNODE, EV_ADD,
							   NOTE_DELETE | NOTE_RENAME, 0, status);
						
						if (kevent(this->kq, &status->event, 1, NULL, 0, NULL) == -1){
							err(1, "kevent %s", filename.c_str());
						}
						
						EV_SET(&status->event, status->fp, EVFILT_READ, EV_ADD, 0, 0, status);
						
						if (kevent(this->kq, &status->event, 1, NULL, 0, NULL) == -1){
							err(1, "kevent %s", filename.c_str());
						}
					}
				}
			}
		}
	}
	globfree(&files);
}

void logcollect::Inputs::File::_sanitizeFilename(const std::string* from, std::string* to){
	int pos;
	
	to->assign("fd_");
	to->append(from->c_str());
	while((pos = to->find("/")) != std::string::npos){
		to->replace(pos, 1, "_");
	}
	while((pos = to->find(".")) != std::string::npos){
		to->replace(pos, 1, "-");
	}
}

int logcollect::Inputs::File::_getFilePos(const std::string* filename){
	std::string clean_name;
	
	this->_sanitizeFilename(filename, &clean_name);
	if(!this->fileinfo->exists(clean_name)){
		this->fileinfo->add(clean_name, libconfig::Setting::Type::TypeInt);
		(*this->fileinfo)[clean_name] = 0;
	}

	return (*this->fileinfo)[clean_name];
}

int logcollect::Inputs::File::_setFilePos(const std::string* filename, int pos){
	int old_pos = this->_getFilePos(filename);

	std::string clean_name;
	this->_sanitizeFilename(filename, &clean_name);
	
	(*this->fileinfo)[clean_name] = pos;
	return old_pos;
}

void logcollect::Inputs::File::_writeFileData(){
	file_list::iterator it;
	for(it = this->list.begin(); it != this->list.end(); it++){
		this->_setFilePos(&it->first, it->second->pos);
	}
	this->fileinfo_config->writeFile(this->fileinfo_cache.c_str());
	std::cout << "Wrote file info" << std::endl;
}


void logcollect::Inputs::File::run(){
	if(this->kq == -1){
		perror("kqueue");
	} else {
		struct timespec timeout = { 1800, 0 };
		struct kevent ke;
		for (;;) {
			this->_updateFileList();
			
			int nev = kevent(this->kq, NULL, 0, &ke, 1, &timeout);
			if (nev == -1) {
				perror("kevent()");
				exit(EXIT_FAILURE);
			} else if (nev > 0) {
				struct file_status* f = (struct file_status *) ke.udata;
				if (ke.filter == EVFILT_READ) {
					if (ke.data < 0) {
						int offset = lseek(f->fp, 0, SEEK_END);
						f->pos = offset;
					} else {
						// Resize line, so wee don't have to do it more than once
						std::string line;
						line.resize(8196);
						line.erase();
						
						// Go through data and append to line string
						// should we find ascii code 10 (line feed)
						// we will index the previous line and empty the string
						// making it ready for the next line
						for(int ci = 0; ci <= ke.data; ci++){
							char buf;
							read(ke.ident, &buf, 1);
							f->pos++;
							
							if((int) buf != 10){
								line += buf;
							} else {
								this->indexLine(&line);
								line.erase();
							}
						}
					}
				} else if(ke.filter == EVFILT_VNODE){
					if(ke.fflags & NOTE_DELETE || ke.fflags & NOTE_RENAME){
						close(f->fp);
						this->list.erase(f->filename);
					}
				}
			}
			this->_writeFileData();
		}
	}
}
