


#include <iostream>
#include <iomanip>
#include <fstream>
#include <CLucene.h>
#include <stdexcept>  
#include <librelp.h>
#include <libconfig.h++>

#include "../include/Pattern.h"
#include "../include/Logger.h"
#include "../include/Result.h"
#include "../include/Inputs.h"
#include "../include/PatternFile.h"
#include "../include/CluceneIndex.h"
#include "../include/DateConversion.h"


#define PATTERN_LENGTH 1024



void usage(){
	printf("Usage: logcollectd -c <config>\n\n");
	printf("   -c Config file\n\n");	
}


int main (int argc, char *const argv[]){
    int argument;
	
    bool  daemon_flag = false;
	
    bool  config_flag = false;
    char *config = NULL;
	
	
    while((argument = getopt(argc, argv, "c:hd")) != -1){
        switch(argument){
            case 'c':
				config_flag = true;
                config = optarg;
                break;
            case 'd':
				daemon_flag = true;
				break;
            case 'h':
            case '?':
            default:
                usage();
                exit(0);
                break;
        }
    }
    argc -= optind;
    argv += optind;


    if(!config_flag){
        usage();
        exit(1);
    }
	
	logcollect::Logger *logger = NULL;
	char logline[4096];
	// Start the logger
	if(!daemon_flag){
		logger = new logcollect::Logger();
	} else {
		printf("Daemon flag is not implimented yet!");
		exit(1);
	}
	

	try {
		// Create config instance
		libconfig::Config cfg;
		
		// Declare config variables
		std::string cfg_patterns = "patterns";
		std::string cfg_datadir = "datadir";
		std::string cfg_indexer = "indexer"; // Indexer is unused for now
		std::string cfg_sources = "sources";
		
		std::string cfg_input_type = "type";
		std::string cfg_input_format = "format";
		std::string cfg_input_date_formats = "date_formats";
		
		// Declare config variable values
		std::string cfg_patterns_file, cfg_datadir_location;
		
		// Read config file and confirm with logline
		cfg.readFile(config);
		sprintf(logline, "Loaded config file: %s", config);
		logger->info(logline);
		
		
		// Lookup configuration values
		cfg.lookup(cfg_patterns);
		cfg.lookup(cfg_indexer);
		cfg.lookup(cfg_datadir);
		libconfig::Setting& sources = cfg.lookup(cfg_sources);

		
		// Read core configuration values
		cfg.lookupValue(cfg_patterns, cfg_patterns_file);
		cfg.lookupValue(cfg_datadir, cfg_datadir_location);
		
		// Print some log informations about index and rules file
		sprintf(logline, "Using rules file: %s", cfg_patterns_file.c_str());
		logger->info(logline);
		sprintf(logline, "Using datadir location: %s", cfg_datadir_location.c_str());
		logger->info(logline);
		
		
		// Load and parse rules
		logcollect::Patterns *p = new logcollect::Patterns(logger);
		logcollect::PatternFile *rules = new logcollect::PatternFile(logger, cfg_patterns_file);
		rules->addPatterns(p);

		

// 		std::cout << sources.getLength() << " sources found" << std::endl;

		
		logcollect::Inputs::Input* input[sources.getLength()];
		
		
		
		for(int i = 0; i < 1; i++){
			std::string type;
			sources[i].lookupValue(cfg_input_type, type);
			/*
			std::string type, format;

			sources[i].lookupValue(cfg_input_type, type);
			
			std::string index_location;
			if(cfg_index_location.at(cfg_datadir_location.length()-1) == '/'){
				index_location = cfg_datadir_location + sources[i].getName() + "/";
			} else {
				index_location = cfg_datadir_location + "/" + sources[i].getName() + "/";
			}
			*/
			/*
			sources[i].lookupValue(cfg_input_format, format);
			logcollect::Pattern* pattern = p->getPattern(format);
			
			if(!pattern){
				throw "Could not find pattern";
			}

			logcollect::DateConversion* converter = new logcollect::DateConversion();
			if(sources[i].exists("date_formats")){
				std::string cformat;
				libconfig::Setting* formats = &sources[i]["date_formats"];
				std::cout << formats->getLength() << std::endl;
				for(int di = 0; di < formats->getLength(); di++){
					const char *s = sources[i]["date_formats"][di];
					converter->addFormat(s);
				}
			}
			*/
			
			if(type.compare("file") == 0){
				input[i] = logcollect::Inputs::Input::createInput<logcollect::Inputs::File>(&sources[i], p);
				input[i]->run(); // Move this to a later stage, only here for testing
//				delete input[i];
			} else {
				std::cout << "Invalid type: " << type << std::endl;
			}

			// std::cout << " - " << sources[i].getName() << type  << " " << index_location << std::endl;
	

		}


	
		
	} catch(libconfig::ParseException e){
		std::cout << e.getError() << " in line: " << e.getLine() << std::endl;
	} catch(libconfig::SettingNotFoundException e){
		std::cout << e.getPath() << " Was not found" << std::endl;
	} catch(char const* e){
		std::cout << e << std::endl;
	} catch(libconfig::FileIOException e){
		std::cout << "Libconfig error: " << e.what() << std::endl;
	} catch(libconfig::SettingNameException e){
		std::cout << "Libconfig error: " << e.what() << std::endl;
	} catch(std::logic_error e){
		std::cout << "Logic error: " << e.what() << std::endl;
	} catch(CLuceneError e){
		std::cout << "Clucene error: " << e.what() << std::endl;
	}
	
	
	
	
	return 0;
	
	/*
	// std::string timestamp = "2012-07-19T09:39:59+02:00";
	
	logcollect::DateConversion* converter = new logcollect::DateConversion();
	converter->addFormat(new std::string("%FT%T%z"));
	converter->addFormat(new std::string("%FT%T"));
	
    

//    logcollect::Patterns *p = new logcollect::Patterns(logger);
//    logcollect::PatternFile *rules = new logcollect::PatternFile(logger, file_rules);
//	  logcollect::CluceneIndex *index = new logcollect::CluceneIndex(index_location);
//    rules->addPatterns(p);

	
	
	time_t start, end;
	float perfcounter = 0;
	
	time(&start);
	

	
	try {

		int counter = 0;
		logcollect::Result *r;
	
		std::string input_line;

		while(std::cin) {
			getline(std::cin, input_line);
        
			r = p->match(format, input_line);
			if(r){
				index->index(r, converter);
			//	r->dump();
				counter++;
				time(&end);
				std::cout << "\r";
				
				perfcounter = (double) (end - start);
				
//				printf(" Test   %f %d", perfcounter, (end - start));
				std::cout << std::right << std::setw(10) << counter;
				if(perfcounter > 0){
					std::cout << std::right << std::setw(10) << std::setprecision(0) << std::fixed << (counter / perfcounter);
					std::cout << "/sec\t Time: " << (end - start);
				}
				

			}
		
			delete r;
		}
		std::cout << "\n";
//		std::cout << "\rIndexed " << counter << "  loglines" << std::endl;
 
		delete index;
		
	} catch (CLuceneError &error) {
		printf("Fejl %s\n", error.what());
	} catch (std::string error){
		std::cout << error << std::endl;
	} catch (std::bad_alloc& ba){
		std::cout << "bad_alloc caught: " << ba.what() << std::endl;
	}
	 */
    return 0;

					 
}









