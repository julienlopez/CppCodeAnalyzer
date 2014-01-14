#ifndef __TESTFILE_HPP__
#define __TESTFILE_HPP__

#include <gtest/gtest.h>

#include <boost/filesystem/path.hpp>

class TestFile : public ::testing::Test
{

protected:
	virtual void SetUp() override;
	virtual void TearDown() override;

public:
	TestFile() = default;

	boost::filesystem::path m_dir_base;
};

#endif