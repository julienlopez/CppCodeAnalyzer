#ifndef __FILEFACTORY_HPP__
#define __FILEFACTORY_HPP__

#include <utils/noninstanciable.hpp>
#include <utils/abstractfactory.hpp>

#include <memory>

class iFile;

class FileFactory : public utils::noninstanciable
{
public:

	static const std::string Modifiable;
	static const std::string External;

	typedef utils::AbstractFactory<iFile, std::string, const std::string&, iFile*(*)(const std::string&)> type_factory;

	static std::unique_ptr<iFile> createFile(const std::string& type, const std::string& path);
};

#endif