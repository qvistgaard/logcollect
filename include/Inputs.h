//
//  Inputs.h
//  logcollect
//
//  Created by Steffen Sørensen on 2/1/13.
//  Copyright (c) 2013 Steffen Sørensen. All rights reserved.
//

#ifndef logcollect_Inputs_h
#define logcollect_Inputs_h

#include <libconfig.h++>
#include "CluceneIndex.h"
#include "Pattern.h"
#include "Patterns.h"

#include <map>

// Kqueue stuff
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>

#include <glob.h>

namespace logcollect {
	namespace Inputs {
		
		
		
		class Input {
		protected:
			libconfig::Setting* config;
			std::string datadir;
			Input();
			
		private:
			logcollect::CluceneIndex* index;
			logcollect::Patterns* patterns;
			logcollect::Result *r;
			logcollect::DateConversion* converter;
			logcollect::Pattern* pattern;
			
			

		public:
			virtual void run() = 0;
			
			template<class I>
			// , logcollect::Pattern* pattern, logcollect::DateConversion* converter
			static I* createInput(libconfig::Setting* config, logcollect::Patterns* patterns){
				I* input = new I;
				input->setConfig(config);
				input->setPatterns(patterns);
				return input;
			}
			
			bool setConfig(libconfig::Setting* config);
			bool setPatterns(logcollect::Patterns* patterns);
			
			bool indexLine(const char* line);
			bool indexLine(const std::string* line);
			~Input();
			
		};
	/*
		class FIFO : public Input {
		//	bool setConfig(libconfig::Setting* config);
		};
	*/
		
		// int=filepointer,int=byteoffset
		/*
		typedef std::pair<int,int> file_status;
		typedef std::pair<int,struct kevent> file_event;
		 */
// 		typedef std::map<std::string, file_event> event_list;
		
		struct file_status {
			file_status() : fp(-1), pos(0) { }
			int fp;
			int pos;
			std::string filename;
			struct kevent event;
		};
		typedef std::pair<std::string, file_status*> file_pair;
		typedef std::map<std::string, file_status*> file_list;


		class File : public Input {
		private:
			const char* files;
			file_list list;
			int kq;
			libconfig::Config* fileinfo_config;
			libconfig::Setting* fileinfo;
			std::string fileinfo_cache;
			
			
			void _ReadFile(int fp);
			void _updateFileList();
			void _writeFileData();
			void _sanitizeFilename(const std::string* from, std::string* to);
			int _getFilePos(const std::string* filename);
			int _setFilePos(const std::string* filename, int pos);
				
		public:
			File();
			void run();
			bool setConfig(libconfig::Setting* config);
			~File();
		};
		
	}
	
}


#endif
