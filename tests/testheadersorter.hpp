#ifndef __TESTHEADERSORTER_HPP__
#define __TESTHEADERSORTER_HPP__

#include <gtest/gtest.h>

#include <boost/filesystem.hpp>

class TestHeaderSorter : public ::testing::Test
{
protected:
	virtual void SetUp() override;
	virtual void TearDown() override;

public:
	TestHeaderSorter() = default;

	boost::filesystem::path m_dir_base;
};

#endif