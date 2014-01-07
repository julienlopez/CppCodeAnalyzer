#include "file.hpp"

#include <utils/stringhelper.hpp>

#include <fstream>
#include <stdexcept>

File::File(const std::string& filePath_): m_filePath(filePath_)
{
	std::ifstream file(filePath_);
	if(!file)
		throw std::invalid_argument("Unable to open file " + filePath_);
	std::string line;
	bool inMultiLineComment = false;
	while(std::getline(file, line))
	{
		Line::Type type = Line::Type::Other;
		if(line.empty())
		{
			type = Line::Type::Empty;
		}
		else if(inMultiLineComment) 
		{
			type = Line::Type::Comment;
			if(StringHelper::contains(line, "*/"))
				inMultiLineComment = false;
		} 
		else 
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
	return std::count_if(std::begin(m_lines), std::end(m_lines), Line::ComparatorByType(&Line::type, type));
}

Line File::at(std::size_t pos)
{
	return m_lines.at(pos);
}

const File::type_path& File::filePath() const
{
	return m_filePath;
}

File::iterator File::begin()
{
	return m_lines.begin();
}

File::iterator File::end()
{
	return m_lines.end();
}

void File::print(std::ostream& o) const
{
	for(const auto& line : m_lines)
		o << line.content() << "\n";
}

void File::print() const
{
	std::ofstream f(m_filePath.generic_string());
	assert(f.is_open());
	print(f);
}