#ifndef __FILE_HPP__
#define __FILE_HPP__

#include <line.hpp>

#include <iterator>
#include <vector>

#include <boost/filesystem/path.hpp>

class File
{
public:
	using type_path = boost::filesystem::path;

	using type_container = std::vector<Line>;
	using iterator = type_container::iterator;
	using const_iterator = type_container::const_iterator;

	/**
	* \brief opens and parses the given file.
	* \throw std::invalid_argument if unable to open the given file.
	*/
	File(const std::string& filePath_);

	std::size_t count() const;

	std::size_t count(Line::Type type) const;

	Line at(std::size_t pos);

	const type_path& filePath() const;

	iterator begin();
	iterator end();

	iterator first_of(Line::Type type);

	template<class PRED>
	iterator first_of(PRED predicat)
	{
		return std::find_if(std::begin(m_lines), std::end(m_lines), predicat);
	}

	template<class PRED>
	iterator last_of(PRED predicat)
	{
		return std::find_if(m_lines.rbegin(), m_lines.rend(), predicat).base();
	}

	void print(std::ostream& o) const;

	void print() const;

private:
	type_path m_filePath;
	type_container m_lines;
};

#endif