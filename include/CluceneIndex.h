//
//  CluceneIndex.h
//  logcollectd
//
//  Created by Steffen Sørensen on 1/3/13.
//
//

#ifndef logcollectd_CluceneIndex_h
#define logcollectd_CluceneIndex_h
#include <string>
#include <CLucene.h>
#include "Result.h"
#include "DateConversion.h"



namespace logcollect {
	class CluceneIndex {
	private:
		lucene::index::IndexWriter* writer;
		lucene::analysis::Analyzer* analyzer;
		lucene::document::Document* document;
		int indexed;
		

	public:
		CluceneIndex(const std::string index);
		~CluceneIndex();
		void index(Result *r, DateConversion* converter);
	};
}

#endif
