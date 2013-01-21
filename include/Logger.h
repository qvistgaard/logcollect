//
//  FileRules.h
//
//
//  Created by Steffen Sørensen on 10/2/12.
//
//
#include <string>
#include <map>
#include "Pattern.h"

#ifndef _LOGCOLLECT_LOGGER
#define _LOGCOLLECT_LOGGER

namespace logcollect {
    
    class Logger {
    public:
        void critical(const std::string &logline);
        void error(const std::string &logline);
        void warning(const std::string &logline);
        void notice(const std::string &logline);
        void info(const std::string &logline);
        void debug(const std::string &logline);
        
    private:
        void _write(const std::string &level, const std::string &logline);
    };
    
};

#endif
