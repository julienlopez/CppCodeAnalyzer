#include <utils/stringhelper.hpp>

#include <boost/filesystem.hpp>

#include <fstream>

using namespace std;

StringHelper::type_vector_string file2vector(std::ifstream& input)
{
	StringHelper::type_vector_string res;
	while(input)
	{
		std::string line;
		std::getline(input, line);
		res.push_back(line);
	}
	return res;
}

StringHelper::type_vector_string convert(const StringHelper::type_vector_string& block)
{
	assert(StringHelper::contains(block.front(), "<tr>"));
	assert(StringHelper::contains(block.back(), "</tr>"));

	std::string line = block[1];
	if(!StringHelper::contains(line, "<td class=\"coverFile\">"))
		return block;

	std::size_t pos = line.find("<a href=");
	assert(pos != std::string::npos);
	pos = line.find(">", pos+1);
	assert(pos != std::string::npos);
	std::size_t endPos = line.find("</a>", pos+1);
	assert(endPos != std::string::npos);
	std::string path = line.substr(pos+1, endPos-pos-1);

	if(!StringHelper::contains(path, "CppCodeAnalyzer/src")) return StringHelper::type_vector_string();

	return block;
}

StringHelper::type_vector_string clean(const StringHelper::type_vector_string& inLines)
{
	StringHelper::type_vector_string res;

	std::size_t pos = 0;
	while(pos < inLines.size() && !StringHelper::contains(inLines[pos], "<center>")) res.push_back(inLines[pos++]);
	assert(pos < inLines.size());

	while(true)
	{
		while(pos < inLines.size() && !StringHelper::contains(inLines[pos], "<tr>")) res.push_back(inLines[pos++]);

		StringHelper::type_vector_string block;
		while(pos < inLines.size() && StringHelper::trim(inLines[pos]) != "</tr>") block.push_back(inLines[pos++]);
		block.push_back(inLines[pos++]);

		StringHelper::type_vector_string newBlock = convert(block);
		std::copy(newBlock.begin(), newBlock.end(), std::back_inserter(res));

		if(StringHelper::contains(inLines[pos], "</table>")) break;
	}

	while(pos < inLines.size()) res.push_back(inLines[pos++]);
	return res;
}

int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		cerr << "You must specify a file" << endl;
		return EXIT_FAILURE;
	}
	boost::filesystem::path p(argv[1]);
	std::ifstream input(argv[1]);
	if(!input)
	{
		cerr << "unable to open file: " << argv[1] << endl;
		return EXIT_FAILURE;
	}
	StringHelper::type_vector_string inLines = file2vector(input);

	StringHelper::type_vector_string outLines = clean(inLines);

	std::ofstream out(argv[1], ios_base::out | ios_base::trunc);
	for(const auto line : outLines)
		out << line << endl;

	return EXIT_SUCCESS;
}