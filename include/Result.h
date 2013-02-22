//
//  result.h
//  logcollectd
//
//  Created by Steffen Sørensen on 12/23/12.
//
//

#ifndef _LOGCOLLECT_RESULT
#define _LOGCOLLECT_RESULT

#include <map>
#include <string>


namespace logcollect {
    typedef std::map<std::string, std::string> result_map;
    typedef std::pair<std::string, std::string> result_pair;
	
	
    class Result {
    private:
        result_map fields;
		std::string data;
		time_t timestamp;
		
    public:
		Result(std::string data);
		void add(std::string field, std::string value);
		void setTimestamp(time_t timestamp);
		time_t getTimestamp();
		void dump();
		result_map* getFields();
		const std::string* getData();
		~Result();
    };
}

#endif
