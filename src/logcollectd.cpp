


#include <iostream>
#include <fstream>
#include <CLucene.h>

#include "../include/Pattern.h"
#include "../include/Logger.h"
#include "../include/Result.h"
#include "../include/PatternFile.h"
#include "../include/CluceneIndex.h"
#include "../include/DateConversion.h"

#define PATTERN_LENGTH 1024



void usage(){
	printf("Usage: logcollectd -f <format> -i <index> -r <rules>\n\n");
	printf("   -r format rule file location\n");
	printf("   -i index location\n");
	printf("   -f format to match data against\n\n");
}


int main (int argc, char *const argv[]){
    int argument;
    
    bool  file_rules_flag = false;
    char *file_rules = NULL;
	
    bool  index_location_flag = false;
    char *index_location = NULL;

    bool  format_flag = false;
    char *format = NULL;
	
    while((argument = getopt(argc, argv, "r:i:f:h")) != -1){
        switch(argument){
            case 'r':
                file_rules_flag = true;
                file_rules = optarg;
                break;
            case 'i':
				index_location_flag = true;
                index_location = optarg;
                break;
            case 'f':
				format_flag = true;
                format = optarg;
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


    if(!file_rules_flag || !index_location_flag || !format_flag){
        usage();
        exit(1);
    }
	
	
	// std::string timestamp = "2012-07-19T09:39:59+02:00";
	
	logcollect::DateConversion* converter = new logcollect::DateConversion();
	converter->addFormat(new std::string("%FT%T%z"));
	converter->addFormat(new std::string("%FT%T"));
	
    
    logcollect::Logger *logger = new logcollect::Logger();
    logcollect::Patterns *p = new logcollect::Patterns(logger);
    logcollect::PatternFile *rules = new logcollect::PatternFile(logger, file_rules);
	logcollect::CluceneIndex *index = new logcollect::CluceneIndex(index_location);
	
    rules->addPatterns(p);

	
	
	try {

		int counter = 0;
		logcollect::Result *r;
	
		std::string input_line;

		while(std::cin) {
			getline(std::cin, input_line);
        
			r = p->match(format, input_line);
			if(r){
				index->index(r, converter);
				// r->dump();
				counter++;
			}
		
			delete r;
		}
		std::cout << "Indexed " << counter << "  loglines" << std::endl;
	} catch (CLuceneError &error) {
		printf("Fejl %s\n", error.what());
	} catch (std::string error){
		std::cout << error << std::endl;
	}
	delete index;
    return 0;
}









