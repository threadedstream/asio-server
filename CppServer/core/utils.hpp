#pragma once

#include "typedefs.hpp"

struct utils {

	/*
		@Splits the only string into two distinct ones, with the first 
		being header related and the other one being dedicated to a response data.
	*/
	static std::pair<std::string, std::string> split_response(const std::string& resp) {
		std::pair<std::string, std::string> pair;
		
		auto found_end_header = resp.find_last_of("\r\n\r\n");
		
		pair.first = resp.substr(0, found_end_header);
		pair.second = resp.substr(found_end_header);
		
		return pair;
	}

	static std::string prettify_json(std::string& json) {
		std::string pretty;
		
		json[0] = '\n';
		json[json.length() - 1] = '\n';

		auto jsons = prettify(json);

		for (const auto json : jsons) {
			pretty += json;
			pretty += "<br>";
		}

		return pretty;
	}

private:

	/*
		@The json response from the server is of the following form:
		{"field1":"value1","field2":"value2", "field3":"value3"},{"field4":"value4","field5":"value5","field6":"value6"};
		
		My intention boils down to prettifying this response and transforming it into the following:
		{
			"field1" : "value1"
			"field2" : "value2"
			"field3" : "value3"
		}
		{
			"field1" : "value1"
			"field2" : "value2"
			"field3" : "value3"
		}

	*/
	static std::vector<std::string>
	prettify(std::string& str) {
		std::vector<std::string> out;
		std::string s;
		for (int i = 0; i < str.length(); ++i) {
			if (str[i] == '{') {
				s += str[i];
				s += "<br>";
				s += "<div style=\"margin-left:30px;\">";
				++i;
				while (str[i] != '}') {
					if (str[i] == ',') {
						s += "<br>";
						++i;
					}
					s += str[i];
					++i;
				}
				s += "</div><br>";
				s += str[i];
				s += "<br>";
				out.emplace_back(s);
				s.clear();
				++i;
			}
		}
		return out;
	}
	
};