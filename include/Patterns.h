//
//  FileRules.h
//
//
//  Created by Steffen Sørensen on 10/2/12.
//
//


#ifndef _LOGCOLLECT_PATTERNS
#define _LOGCOLLECT_PATTERNS

#include <string>
#include <map>
#include "Logger.h"
#include "Pattern.h"
#include "Result.h"

namespace logcollect {
    typedef std::map<std::string, Pattern*> pattern_map;
    typedef std::pair<std::string, Pattern*> pattern_pair;


    class Patterns {
    private:
		Logger* log;
		
        pattern_map patterns;
        
		bool _expand();
    
    public:
        Patterns(Logger *log);
        Pattern* getPattern(const std::string &name);
        bool addPattern(Pattern *pattern);
		Result* match(std::string pattern_name, std::string data, DateConversion* converter);
    };
}

#endif
