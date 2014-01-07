#ifndef __LINE_HPP__
#define __LINE_HPP__

#include <string>

#include <utils/comparatorbymethodvalue.hpp>

class Line
{
public:
	enum class Type {Comment, Preprocessor, Empty, ForwardDeclaration, Other};

	Line(Type type_, std::string content_);

	Type type() const;
	std::string content() const;

	bool isAnIncludeStatement() const;

	using ComparatorByType = utils::comparator_by_method_value<Line, Line::Type>;

private:
	Type m_type;
	std::string m_content;
};

bool operator<(const Line& l1, const Line& l2);

#endif