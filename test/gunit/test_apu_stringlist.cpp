#include "gtest/gtest.h"
#include "blockdsp.h"

class APUStringListTestFixture : public ::testing::Test
{
protected:
	APUStringList stringList;
};

TEST_F(APUStringListTestFixture, test_empty_list)
{
	EXPECT_EQ(0, stringList.size());
	EXPECT_EQ(NULL, stringList.getString(0));
}

TEST_F(APUStringListTestFixture, test_append)
{
	stringList.append("one");
	EXPECT_EQ(1, stringList.size());
	EXPECT_EQ("one", std::string(stringList.getString(0)));
	EXPECT_EQ(NULL, stringList.getString(1));

	stringList.append("two");
	EXPECT_EQ(2, stringList.size());
	EXPECT_EQ("two", std::string(stringList.getString(1)));
}

TEST_F(APUStringListTestFixture, test_set_string)
{
	stringList.append("one");
	EXPECT_EQ(1, stringList.size());
	stringList.setString(0, "two");
	EXPECT_EQ(1, stringList.size());
	EXPECT_EQ("two", std::string(stringList.getString(0)));
}

TEST_F(APUStringListTestFixture, test_reference)
{
	APUStringList sl = stringList;
	EXPECT_EQ(0, sl.size());
	sl.append("one");
	EXPECT_EQ(1, stringList.size());
	EXPECT_EQ("one", std::string(stringList.getString(0)));

	APUStringList scpy;
	scpy = stringList.copy();
	EXPECT_EQ(1, scpy.size());
	EXPECT_EQ("one", std::string(scpy.getString(0)));
	scpy.append("two");
	EXPECT_EQ(1, stringList.size());
	EXPECT_EQ(2, scpy.size());
}