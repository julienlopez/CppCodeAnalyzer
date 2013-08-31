#include "graphconstructor.hpp"
#include "dependencygraph.hpp"
#include "stringhelper.hpp"

#include <boost/program_options.hpp>

#include <iostream>
#include <cstdlib>

using namespace std;

void analyze(const DependencyGraph& graph);

int main(int argc, char* argv[])
{
	const StringHelper::type_vector_string defaultExtensions = GraphConstructor::s_extensions;

	boost::program_options::options_description desc("options possibles");
    desc.add_options()
        ("version,v", "display software version")
        ("help", "display this help message")
        ("extensions", boost::program_options::value<StringHelper::type_vector_string>()->default_value(defaultExtensions, StringHelper::join(defaultExtensions)), "list the extensions of the files to be parsed")
        ("graph", boost::program_options::value<StringHelper::type_vector_string>(), "display a graph")
        ("analyze", boost::program_options::value<StringHelper::type_vector_string>(), "analyze the files to point out redundant includes");

	boost::program_options::positional_options_description p;
    p.add("graph", -1); //p.add("analyze", -1); //TODO switch back to analyze by default

	boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
    boost::program_options::notify(vm);

	if(vm.count("help"))
	{
        std::cout << desc << std::endl;
        return EXIT_SUCCESS;
    }

    if(vm.count("version"))
    {
        std::cout << "HeaderDependencyAnalyser v1.0" << std::endl;
        return EXIT_SUCCESS;
    }

	DependencyGraph graph;
	StringHelper::type_vector_string inputs;
	if(vm.count("graph"))
	{
		inputs = vm["graph"].as<std::vector<std::string>>();
	}
	else if(vm.count("analyze"))
	{
		cout << "analyzing " << endl;
		inputs = vm["analyze"].as<std::vector<std::string>>();
	} else
	{
		cerr << "no recognized option" << endl;
		return EXIT_FAILURE;
	}

	try {
		for(auto i = inputs.cbegin(); i != inputs.cend(); ++i)
			GraphConstructor::buildGraph(graph, boost::filesystem::path(*i));

		if(vm.count("graph"))
			graph.print();
		else if(vm.count("analyze"))
			analyze(graph);
		else cerr << "que dalle!!!" << endl;
	}
	catch(std::exception& e) {
		cerr << e.what() << endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

#include "analyzer/analyzerfactory.hpp"
#include "analyzer/ianalyzer.hpp"

void analyze(const DependencyGraph& graph)
{
	std::unique_ptr<iAnalyzer> redondantheaderanalyzer(AnalyzerFactory::createAnalyzer("RedondantHeaderAnalyzer", graph));
	redondantheaderanalyzer->analyze();
	redondantheaderanalyzer->printReport(std::cerr);
}
