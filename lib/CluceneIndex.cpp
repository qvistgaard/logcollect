

#include <string>
#include <iostream>
#include <CLucene.h>
#include "../include/CluceneIndex.h"
#include "../include/Result.h"
#include "../include/DateConversion.h"


logcollect::CluceneIndex::CluceneIndex(const std::string index){
	this->analyzer = new lucene::analysis::standard::StandardAnalyzer();
	this->document = new lucene::document::Document();

	if(lucene::index::IndexReader::indexExists(index.c_str())){
		this->writer = new lucene::index::IndexWriter(index.c_str(), this->analyzer, false);
	} else {
		this->writer = new lucene::index::IndexWriter(index.c_str(), this->analyzer, true);
	}
	this->writer->setMaxFieldLength(0x7FFFFFFFL);
	this->writer->setUseCompoundFile(false);
	this->writer->setMaxBufferedDocs(5000000);
//	this->writer->setMinMergeDocs(5000000);
	
}

void logcollect::CluceneIndex::index(Result *r){
	this->index(r, nullptr);
}

void logcollect::CluceneIndex::index(Result *r, DateConversion* converter){
	
	// Convert normal strings into wstrings for use with lucene::document::Field
	const std::string *str_fielddata = r->getData();
	std::wstring fieldname = L"logline";
	std::wstring fielddata;
	fielddata.assign(str_fielddata->begin(), str_fielddata->end());

	// Add entire logline as field logline
	lucene::document::Field *field = new lucene::document::Field(fieldname.c_str(), fielddata.c_str(), lucene::document::Field::STORE_YES | lucene::document::Field::INDEX_TOKENIZED | lucene::document::Field::STORE_COMPRESS /* | lucene::document::Field::INDEX_NONORMS */ | lucene::document::Field::TERMVECTOR_NO );
	this->document->add(*field);

	// Add field for each result
	logcollect::result_map* fields = r->getFields();
    logcollect::result_map::iterator it;

	std::wstring name, value;

	
	TCHAR* timestamp = lucene::document::DateTools::timeToString((r->getTimestamp() * 1000), lucene::document::DateTools::MILLISECOND_FORMAT );
	field = new lucene::document::Field(L"_timestamp", timestamp, lucene::document::Field::STORE_YES | lucene::document::Field::INDEX_UNTOKENIZED | lucene::document::Field::INDEX_NONORMS | lucene::document::Field::TERMVECTOR_NO );
	this->document->add(*field);	
	
	
//	lucene::document::Field *field;
	for(it = fields->begin(); it != fields->end(); it++){
		
		name.assign(it->first.begin(), it->first.end());
		value.assign(it->second.begin(), it->second.end());

		field = new lucene::document::Field(name.c_str(), value.c_str(), lucene::document::Field::STORE_YES | lucene::document::Field::INDEX_TOKENIZED  | lucene::document::Field::INDEX_NONORMS | lucene::document::Field::TERMVECTOR_NO );

		this->document->add(*field);
	}
	 
	this->writer->addDocument(this->document);
	this->document->clear();
	
	if(this->indexed >= 1000){
 		this->writer->optimize();
		this->indexed = 0;
	} else {
		this->indexed++;
	}
}

logcollect::CluceneIndex::~CluceneIndex(){
	this->writer->optimize();
	this->writer->close();
}
