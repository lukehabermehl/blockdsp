#include "gtest/gtest.h"
#include "blockdsp.h"

class APUParameterMapTestFixture : public ::testing::Test
{
protected:
	APUParameterMap paramMap;
	void SetUp()
	{
		paramMap.addParameter(new APUParameter("param1", APUNUM_FLOAT, APUNUM_FLOAT(0), APUNUM_FLOAT(100), APUNUM_FLOAT(0)));
		paramMap.addParameter(new APUParameter("param2", APUNUM_BOOLEAN, APUNUM_BOOL(0), APUNUM_BOOL(1), APUNUM_BOOL(0)));
	}
};

TEST_F(APUParameterMapTestFixture, test_iterator)
{
	ASSERT_EQ(2, paramMap.size());

	APUParameterMap::Iterator it = paramMap.begin();

	EXPECT_TRUE(it.valid());
	bool found[] = {false, false};

	for (; it.valid(); ++it)
	{
		std::string name = it.first();
		if (name == "param1")
		{
			found[0] = true;
			EXPECT_TRUE(it.second() != NULL);
		}
		else if (name == "param2")
		{
			found[1] = true;
			EXPECT_TRUE(it.second()!= NULL);
		}
		else
		{
			std::cout << "name was: " << name << std::endl;
			EXPECT_TRUE(false); //Something bad happened
		}
	}

	EXPECT_TRUE(found[0]);
	EXPECT_TRUE(found[1]);
}

TEST_F(APUParameterMapTestFixture, test_api)
{
	APUParameter * param1 = paramMap.parameterWithName("param1");
	EXPECT_TRUE(param1 != NULL);
	EXPECT_EQ(APUNUM_FLOAT, param1->type());

	APUParameter * param2 = paramMap.parameterWithName("param2");
	EXPECT_TRUE(param2 != NULL);
	EXPECT_EQ(APUNUM_BOOLEAN, param2->type());

	EXPECT_EQ(NULL, paramMap.parameterWithName("notaparam"));
}

TEST_F(APUParameterMapTestFixture, test_memory)
{
	APUParameterMap pmap(paramMap);
	EXPECT_EQ(paramMap.parameterWithName("param1"), pmap.parameterWithName("param1"));

	APUParameterMap pmap2 = pmap;
	EXPECT_EQ(paramMap.parameterWithName("param1"), pmap2.parameterWithName("param1"));
}

