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
#include <jansson.h>
#include <CLucene/config/repl_tchar.h>



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


std::wstring Replace(const std::wstring& orig,const std::wstring& fnd, const std::wstring& repl){
	std::wstring ret = orig;
    size_t pos = 0;
	
    while(true) {
        pos = ret.find(fnd,pos);
//		std::cout << pos << " " << std::wstring::npos << std::endl;
        if(pos == std::wstring::npos)  // no more instances found
            break;
		
        ret.replace(pos, fnd.size(), repl);  // replace old string with new string
        pos += repl.size();
    }
	
    return ret;

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

	lucene::index::IndexReader* reader;
	
	if(lucene::index::IndexReader::indexExists(index_location)){
		reader = lucene::index::IndexReader::open(index_location);
	} else {
		printf("Unable to access index, index does not exists\n");
		exit(1);
	}
	
	lucene::analysis::standard::StandardAnalyzer analyzer;
	lucene::search::IndexSearcher s(reader);
	
	int length = strlen(query_arg);
	
	
	
	//std::wstring q_string = "TEST";
	TCHAR field[] = L"logline";
	TCHAR query[length+1];
	
	ctow(query_arg, query);
	
	/*
	std::cout << "Documents in index: " << reader->numDocs() << std::endl;
	std::cout << "Field: '" << field_arg << "'" << std::endl;
	std::wcout << "Query: '" <<	query << "'" << std::endl;
	*/
	
//	std::cout << " [ " << std::endl;
	
	try {
		lucene::search::Query* q = lucene::queryParser::QueryParser::parse(query, field, &analyzer);
		lucene::search::Hits* h = s.search(q);
		// printf("Hits: %d\n", h->length());
		
		json_t* objects = json_array();
		
		
		int length = h->length();
		
		if(length > 30){
			length = 30;
		}
		
		
		
		
		lucene::document::Document doc;
		for(int i = 0; i < length; i++){
			
			json_t* object = json_object();
			
			
			doc = h->doc(i);
			
			
			lucene::document::DocumentFieldEnumeration* tfields;
			lucene::document::Field* tfield;
			
			tfields = doc.fields();
//			std::cout << "{\n";
			while(tfield = tfields->nextElement()){
				
				json_t* value = json_string((char*) tfield->stringValue());
				json_object_set(object, (char*) tfield->name(), value);
				/*
				const TCHAR* fname = tfield->name();
				const TCHAR* value = tfield->stringValue();
				
				std::wstring valuej = value;
				std::wstring replaced = Replace(valuej, L"\"", L"\\\"");
				
				std::wcout << "\t\"" << fname << "\":\t\"" << replaced << "\"";
				
				if(tfields->hasMoreElements()){
					std::cout << ",\n";
				}
				 */
			}
			
			json_array_append(objects, object);
			
			/*
			if((i+1) == length){
				std::cout << "}";				
			} else {
				std::cout << "},\n";
			}
*/
			
		}
		
		// std::cout << "LENGTH: "<< h->length() << std::endl;
		
		// printf("ID:     %d\n", h->id(1));
		
//		lucene::document::Document* document;
//		reader->document(1, document);
		// lucene::document::DocumentFieldEnumeration* fields = document->fields();
		
		
		std::cout << json_dumps(objects, JSON_ENCODE_ANY);
		
		delete reader;
		// std::cout << " ] " << std::endl;
		
		
	}
	catch (CLuceneError &error) {
		printf("Fejl %s\n", error.what());
	}
	
	
	
	
	
	
	
//	std::cout << *field << " > " << *query << " > " << (*q->toString()) << " > " << *(const TCHAR*) field << std::endl;
//	std::cout << h->length() << std::endl;
	
}




