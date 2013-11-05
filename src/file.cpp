#include "file.hpp"

#include <utils/stringhelper.hpp>

#include <algorithm>
#include <fstream>
#include <stdexcept>

File::File(const std::string& filePath)
{
	std::ifstream file(filePath);
	if(!file)
		throw std::invalid_argument("Unable to open file " + filePath);
	std::string line;
	bool inMultiLineComment = false;
	while(std::getline(file, line))
	{
		Line::Type type = Line::Type::Other;
		if(inMultiLineComment) 
		{
			type = Line::Type::Comment;
			if(StringHelper::contains(line, "*/"))
				inMultiLineComment = false;
		} else 
		{
			if(StringHelper::startsWith(line, "#"))
				type = Line::Type::Preprocessor;
			else if(StringHelper::startsWith(line, "//"))
				type = Line::Type::Comment;
			else if(StringHelper::contains(line, "/*"))
			{
				type = Line::Type::Comment;
				inMultiLineComment = true;
			}
		}
		m_lines.emplace_back(type, line);
	}
}

std::size_t File::count() const
{
	return m_lines.size();
}

std::size_t File::count(Line::Type type) const
{
	return std::count_if(begin(m_lines), end(m_lines), [&type](const Line& line) { return line.type() == type; });
}

Line File::at(std::size_t pos)
{
	return m_lines.at(pos);
}