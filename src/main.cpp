#include "graphconstructor.hpp"
#include "dependencygraph.hpp"

#include <boost/program_options.hpp>

#include <cstdlib>
#include <fstream>

using namespace std;

void analyze(const DependencyGraph& graph);

void drawGraphWithOGDF(const DependencyGraph& graph);

void drawGraphWithGraphwiz(const DependencyGraph& graph);

int main(int argc, char* argv[])
{
	const StringHelper::type_vector_string defaultExtensions = GraphConstructor::s_extensions;

	boost::program_options::options_description desc("possible options");
    desc.add_options()
        ("version,v", "display software version")
        ("help", "display this help message")
        ("extensions", boost::program_options::value<StringHelper::type_vector_string>()->default_value(defaultExtensions, StringHelper::join(defaultExtensions)), "list the extensions of the files to be parsed")
        ("graph", boost::program_options::value<StringHelper::type_vector_string>(), "display a graph")
        ("analyze", boost::program_options::value<StringHelper::type_vector_string>(), "analyze the files to point out redundant includes")
        ("include_paths,I", boost::program_options::value<StringHelper::type_vector_string>(), "paths to include directory to account for while looking for header files")
        ("display,d", boost::program_options::value<std::string>(), "engine used to display the graph (if the 'graph' option is used).\nCurrently, only graphwiz (default) and OGDF are supported.");

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

	StringHelper::type_vector_string includePaths;
    if(vm.count("include_paths"))
    {
		includePaths = vm["include_paths"].as<std::vector<std::string>>();
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
		std::cout << desc << std::endl;
		return EXIT_SUCCESS;
	}

	try {
		for(auto i = inputs.cbegin(); i != inputs.cend(); ++i)
			GraphConstructor::buildGraph(graph, boost::filesystem::path(*i), includePaths);

		if(vm.count("graph"))
		{
			std::string display;
			if(vm.count("display"))
				display = vm["display"].as<std::string>();

			if(display == "OGDF")
				drawGraphWithOGDF(graph);
			else
				drawGraphWithGraphwiz(graph);
		}
		else if(vm.count("analyze"))
		{
			std::cout << "generating report" << std::endl;
			analyze(graph);
		}
		else assert(0);
	}
	catch(std::exception& e) {
		cerr << "an exception has been caught:" << endl;
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

#define USE_COIN
#define OGDF_OWN_LPSOLVER
#define COIN_OSI_CLP

#include <ogdf/layered/SugiyamaLayout.h>
#include <ogdf/layered/OptimalRanking.h>
#include <ogdf/layered/MedianHeuristic.h>
#include <ogdf/layered/OptimalHierarchyLayout.h>

#include <ogdf/energybased/FMMMLayout.h>

#include <ogdf/planarity/PlanarizationLayout.h>
#include <ogdf/planarity/VariableEmbeddingInserter.h>
#include <ogdf/planarity/FastPlanarSubgraph.h>
#include <ogdf/orthogonal/OrthoLayout.h>
#include <ogdf/planarity/EmbedderMinDepthMaxFaceLayers.h>

#include <ogdf/boostgraph2ogdfconverter.hpp>

void drawGraphWithOGDFOptimalHierarchy(ogdf::Graph& G, ogdf::GraphAttributes& GA)
{
	ogdf::SugiyamaLayout SL;
	SL.setRanking(new ogdf::OptimalRanking);
	SL.setCrossMin(new ogdf::MedianHeuristic);

	//TODO compile OGDF with solver
	ogdf::OptimalHierarchyLayout* ohl = new ogdf::OptimalHierarchyLayout;
	ohl->layerDistance(30.0);
	ohl->nodeDistance(25.0);
	ohl->weightBalancing(0.8);
	SL.setLayout(ohl);

	SL.call(GA);
}

void drawGraphWithOGDFEnergyBased(ogdf::Graph& /*G*/, ogdf::GraphAttributes& GA)
{
	ogdf::FMMMLayout fmmm;
 
	fmmm.useHighLevelOptions(true);
	fmmm.unitEdgeLength(15.0); 
	fmmm.newInitialPlacement(true);
	fmmm.qualityVersusSpeed(ogdf::FMMMLayout::qvsGorgeousAndEfficient);

	fmmm.call(GA);
}

void drawGraphWithOGDFOrthogonalLayout(ogdf::Graph& /*G*/, ogdf::GraphAttributes& GA)
{
	ogdf::PlanarizationLayout pl;
 
    ogdf::FastPlanarSubgraph *ps = new ogdf::FastPlanarSubgraph;
    ps->runs(100);
    ogdf::VariableEmbeddingInserter *ves = new ogdf::VariableEmbeddingInserter;
    ves->removeReinsert(ogdf::EdgeInsertionModule::rrAll);
    pl.setSubgraph(ps);
    pl.setInserter(ves);
 
    ogdf::EmbedderMinDepthMaxFaceLayers *emb = new ogdf::EmbedderMinDepthMaxFaceLayers;
    pl.setEmbedder(emb);
 
    ogdf::OrthoLayout *ol = new ogdf::OrthoLayout;
    ol->separation(20.0);
    ol->cOverhang(0.4);
    ol->setOptions(2+4);
    pl.setPlanarLayouter(ol);
 
    pl.call(GA);
}

void drawGraphWithOGDF(const DependencyGraph& graph)
{
	std::cout << "displaying dependency graph with OGDF" << std::endl;

	//convert boost graph to OGDF graph
	auto res = BoostGraph2OGDFConverter::convert(graph);

	drawGraphWithOGDFOptimalHierarchy(*res.first, *res.second);
	// drawGraphWithOGDFEnergyBased(*res.first, *res.second);
	// drawGraphWithOGDFOrthogonalLayout(*res.first, *res.second);

	res.second->writeGML("dependencygraph.gml");
	system("gml2pic dependencygraph.gml");	
}

void drawGraphWithGraphwiz(const DependencyGraph& graph)
{
	std::cout << "displaying dependency graph with graphwiz" << std::endl;
	std::ofstream g("dependencygraph.dot");
	graph.print(g);
	g.close();
	system("dot -Tps -Goverlap=false dependencygraph.dot -o dependencygraph.ps");
	system("gv dependencygraph.ps ");
}
