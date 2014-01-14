#include "filefactory.hpp"
#include "ifile.hpp"

const std::string FileFactory::Modifiable = "Modifiable";
const std::string FileFactory::External = "External";

std::unique_ptr<iFile> FileFactory::createFile(const std::string& type, const std::string& path)
{
	return std::unique_ptr<iFile>(type_factory::instance().create(type, path));
}