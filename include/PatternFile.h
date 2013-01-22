//
//  FileRules.h
//  
//
//  Created by Steffen Sørensen on 10/2/12.
//
//

#include <string>
#include <fstream>
#include "Patterns.h"
#include "Logger.h"

#ifndef _LOGCOLLECT_PATTERN_FILE
#define _LOGCOLLECT_PATTERN_FILE

namespace logcollect {

    class PatternFile {
    private:
        std::ifstream stream;
        Logger* log;
        
        Pattern* _buildPattern(const char *line);
        
    public:
        PatternFile (Logger *log, const std::string &filename);
        void addPatterns(Patterns *patterns);
    };

}

#endif
