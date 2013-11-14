#ifndef __GRAPHCONSTRUCTOR_HPP__
#define __GRAPHCONSTRUCTOR_HPP__

#include <utils/stringhelper.hpp>
#include <utils/exception.hpp>

#include <boost/filesystem.hpp>

class DependencyGraph;

class GraphConstructor
{
		using type_vector_path = std::vector<boost::filesystem::path>;

		static type_vector_path s_includePaths;

	public:
		static StringHelper::type_vector_string s_extensions;

/*
		class ParsingError : public Exception
		{
		public:
			ParsingError(const std::string& mess);
			ParsingError(std::string&& mess);

			virtual ~ParsingError() noexcept = default;
		};

		class FileNotFound : public Exception
		{
		public:
			FileNotFound(const std::string& mess);
			FileNotFound(std::string&& mess);

			virtual ~FileNotFound() noexcept = default;
		};
*/

		static void buildGraph(DependencyGraph& graph, const boost::filesystem::path& p, const StringHelper::type_vector_string& includePaths = StringHelper::type_vector_string());

	private:
		/**
		*
		*
		* \throws ParsingError
		* \throws std::invalid_argument 
		*/
		static void browseDirectory(DependencyGraph& graph, const boost::filesystem::path& p);

		/**
		*
		*
		* \throws ParsingError 
		* \throws std::invalid_argument 
		*/
		static void analyseFile(DependencyGraph& graph, const boost::filesystem::path& p);

		static void cleanUpLine(std::string& line);

		static boost::filesystem::path findFileInIncludeDirs(const boost::filesystem::path& file);

};

#endif // __GRAPHCONSTRUCTOR_HPP__
