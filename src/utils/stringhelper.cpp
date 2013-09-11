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

bool StringHelper::contains(const std::string& str, const std::string& token)
{
	return str.find(token) != std::string::npos;
}

std::string StringHelper::trim(std::string str)
{
	while(isspace(str.front())) str.erase(0, 1);
	while(isspace(str.back())) str.erase(str.size() -1, 1);
	return str;
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
