#include "gtest/gtest.h"
#include "blockdsp.h"

class ParameterTestFixture : public testing::Test
{
protected:
	BlockDSPSystem *system;
	BlockDSPAPU *contextAPU;

	void SetUp() {
		system = new BlockDSPSystem();
		contextAPU = new BlockDSPAPU();
		contextAPU->system = system;
	}

	void TearDown() {
		delete system;
		delete contextAPU;
	}
};

void callback_fn1(BlockDSPAPU *contextAPU, 
				BlockDSPParameter *param, 
				BlockDSPNumber *value)
{
	BlockDSPNumber *num = contextAPU->system->numberWithName("number");
	ASSERT_TRUE((num != nullptr));
	num->setFloatValue(value->floatValue());
}

TEST_F(ParameterTestFixture, test_target_number)
{
	BlockDSPNumber num;

	num.setFloatValue(1);
	BlockDSPParameter floatParam(BlockDSPNumberType::FLOAT, "floatParam", &num, contextAPU);
	floatParam.setValue(2.f);
	EXPECT_EQ(2.f, num.floatValue());

	floatParam.setValue(4.f);
	EXPECT_EQ(4.f, num.floatValue());

	num.setIntegerValue(1);
	BlockDSPParameter intParam(BlockDSPNumberType::INTEGER, "intParam", &num, contextAPU);
	intParam.setValue(3);
	EXPECT_EQ(3, num.integerValue());

	num.setBoolValue(false);
	BlockDSPParameter boolParam(BlockDSPNumberType::BOOLEAN, "boolParam", &num, contextAPU);
	boolParam.setValue(true);
	EXPECT_EQ(true, num.boolValue());
}

TEST_F(ParameterTestFixture, test_callback)
{
	BlockDSPNumber *number = new BlockDSPNumber();
	number->setFloatValue(0);
	system->addNumber("number", number);

	BlockDSPParameter *param = system->createParameter("param", BlockDSPNumberType::FLOAT, NULL, contextAPU);
	param->callback = callback_fn1;
	param->setValue(3.f);

	EXPECT_EQ(3.f, number->floatValue());
}

TEST_F(ParameterTestFixture, test_callback_and_param)
{
	BlockDSPNumber *number = new BlockDSPNumber();
	BlockDSPNumber *number2 = new BlockDSPNumber();
	number->setFloatValue(0);
	number2->setFloatValue(1);

	system->addNumber("number", number);

	BlockDSPParameter *param = system->createParameter("param", BlockDSPNumberType::FLOAT, number2, contextAPU);
	param->callback = callback_fn1;

	param->setValue(3.f);

	EXPECT_EQ(3.f, number->floatValue());
	EXPECT_EQ(3.f, number2->floatValue());
}
