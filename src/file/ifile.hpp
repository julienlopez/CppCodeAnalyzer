#ifndef __IFILE_HPP__
#define __IFILE_HPP__

#include <file/line.hpp>

#include <vector>
#include <string>

class iFile
{
public:
	using type_container = std::vector<Line>;
	using iterator = type_container::iterator;
	using const_iterator = type_container::const_iterator;

	virtual ~iFile() = default;

	bool isRegistered() const;

	bool isModifiable() const;

	const std::string& filePath() const;

	type_container getLinesByType(Line::Type type) const;

protected:
	iFile() = default;

private:

	virtual bool impl_isRegistered() const = 0;

	virtual const std::string& impl_filePath() const = 0;

	virtual bool impl_isModifiable() const = 0;

	virtual type_container impl_getLinesByType(Line::Type type) const = 0;
};

#endif