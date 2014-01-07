#include "line.hpp"

#include <utils/stringhelper.hpp>

#include <cassert>

Line::Line(Type type_, std::string content_): m_type(type_), m_content(std::move(content_))
{
	if(isAnIncludeStatement())
	{
		assert(m_content.size() > 8);
		if(!std::isblank(m_content[8]))
			m_content.insert(8, " ");
	}
}

Line::Type Line::type() const
{
	return m_type;
}

std::string Line::content() const
{
	return m_content;
}

bool Line::isAnIncludeStatement() const
{
	return m_type == Type::Preprocessor && StringHelper::startsWith(m_content, "#include");
}

bool operator<(const Line& l1, const Line& l2)
{
	return l1.content() < l2.content();
}