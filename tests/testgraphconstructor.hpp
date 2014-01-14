#ifndef __TESTGRAPHCONSTRUCTOR_HPP__
#define __TESTGRAPHCONSTRUCTOR_HPP__

#include <gtest/gtest.h>

#include <boost/filesystem.hpp>

/**
* \brief unit test for the graph construction.
*/
class TestGraphConstructor : public ::testing::Test
{
protected:
	virtual void SetUp() override;
	virtual void TearDown() override;

public:
	TestGraphConstructor() = default;

	boost::filesystem::path m_dir_base;
};

#endif