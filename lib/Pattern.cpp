//
//  Pattern.cpp
//  
//
//  Created by Steffen Sørensen on 10/3/12.
//
//
#include <string>
#include <iostream>
#include <pcre.h>
#include <CLucene.h>
#include "../include/Pattern.h"
#include "../include/Patterns.h"
#include "../include/Result.h"


logcollect::Pattern::Pattern(const std::string &name, const std::string &pattern){
	this->name = name;
	this->pattern = pattern.c_str();
	this->expression = NULL;
}

const std::string* logcollect::Pattern::getName(){
	return &this->name;
}

std::string* logcollect::Pattern::getPattern(){
	return &this->pattern;
}

logcollect::Result* logcollect::Pattern::match(const std::string data){

	if(!this->expression){
		const char *error;
		int erroffset;
		this->expression = pcre_compile(this->pattern.c_str(), 0, &error, &erroffset, NULL);
		
		// Analyze expression
		const char *error_extra;
		int namecount, namesize;
		this->expression_extra = pcre_study(this->expression, 0, &error_extra);
		pcre_fullinfo(this->expression, this->expression_extra, PCRE_INFO_NAMECOUNT, &namecount);
		pcre_fullinfo(this->expression, this->expression_extra, PCRE_INFO_NAMEENTRYSIZE, &namesize);
		
		// Fetch named subpatterns
		char *names;
		pcre_fullinfo(this->expression, this->expression_extra, PCRE_INFO_NAMETABLE, &names);
		

		for (int i = 0; i < namecount; i++) {
			
			int start = (i * namesize);
			int end = start + namesize;
			int num;
			std::string subpattern = "";
			
			for (int ni = start; ni < end; ni++) {
				if(ni % namesize == 0){
				} else if(ni % namesize == 1){
					num = names[ni];
				} else if(names[ni]){
					subpattern += names[ni];
				} else {
					break;
				}
			}
			this->subpatterns.insert(subpattern_pair(subpattern, num));
		}
		
		// Create key=value pair matching pattern
		this->value_expression = pcre_compile("(\\S+)=(\")?(\\S+)(\")?(\\s|,|&)", 0, &error, &erroffset, NULL);
		this->value_expression_extra = pcre_study(this->value_expression, 0, &error_extra);
	}


	int ovector[60];
	int pcre_match = pcre_exec(this->expression, this->expression_extra, data.c_str(), (int) data.length(), 0, 0, ovector, 60);
	if(pcre_match > 0){
		Result *r = new Result(data);
		
		logcollect::subpattern_map::iterator it;
		for(it = this->subpatterns.begin(); it != this->subpatterns.end(); it++){
			const char *sub[1024];
			pcre_get_substring(data.c_str(), ovector, 20, it->second, sub);
			
			if(strlen(*sub) > 0){
				r->add(it->first, *sub);
			}
		}
		
		/*
		int ovector2[60];
		int pcre_match = pcre_exec(this->value_expression, this->value_expression_extra, data.c_str(), (int) data.length(), 0, 0, ovector2, 60);
		*/
		
		return r;
	}
	return NULL;
}

bool logcollect::Pattern::expand(Patterns *patterns){
	while(this->_expander(patterns)){ }
	return true;
}

bool logcollect::Pattern::_expander(Patterns *patterns){
	
	const char *error;
	int erroffset;
	std::string pattern = "(%\{(.*?)(:(.*?))?})";
	pcre* match = pcre_compile(pattern.c_str(), 0, &error, &erroffset, NULL);
	
	
	int ovector[60];
	
	
	int pcre_match = pcre_exec(match, NULL, this->pattern.c_str(), (int) this->pattern.length(), 0, 0, ovector, 60);
	
	if(pcre_match > 0){
		const char *sub[1024];
		const char *sub_name[1024];
		
		
		pcre_get_substring(this->pattern.c_str(), ovector, 20, 0, sub);
		pcre_get_substring(this->pattern.c_str(), ovector, 20, 2, sub_name);
		
		std::string name = *sub_name;
		std::string prefix = this->pattern.substr(0, this->pattern.find(*sub));
		std::string suffix = this->pattern.substr(this->pattern.find(*sub)+strlen(*sub));
		
		Pattern *pattern = patterns->getPattern(name);
		
		if(pattern != 0){
			if(pcre_match == 5){
				const char *field[1024];
				pcre_get_substring(this->pattern.c_str(), ovector, 20, 4, field);
				this->pattern = prefix + "(?P<" + (*field) + ">" + (*pattern->getPattern()) + ")" + suffix;
			} else {
				this->pattern = prefix + (*pattern->getPattern()) + suffix;
			}
		} else {
			throw this->pattern;
		}
		pcre_free(match);
		return true;
	} else {
		pcre_free(match);
		return false;
	}
}

