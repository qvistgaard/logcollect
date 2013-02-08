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

// Kqueue stuff
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>

namespace logcollect {
	namespace Inputs {
		
		
		
		class Input {
		protected:
			libconfig::Setting* config;
			Input();
			
		private:
			logcollect::CluceneIndex* index;
			logcollect::Pattern* pattern;
			logcollect::Result *r;
			logcollect::DateConversion* converter;
			
			

		public:
			virtual void run() = 0;
			
			template<class I>
			static I* createInput(libconfig::Setting* config, logcollect::CluceneIndex* index, logcollect::Pattern* pattern, logcollect::DateConversion* converter){
				I* input = new I;
				input->setConfig(config);
				input->setIndex(index);
				input->setPattern(pattern);
				input->setConverter(converter);
				
				return input;
			}
			
			bool setConfig(libconfig::Setting* config);
			bool setPattern(logcollect::Pattern* pattern);
			bool setIndex(logcollect::CluceneIndex* index);
			bool setConverter(logcollect::DateConversion* converter);
			bool indexLine(const char* line);
			bool indexLine(const std::string* line);
			~Input();
			
		};
	/*
		class FIFO : public Input {
		//	bool setConfig(libconfig::Setting* config);
		};
	*/
		
		class File : public Input {
		private:
			const char* file;
				
		public:
			void run();
			bool setConfig(libconfig::Setting* config);
			~File();
		};
		
	}
	
}


#endif
