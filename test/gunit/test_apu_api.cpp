#include "gtest/gtest.h"
#include "blockdsp.h"

class APUApiTestAPU : public AudioProcessingUnit
{
public:
	APUApiTestAPU() : AudioProcessingUnit()
	{
		APUParameter *param1 = new APUParameter("param1", APUNUM_FLOAT, APUNUM_FLOAT(0), APUNUM_FLOAT(100));
		APUParameter *param2 = new APUParameter("param2", APUNUM_BOOLEAN, APUNUM_BOOL(0), APUNUM_BOOL(1));

		addParameter(param1);
		addParameter(param2);
	}
};

class APUApiTestFixture : public ::testing::Test
{
protected:
	APUApiTestAPU *apu;

	void SetUp()
	{
		apu = new APUApiTestAPU();
	}

	void TearDown()
	{
		delete apu;
	}
};

TEST_F(APUApiTestFixture, test_param_lookup)
{
	APUParameter *param1 = apu->getParameterWithName("param1");
	EXPECT_TRUE(param1 != NULL);
	EXPECT_EQ(APUNUM_FLOAT, param1->type());

	APUParameter *param2 = apu->getParameterWithName("param2");
	EXPECT_TRUE(param2 != NULL);
	EXPECT_EQ(APUNUM_BOOLEAN, param2->type());

	APUParameter *nullParam = apu->getParameterWithName("shouldNotExist");
	EXPECT_EQ(NULL, nullParam);
}

TEST_F(APUApiTestFixture, test_param_list)
{
	const APUParameterMap params = apu->getParameterMap();

	EXPECT_EQ(2, params.size());
}
