#ifndef __FILE_HPP__
#define __FILE_HPP__

#include <line.hpp>

#include <vector>

class File
{
public:
	/**
	* \brief opens and parses the given file.
	* \throw std::invalid_argument if unable to open the given file.
	*/
	File(const std::string& filePath);

	std::size_t count() const;

	std::size_t count(Line::Type type) const;

	Line at(std::size_t pos);

private:
	using type_container = std::vector<Line>;
	type_container m_lines;
};

#endif