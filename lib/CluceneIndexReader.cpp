//
//  CluceneIndexReader.cpp
//  logcollect
//
//  Created by Steffen Sørensen on 1/21/13.
//  Copyright (c) 2013 Steffen Sørensen. All rights reserved.
//
#include <iostream>
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

int logcollect::CluceneIndexReader::query(json_t* json, const char* query, const char* field, int limit, int offset){
	
	TCHAR wquery[strlen(query)];
	TCHAR wfield[strlen(field)];
	

 	this->makeTCHAR(query, wquery);
	this->makeTCHAR(field, wfield);
	
	
	lucene::search::Query* q = lucene::queryParser::QueryParser::parse(wquery, wfield, this->analyzer);
	lucene::search::Hits* h = this->searcher->search(q);

	
	int length = h->length() - offset - 1;
	if(length > limit){
		length = limit;
	}
	
	/*
	printf("Lenght: %d\n", h->length());
	printf("Limit: %d\n", length);
	printf("Offset: %d\n", length);	
	 */
	lucene::document::Document doc;
	for(int i = 0; i < length; i++){
		json_t* object = json_object();
		
	
		doc = h->doc(offset);
		
		
		lucene::document::DocumentFieldEnumeration* tfields;
		lucene::document::Field* tfield;
		
		tfields = doc.fields();
		while( (tfield = tfields->nextElement()) ){

			
			
			char fieldname[wcslen(tfield->name())+1];
			char fieldvalue[wcslen(tfield->stringValue())+1];
			

			this->toChar(tfield->name(), fieldname);
			this->toChar(tfield->stringValue(), fieldvalue);
			
	

			
			json_t* value = json_string((char*) fieldvalue);
			json_object_set(object, fieldname, value);
		}
		json_array_append(json, object);

	}
	
	
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
