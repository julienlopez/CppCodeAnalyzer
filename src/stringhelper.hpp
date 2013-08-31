#ifndef __STRINGHELPER_HPP__
#define __STRINGHELPER_HPP__

#include <string>
#include <vector>

class StringHelper
{
	public:
		static bool startsWith(const std::string& str, const std::string& token);
		static bool endsWith(const std::string& str, const std::string& token);

		typedef std::vector<std::string> type_vector_string;

		static std::string join(const type_vector_string& vector, const std::string& joint = " ");
};

#endif // __STRINGHELPER_HPP__
