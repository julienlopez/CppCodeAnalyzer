#include "headersorter.hpp"

#include <file/modifiablefile.hpp>

#include <utils/algorithms.hpp>

const std::string HeaderSorter::s_name = "HeaderSorter";

HeaderSorter::HeaderSorter(DependencyGraph& g): CRTP_Modifier<HeaderSorter>(g)
{}

void HeaderSorter::do_process()
{
	DependencyGraph::vertex_iterator i, i_end;
	std::tie(i, i_end) = graph().vertices();
	std::for_each(i, i_end, std::bind(&HeaderSorter::processFile, this, std::placeholders::_1));
}

void HeaderSorter::do_printReport(std::ostream& oss) const
{
	oss << "Nothing has been changed";
}

void HeaderSorter::processFile(DependencyGraph::vertex_descriptor it)
{
	try 
	{
		//TODO clean up that part
		auto& node = graph()(it);
		if(!node->isModifiable()) 
			return;

		ModifiableFile* mf = dynamic_cast<ModifiableFile*>(node.get());
		assert(mf);
		ModifiableFile& file = *mf;

		auto predIsIncludeStatement = std::bind(&Line::isAnIncludeStatement, std::placeholders::_1);

		ModifiableFile::iterator it_header = file.first_of(predIsIncludeStatement);
		if(it_header == file.end()) return;

		utils::gather(file.begin(), file.end(), it_header, predIsIncludeStatement);

		auto it_header_end = file.last_of(predIsIncludeStatement);
		assert(std::all_of(it_header, it_header_end, predIsIncludeStatement));
		std::sort(it_header, it_header_end);

		file.print();
	}
	catch(std::invalid_argument& ex) {}
}