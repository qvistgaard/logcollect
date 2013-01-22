//
//  search.cpp
//  logcollectd
//
//  Created by Steffen Sørensen on 1/4/13.
//
//

#include <iostream>
#include <fstream>
#include <CLucene.h>
#include <CLucene/config/repl_tchar.h>
#include <jansson.h>
#include "../include/CluceneIndexReader.h"



void usage(){
    printf("Missing -i argument, please specify a index\n");
}


void ctow(const char* from, TCHAR* to){
#if defined(_UCS2)
	std::wstring test;
	test.assign(from, from+strlen(from));
	wcsncpy(to, test.c_str(), test.length()+1);
#else
	std::cout << "NORMAL" << std::endl;
#endif

}



int main (int argc, char *argv[]){
	
    int argument;

    bool  index_location_flag = false;
    char *index_location;

	bool  field_arg_flag = false;
    char *field_arg;
	

	bool  query_arg_flag = false;
    char *query_arg;

   
    while((argument = getopt(argc, argv, "i:hq:f:")) != -1){
        switch(argument){
            case 'i':
				index_location_flag = true;
                index_location = optarg;
                break;
            case 'q':
				query_arg_flag = true;
                query_arg = optarg;
                break;
            case 'f':
				field_arg_flag = true;
                field_arg = optarg;
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
	
	
    if(!index_location_flag){
        usage();
        exit(1);
    }

	
	try {
		logcollect::CluceneIndexReader* reader = new logcollect::CluceneIndexReader(index_location);

		json_t* resultset = json_object();
		
	
		int results, limit=30, offset=0;
		
		
		json_t* objects = json_array();
		results = reader->query(objects, query_arg, field_arg, limit, offset);
		
		json_object_set(resultset, "count", json_integer(results));
		json_object_set(resultset, "limit", json_integer(limit));
		json_object_set(resultset, "offset", json_integer(offset));
		json_object_set(resultset, "result", objects);
		
		std::cout << json_dumps(resultset, JSON_ENCODE_ANY) << std::endl;
		
		
		

		delete reader;
		
		
	} catch (CLuceneError &error) {
		printf("Fejl %s\n", error.what());
	} catch (const char* &e){
		printf("Fejl %s\n", e);
	}
}




