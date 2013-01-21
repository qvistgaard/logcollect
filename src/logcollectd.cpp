


#include <iostream>
#include <fstream>
#include <CLucene.h>

#include "../include/Pattern.h"
#include "../include/Logger.h"
#include "../include/Result.h"
#include "../include/PatternFile.h"
#include "../include/CluceneIndex.h"

#define PATTERN_LENGTH 1024



void usage(){
    printf("Missing -r argument, please specify a ruleset\n");
}


int main (int argc, char *const argv[]){
    int argument;
    
    bool  file_rules_flag = false;
    char *file_rules;
	
    bool  index_location_flag = false;
    char *index_location;

    

    

    // char logline[PATTERN_LENGTH];
   // logcollect::Logparser parser;
    
    while((argument = getopt(argc, argv, "r:i:h")) != -1){
        switch(argument){
            case 'r':
                file_rules_flag = true;
                file_rules = optarg;
                break;
            case 'i':
				index_location_flag = true;
                index_location = optarg;
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


    if(!file_rules_flag || !index_location_flag){
        usage();
        exit(1);
    }

    
    logcollect::Logger *logger;
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
        
			r = p->match("RSYSLOGBASE", input_line);
			if(r){
				index->index(r);
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
	
/*
	r = p->match("SYSLOGBASE", "Aug 26 11:20:12 freebsd postfix/qmgr[1079]: A2EF2284D9: from=<www@gormlarsenzornig.com>, size=933, nrcpt=1 (queue active)");
	
	if(r){
		index->index(r);
		r->dump();
	}
	delete r;
	
	
	delete index;
	
//    logcollect::Pattern *pattern = p->getPattern("MAC");
//    std::cout << "Patttern:" << (*pattern->getPattern()) << std::endl;
    
    /*
    ifstream rules(rules_file);
    if(rules.is_open()){
        while(rules.good()){
            rules.getline(logline, sizeof(logline));
            parser.addRule(logline);
        }
        
    } else {
        printf("Could not load rules, no such file or directory\n");
        exit(1);
    }
    
    rules.close();
    
    parser.parseRules();
    
    */
    /*
    
    std::string input_line;
    while(std::cin) {
        getline(std::cin, input_line);
        
        parser.match("SYSLOGBASE", input_line);
    };
    
    /*
     
     parser.match("SYSLOGBASE", "Aug 26 11:20:12 freebsd postfix/qmgr[1079]: A2EF2284D9: from=<www@gormlarsenzornig.com>, size=933, nrcpt=1 (queue active)");
     cout << "END" << endl;
     */
    return 0;
}









