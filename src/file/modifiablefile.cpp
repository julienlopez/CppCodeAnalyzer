#include "modifiablefile.hpp"

#include <utils/stringhelper.hpp>

#include <fstream>
#include <stdexcept>

ModifiableFile::ModifiableFile(const std::string& filePath_): CRTP_File<ModifiableFile>(filePath_), m_filePath(filePath_)
{
	std::ifstream file(filePath_);
	if(!file)
		throw std::invalid_argument("Unable to open file " + filePath_);
	std::string line;
	bool inMultiLineComment = false;
	while(std::getline(file, line))
	{
		Line::Type lineType = Line::Type::Other;
		if(line.empty())
		{
			lineType = Line::Type::Empty;
		}
		else if(inMultiLineComment) 
		{
			lineType = Line::Type::Comment;
			if(StringHelper::contains(line, "*/"))
				inMultiLineComment = false;
		} 
		else 
		{
			if(StringHelper::startsWith(line, "#"))
				lineType = Line::Type::Preprocessor;
			else if(StringHelper::startsWith(line, "//"))
				lineType = Line::Type::Comment;
			else if(StringHelper::contains(line, "/*"))
			{
				lineType = Line::Type::Comment;
				inMultiLineComment = true;
			}
		}
		m_lines.emplace_back(lineType, line);
	}
}

std::size_t ModifiableFile::count() const
{
	return m_lines.size();
}

std::size_t ModifiableFile::count(Line::Type lineType) const
{
	return std::count_if(std::begin(m_lines), std::end(m_lines), Line::ComparatorByType(&Line::type, lineType));
}

Line ModifiableFile::at(std::size_t pos)
{
	return m_lines.at(pos);
}

const ModifiableFile::type_path& ModifiableFile::filePath() const
{
	return m_filePath;
}

ModifiableFile::iterator ModifiableFile::begin()
{
	return m_lines.begin();
}

ModifiableFile::iterator ModifiableFile::end()
{
	return m_lines.end();
}

void ModifiableFile::print(std::ostream& o) const
{
	for(const auto& line : m_lines)
		o << line.content() << "\n";
}

void ModifiableFile::print() const
{
	std::ofstream f(m_filePath.generic_string());
	assert(f.is_open());
	print(f);
}

std::string ModifiableFile::type()
{
	return  "Modifiable";
}

bool ModifiableFile::impl_isModifiable() const
{
	return true;
}