#include "gtest/gtest.h"
#include "blockdsp.h"
#include <string.h>

class APUParameterTestFixture : public ::testing::Test
{
protected:
	void SetUp() {

	}

	void TearDown() {

	}
};

TEST_F(APUParameterTestFixture, test_essentials)
{
	APUParameter param("param", APUNUM_FLOAT);
	EXPECT_EQ(0, strcmp("param", param.getName()));
	param.setName("new_name");
	EXPECT_EQ(0, strcmp("new_name", param.getName()));

	EXPECT_EQ(APUNUM_FLOAT, param.type());
	EXPECT_EQ(0.0, param.getCurrentValue().floatValue());
	EXPECT_EQ(0.0, param.getTarget().floatValue());

	APUNumber val = APUNUM_FLOAT(7.5);
	param.setValue(val);
	EXPECT_EQ(7.5, param.getCurrentValue().floatValue());
	EXPECT_EQ(7.5, param.getTarget().floatValue());
}

TEST_F(APUParameterTestFixture, test_value_clamping)
{
	APUParameter *param = new APUParameter("param", APUNUM_FLOAT);
	param->setMinValue(APUNUM_FLOAT(-5));
	param->setMaxValue(APUNUM_FLOAT(5));

	APUNumber val = APUNUM_FLOAT(3.5);
	param->setValue(val);
	EXPECT_EQ(3.5, param->getCurrentValue().floatValue());

	val.setFloatValue(-6.5);
	param->setValue(val);
	EXPECT_EQ(-5.0, param->getCurrentValue().floatValue());

	val.setFloatValue(7.5);
	param->setValue(val);
	EXPECT_EQ(5.0, param->getCurrentValue().floatValue());

	delete param;
}
