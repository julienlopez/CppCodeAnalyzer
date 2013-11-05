#include "line.hpp"

Line::Line(Type type_, std::string content_): m_type(type_), m_content(std::move(content_))
{}

Line::Type Line::type() const
{
	return m_type;
}

std::string Line::content() const
{
	return m_content;
}