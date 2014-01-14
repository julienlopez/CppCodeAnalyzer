#include "ifile.hpp"

bool iFile::isRegistered() const
{
	return impl_isRegistered();
}

bool iFile::isModifiable() const
{
	return impl_isModifiable();
}

const std::string& iFile::filePath() const
{
	return impl_filePath();
}

iFile::type_container iFile::getLinesByType(Line::Type type) const
{
	return impl_getLinesByType(type);
}