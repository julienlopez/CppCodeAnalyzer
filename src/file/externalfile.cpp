#include "externalfile.hpp"

ExternalFile::ExternalFile(const std::string& filePath_): CRTP_File<ExternalFile>(filePath_)
{}

std::string ExternalFile::type()
{
	return std::string("External");
}

bool ExternalFile::impl_isModifiable() const
{
	return false;
}