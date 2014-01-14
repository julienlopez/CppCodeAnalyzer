#ifndef __EXTERNALFILE_HPP__
#define __EXTERNALFILE_HPP__

#include <file/crtp_file.hpp>

class ExternalFile : public CRTP_File<ExternalFile>
{
public:
	
	ExternalFile(const std::string& filePath_);

	static std::string type();

	virtual bool impl_isModifiable() const final override;
};

#endif