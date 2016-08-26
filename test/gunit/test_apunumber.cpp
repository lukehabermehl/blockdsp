#include "gtest/gtest.h"
#include "blockdsp.h"

class TestAPUNumber : public ::testing::Test
{
protected:
	void SetUp() {

	}

	void TearDown() {

	}
};

TEST_F(TestAPUNumber, test_number_types)
{
	APUNumber floatNumber = APUNumber::numberForFloat(1.5);
	EXPECT_EQ(1.5, floatNumber.floatValue());
	EXPECT_EQ(1, floatNumber.integerValue());
	EXPECT_EQ(true, floatNumber.boolValue());

	APUNumber intNumber = APUNumber::numberForInteger(5);
	EXPECT_EQ(5.f, intNumber.floatValue());
	EXPECT_EQ(5, intNumber.integerValue());
	EXPECT_EQ(true, intNumber.boolValue());

	APUNumber boolNumber = APUNumber::numberForBool(false);
	EXPECT_EQ(0, boolNumber.floatValue());
	EXPECT_EQ(0, boolNumber.integerValue());
	EXPECT_EQ(false, boolNumber.boolValue());
}

TEST_F(TestAPUNumber, test_copy_constructor)
{
	APUNumber num;
	APUNumber num2(num);

	num.setFloatValue(3.5);
	EXPECT_EQ(3.5, num2.floatValue());
}

TEST_F(TestAPUNumber, test_assignment)
{
	APUNumber num = APUNUM_FLOAT(4.2f);
	APUNumber num2;
	num2 = num;
	EXPECT_EQ(4.2f, num2.floatValue());
	
	num.setFloatValue(3.5);
	EXPECT_EQ(4.2f, num2.floatValue());
}