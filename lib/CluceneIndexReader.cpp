//
//  CluceneIndexReader.cpp
//  logcollect
//
//  Created by Steffen Sørensen on 1/21/13.
//  Copyright (c) 2013 Steffen Sørensen. All rights reserved.
//
#include <iostream>
#include <vector>
#include <jansson.h>
#include <CLucene.h>
#include "../include/CluceneIndexReader.h"


logcollect::CluceneIndexReader::CluceneIndexReader(const char* index){
	if(lucene::index::IndexReader::indexExists(index)){
		this->reader = lucene::index::IndexReader::open(index);
	} else {
		throw "Unable to access index, index does not exists";
	}
	this->analyzer = new lucene::analysis::standard::StandardAnalyzer;
	this->searcher = new lucene::search::IndexSearcher(this->reader);
}

int logcollect::CluceneIndexReader::getTotal(){
	return this->reader->numDocs();
}

int logcollect::CluceneIndexReader::query(json_t* json, const char* query, const char* field, int limit, int offset){
	
	TCHAR* wquery = new TCHAR[strlen(query)];
	TCHAR* wfield = new TCHAR[strlen(field)];
	

 	this->makeTCHAR(query, wquery);
	this->makeTCHAR(field, wfield);
	
	lucene::search::Sort* s = new lucene::search::Sort(L"timestamp", true);
	lucene::search::Query* q = lucene::queryParser::QueryParser::parse(wquery, wfield, this->analyzer);
	lucene::search::Hits* h = this->searcher->search(q, s);
// 	lucene::search::Hits* h = this->searcher->search(q);

	
	int length = h->length() - offset - 1;
	if(length > limit){
		length = limit + offset;
	}
	
	/*
	printf("Lenght: %d\n", h->length());
	printf("Limit: %d\n", length);
	printf("Offset: %d\n", length);	
	*/
	lucene::document::Document doc;
	for(int i = offset; i <= length; i++){
		json_t* object = json_object();
		
	
		doc = h->doc(i);
		
		
		lucene::document::DocumentFieldEnumeration* tfields;
		
		lucene::document::Field* tfield;
		
		tfields = doc.fields();
		while( (tfield = tfields->nextElement()) ){

			char* fieldname = new char[wcslen(tfield->name())+1];
			char* fieldvalue = new char[wcslen(tfield->stringValue())+1];
			json_t* value;
			
			
			this->toChar(tfield->name(), fieldname);
			if(strcmp(fieldname, "_timestamp") == 0){
				int64_t timestamp = lucene::document::DateTools::stringToTime(tfield->stringValue());
				// timestamp = timestamp;
				value = json_integer(timestamp / 1000);
/*
				this->toChar(tfield->stringValue(), fieldvalue);
				value = json_string((char*) fieldvalue);
*/				
			} else {
				this->toChar(tfield->stringValue(), fieldvalue);
				value = json_string((char*) fieldvalue);
			}
			json_object_set(object, fieldname, value);
			
			delete[] fieldname;
			delete[] fieldvalue;

		}
		json_array_append(json, object);

	}
	
	delete[] wquery;
	delete[] wfield;

	// std::cout << json_dumps(json, JSON_ENCODE_ANY);
	return h->length();
	
}

void logcollect::CluceneIndexReader::makeTCHAR(const char* input, TCHAR* output){
	#if defined(_UCS2)
		mbstowcs(output, input, strlen(input)+1);
	#else
		wcscpy(output, input);
	#endif
}

void logcollect::CluceneIndexReader::toChar(const TCHAR* input, char* output){
	#if defined(_UCS2)
		wcstombs(output, input, wcslen(input)+1);
	#else
		strcpy(output, input);
	#endif
}

logcollect::CluceneIndexReader::~CluceneIndexReader(){
	/*
	delete this->reader;
	delete this->analyzer;
	delete this->seacher;
	 */
}
