#ifndef __GRAPHCONSTRUCTOR_HPP__
#define __GRAPHCONSTRUCTOR_HPP__

#include <utils/stringhelper.hpp>

#include <stdexcept>
#include <boost/filesystem.hpp>

class DependencyGraph;

class GraphConstructor
{
	public:
		static StringHelper::type_vector_string s_extensions;

		class ParsingError : public std::exception
		{
		public:
			ParsingError(const std::string& message);

			virtual ~ParsingError() noexcept = default;

			virtual const char* what() const noexcept override;

		private:
			std::string m_message;
		};

		static void buildGraph(DependencyGraph& graph, const boost::filesystem::path& p);

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
};

#endif // __GRAPHCONSTRUCTOR_HPP__
