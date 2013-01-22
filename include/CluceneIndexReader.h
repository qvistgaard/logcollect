//
//  CluceneIndexReader.h
//  logcollect
//
//  Created by Steffen Sørensen on 1/21/13.
//  Copyright (c) 2013 Steffen Sørensen. All rights reserved.
//

#ifndef logcollect_CluceneIndexReader_h
#define logcollect_CluceneIndexReader_h

#include <CLucene.h>
#include <jansson.h>

namespace logcollect {
	class CluceneIndexReader {
	private:
		lucene::index::IndexReader* reader;
		lucene::analysis::Analyzer* analyzer;
		lucene::search::Searcher* searcher;
		
	protected:
		void makeTCHAR(const char* input, TCHAR* output);
		void toChar(const TCHAR* input, char* output);

	public:
		CluceneIndexReader(const char* index);
		int query(json_t* json, const char* query, const char* field, int limit=30, int offset=0);
		~CluceneIndexReader();
	};
}


#endif
