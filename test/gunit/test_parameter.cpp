#include "gtest/gtest.h"
#include "blockdsp.h"

class TestAPU : public BlockDSPAPU {
public:
	BlockDSPParameter *param1;
	BlockDSPParameter *param2;
	BlockDSPNumberRef num1;
	BlockDSPNumberRef num2;
	TestAPU(BlockDSPSystem *system) : BlockDSPAPU(system) {
		num1 = std::make_shared<BlockDSPNumber>();
		num2 = std::make_shared<BlockDSPNumber>();
		param1 = createParameter("param1", BlockDSPNumberType::FLOAT, num1);
		param2 = createParameter("param2", BlockDSPNumberType::BOOLEAN, num2);
	}

	BlockDSPParameter * testCreateParameter(const char *name, BlockDSPNumberType type, BlockDSPNumberRef target) {
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
				BlockDSPNumberRef value)
{
	BlockDSPNumberRef num = contextAPU->getSystem()->numberWithName("number");
	ASSERT_TRUE(num);
	num->setFloatValue(value->floatValue());
}

TEST_F(ParameterTestFixture, test_target_number)
{
	BlockDSPNumberRef num = BDSP_FLOAT(0);

	num->setFloatValue(1);
	BlockDSPParameter floatParam(BlockDSPNumberType::FLOAT, "floatParam", num, contextAPU);
	floatParam.setValue(BlockDSPNumber::numberForFloat(2));
	EXPECT_EQ(2.f, num->floatValue());

	floatParam.setValue(BlockDSPNumber::numberForFloat(4));
	EXPECT_EQ(4.f, num->floatValue());

	num->setIntegerValue(1);
	BlockDSPParameter intParam(BlockDSPNumberType::INTEGER, "intParam", num, contextAPU);
	intParam.setValue(BlockDSPNumber::numberForInteger(3));
	EXPECT_EQ(3, num->integerValue());

	num->setBoolValue(false);
	BlockDSPParameter boolParam(BlockDSPNumberType::BOOLEAN, "boolParam", num, contextAPU);
	boolParam.setValue(BlockDSPNumber::numberForBool(true));
	EXPECT_EQ(true, num->boolValue());
}

TEST_F(ParameterTestFixture, test_callback)
{
	BlockDSPNumberRef number(new BlockDSPNumber());
	number->setFloatValue(0);
	system->addNumber("number", number);

	BlockDSPParameter *param = contextAPU->testCreateParameter("param", BlockDSPNumberType::FLOAT, NULL);
	param->callback = callback_fn1;
	param->setValue(BlockDSPNumber::numberForFloat(3));

	EXPECT_EQ(3.f, number->floatValue());
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
	BlockDSPParameterList& paramList = contextAPU->getParameterList();
	EXPECT_EQ(2, paramList.size());
	EXPECT_EQ(contextAPU->param1, paramList.getAt(0));
	EXPECT_EQ(contextAPU->param1, paramList[0]);
	EXPECT_EQ(contextAPU->param2, paramList.getAt(1));
	EXPECT_EQ(contextAPU->param2, paramList[1]);
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
