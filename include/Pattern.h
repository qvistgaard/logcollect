//
//  Pattern.h
//  
//
//  Created by Steffen Sørensen on 10/2/12.
//
//

#ifndef _LOGCOLLECT_PATTERN
#define _LOGCOLLECT_PATTERN
#include <string>
#include <pcre.h>
#include <map>
#include "Result.h"



namespace logcollect {
	typedef std::map<std::string, int> subpattern_map;
	typedef std::pair<std::string, int> subpattern_pair;
	
	class Patterns;
	
	class Pattern {
	private:
		std::string name;
		std::string pattern;
		pcre* expression;
		pcre_extra* expression_extra;
		pcre* value_expression;
		pcre_extra* value_expression_extra;
		subpattern_map subpatterns;
		
		
		bool _expander(Patterns *patterns);
    
	public:
		Pattern(const std::string &name, const std::string &pattern);
    
		const std::string* getName();
		std::string* getPattern();
		bool expand(Patterns *patterns);
		Result* match(const std::string data);
	};
}

#endif
