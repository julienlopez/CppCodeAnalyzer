#ifndef __LINE_HPP__
#define __LINE_HPP__

#include <string>

class Line
{
public:
	enum class Type {Comment, Preprocessor, Other};

	Line(Type type_, std::string content_);

	Type type() const;
	std::string content() const;

private:
	Type m_type;
	std::string m_content;
};

#endif