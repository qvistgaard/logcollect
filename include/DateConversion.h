//
//  DateConversion.h
//  logcollect
//
//  Created by Steffen Sørensen on 1/23/13.
//  Copyright (c) 2013 Steffen Sørensen. All rights reserved.
//


#include <vector>
#include <string>

#ifndef logcollect_DateConversion_h
#define logcollect_DateConversion_h


namespace logcollect {
	class DateConversion {
	private:
		std::vector<const std::string*> formats;
		
	public:
		DateConversion();
		bool addFormat(std::string* format);
		bool addFormat(const char* format);
		time_t getTime(const std::string* str_time);
		time_t getTime(const char* str_time);
	};
}

#endif
