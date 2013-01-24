

#include <string>
#include <iostream>
#include <CLucene.h>
#include "../include/CluceneIndex.h"
#include "../include/Result.h"
#include "../include/DateConversion.h"


logcollect::CluceneIndex::CluceneIndex(const std::string index){
	this->analyzer = new lucene::analysis::SimpleAnalyzer();
	this->document = new lucene::document::Document();

	if(lucene::index::IndexReader::indexExists(index.c_str())){
		this->writer = new lucene::index::IndexWriter(index.c_str(), this->analyzer, false);
	} else {
		this->writer = new lucene::index::IndexWriter(index.c_str(), this->analyzer, true);
	}
	this->writer->setMaxFieldLength(0x7FFFFFFFL);
}

void logcollect::CluceneIndex::index(Result *r, DateConversion* converter){
	
	// Convert normal strings into wstrings for use with lucene::document::Field
	const std::string *str_fielddata = r->getData();
	std::wstring fieldname = L"logline";
	std::wstring fielddata;
	fielddata.assign(str_fielddata->begin(), str_fielddata->end());

	// Add entire logline as field logline
	lucene::document::Field *field = new lucene::document::Field(fieldname.c_str(), fielddata.c_str(), lucene::document::Field::STORE_YES | lucene::document::Field::INDEX_TOKENIZED);
	this->document->add(*field);

	// Add field for each result
	logcollect::result_map* fields = r->getFields();
    logcollect::result_map::iterator it;

	std::wstring name, value;

	
	for(it = fields->begin(); it != fields->end(); it++){
		
		name.assign(it->first.begin(), it->first.end());
		value.assign(it->second.begin(), it->second.end());
		lucene::document::Field *field;
		
		if(name.compare(L"timestamp") == 0){
			
			
			
			std::string str_timestamp = it->second;
//			std::cout << "'" << it->second << "'" << std::endl;
//
			time_t int_timestamp = converter->getTime(&it->second) * 1000;
			
//			std::cout << int_timestamp << std::endl;
			
			
			TCHAR* timestamp = lucene::document::DateTools::timeToString(int_timestamp, lucene::document::DateTools::MILLISECOND_FORMAT );
//			std::wcout << timestamp << std::endl;
			
			field = new lucene::document::Field(name.c_str(), timestamp, lucene::document::Field::STORE_YES | lucene::document::Field::INDEX_TOKENIZED );
		} else {
			field = new lucene::document::Field(name.c_str(), value.c_str(), lucene::document::Field::STORE_YES | lucene::document::Field::INDEX_TOKENIZED );
		}
		this->document->add(*field);
	}
	

	
	this->writer->addDocument(this->document);
	
	/*
	lucene::document::DocumentFieldEnumeration* tfields;
	lucene::document::Field* tfield;
	
	tfields = this->document->fields();
	
	while(tfield = tfields->nextElement()){
		const TCHAR* fname = tfield->name();
		const TCHAR* value = tfield->stringValue();
		std::wcout << fname << ": "<< tfield->toString() << std::endl;
	}
	 */

	this->document->clear();
	
	if(this->indexed > 100000){
		//this->writer->optimize();
		this->indexed = 0;
	} else {
		this->indexed++;
	}
}

logcollect::CluceneIndex::~CluceneIndex(){
	this->writer->optimize();
	this->writer->close();
}
