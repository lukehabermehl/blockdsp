#include "gtest/gtest.h"
#include "blockdsp.h"

class TestAPU : public BlockDSPAPU {
public:
	BlockDSPParameter *param1;
	BlockDSPParameter *param2;
	APUNumber num1;
	APUNumber num2;
	TestAPU(BlockDSPSystem *system) : BlockDSPAPU(system) {
		param1 = createParameter("param1", APUNumberType::APUNUM_FLOAT, num1);
		param2 = createParameter("param2", APUNumberType::APUNUM_BOOLEAN, num2);
	}

	BlockDSPParameter * testCreateParameter(const char *name, APUNumberType type, APUNumber target) {
		return createParameter(name, type, target);
	}
};

class ParameterTestFixture : public testing::Test
{
protected:
	BlockDSPSystem *system;
	TestAPU *contextAPU;

	void SetUp() {
		system = new BlockDSPSystem();
		contextAPU = new TestAPU(system);
	}

	void TearDown() {
		delete contextAPU;
	}
};

void callback_fn1(BlockDSPAPU *contextAPU, 
				BlockDSPParameter *param, 
				APUNumber value)
{
	bool success;
	APUNumber num = contextAPU->getSystem()->numberWithName("number", success);
	ASSERT_TRUE(success);
	num.setFloatValue(value.floatValue());
}

// TEST_F(ParameterTestFixture, test_target_number)
// {
// 	APUNumber num;

// 	num.setFloatValue(1);
// 	BlockDSPParameter floatParam(APUNumberType::APUNUM_FLOAT, "floatParam", num, contextAPU);
// 	floatParam.setValue(APUNumber::numberForFloat(2));
// 	EXPECT_EQ(2.f, num.floatValue());

// 	floatParam.setValue(APUNumber::numberForFloat(4));
// 	EXPECT_EQ(4.f, num.floatValue());

// 	num->setIntegerValue(1);
// 	BlockDSPParameter intParam(BlockDSPNumberType::INTEGER, "intParam", num, contextAPU);
// 	intParam.setValue(BlockDSPNumber::numberForInteger(3));
// 	EXPECT_EQ(3, num->integerValue());

// 	num->setBoolValue(false);
// 	BlockDSPParameter boolParam(BlockDSPNumberType::BOOLEAN, "boolParam", num, contextAPU);
// 	boolParam.setValue(BlockDSPNumber::numberForBool(true));
// 	EXPECT_EQ(true, num->boolValue());
// }

TEST_F(ParameterTestFixture, test_callback)
{
	APUNumber number;
	number->setFloatValue(0);
	system->addNumber("number", number);

	BlockDSPParameter *param = contextAPU->testCreateParameter("param", APUNumberType::APUNUM_FLOAT, APUNumber());
	param->callback = callback_fn1;
	param->setValue(APUNumber::numberForFloat(3));

	EXPECT_EQ(3.f, number.floatValue());
}

TEST_F(ParameterTestFixture, test_callback_and_param)
{
	BlockDSPNumberRef number(new BlockDSPNumber());
	BlockDSPNumberRef number2(new BlockDSPNumber());
	number->setFloatValue(0);
	number2->setFloatValue(1);

	system->addNumber("number", number);

	BlockDSPParameter *param = contextAPU->testCreateParameter("param", BlockDSPNumberType::FLOAT, number2);
	param->callback = callback_fn1;

	param->setValue(BlockDSPNumber::numberForFloat(3));

	EXPECT_EQ(3.f, number->floatValue());
	EXPECT_EQ(3.f, number2->floatValue());
}

TEST_F(ParameterTestFixture, test_parameter_list)
{
	BlockDSPParameterMap& paramList = contextAPU->getParameterMap();
	EXPECT_EQ(2, paramList.size());
	EXPECT_EQ(contextAPU->param1, paramList.parameterWithName("param1"));
	EXPECT_EQ(contextAPU->param1, paramList["param1"]);
	EXPECT_EQ(contextAPU->param2, paramList.parameterWithName("param2"));
	EXPECT_EQ(contextAPU->param2, paramList["param2"]);
}

TEST_F(ParameterTestFixture, test_value_constraints)
{
	contextAPU->param1->setMinValue(BDSP_FLOAT(-5.5));
	contextAPU->param1->setMaxValue(BDSP_FLOAT(5.5));

	contextAPU->param1->setValue(BDSP_FLOAT(3.5));
	EXPECT_EQ(3.5, contextAPU->num1->floatValue());

	contextAPU->param1->setValue(BDSP_FLOAT(-10.5));
	EXPECT_EQ(-5.5, contextAPU->num1->floatValue());

	contextAPU->param1->setValue(BDSP_FLOAT(5.6));
	EXPECT_EQ(5.5, contextAPU->num1->floatValue());
}
