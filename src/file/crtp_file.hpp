#ifndef __CRTP_FILE_HPP__
#define __CRTP_FILE_HPP__

#include "ifile.hpp"
#include "filefactory.hpp"

template<class T> class CRTP_File : public iFile
{
protected:
    template<typename... Args> CRTP_File(const std::string& filePath_, Args&&... args): iFile(std::forward<Args>(args)...), m_filePath(filePath_)
    {}

public:
	static const bool s_isRegistered;

	virtual bool impl_isRegistered() const override final
	{
		return s_isRegistered;
	}

private:
	std::string m_filePath;

	const std::string& impl_filePath() const override final
	{
		return m_filePath;
	}
};

namespace 
{
	template<class T> iFile* createFile(const std::string& type)
	{ 
		return new T(type); 
	}
}

template<class T> const bool CRTP_File<T>::s_isRegistered = FileFactory::type_factory::instance().registerProduct(T::type(), &createFile<T>);

#endif