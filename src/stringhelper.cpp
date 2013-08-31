#include "stringhelper.hpp"

#include <iostream>
using namespace std;

bool StringHelper::startsWith(const std::string& str, const std::string& token)
{
	if(str.length() < token.length()) return false;
	bool res = str.find(token) == 0;
	return res;
}

bool StringHelper::endsWith(const std::string& str, const std::string& token)
{
	if(str.length() < token.length()) return false;
	bool res = str.rfind(token) == str.length() - token.length();
	return res;
}

std::string StringHelper::join(const type_vector_string& vector, const std::string& joint)
{
	std::string res;
	if(vector.empty()) return res;
	type_vector_string::const_iterator i = vector.begin();
	res = *i++;
	for(; i != vector.end(); ++i)
		res += joint + *i;
	return res;
}
