#ifndef __FILE_HPP__
#define __FILE_HPP__

#include <file/crtp_file.hpp>

#include <boost/filesystem/path.hpp>

class ModifiableFile : public CRTP_File<ModifiableFile>
{
public:
	using type_path = boost::filesystem::path;

	/**
	* \brief opens and parses the given file.
	* \throw std::invalid_argument if unable to open the given file.
	*/
	ModifiableFile(const std::string& filePath_);

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

	static std::string type();

	virtual bool impl_isModifiable() const final override;

	virtual type_container impl_getLinesByType(Line::Type type) const final override;

private:
	type_path m_filePath;
	type_container m_lines;

	// virtual ~ModifiableFile() = default;
};

#endif