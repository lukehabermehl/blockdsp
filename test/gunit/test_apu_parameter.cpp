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

class SliderParameter : public APUParameter
{
public:
	SliderParameter(const char *name, APUNumberType type)
	: APUParameter(name, type, APUNUM_FLOAT(0), APUNUM_FLOAT(10), APUNUM_FLOAT(0)) {}
	APUUIAttribute getUIAttributes() {
		return (APU_UI_TYPE_SLIDER ^ APU_UI_ORIENTATION_HORIZONTAL);
	}
};

TEST_F(APUParameterTestFixture, test_essentials)
{
	APUParameter param("param", APUNUM_FLOAT, APUNUM_FLOAT(0), APUNUM_FLOAT(10), APUNUM_FLOAT(0));
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
	APUParameter *param = new APUParameter("param", APUNUM_FLOAT, APUNUM_FLOAT(-5), APUNUM_FLOAT(5), APUNUM_FLOAT(0));

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

TEST_F(APUParameterTestFixture, test_ui_attrs)
{
	SliderParameter *param = new SliderParameter("param", APUNUM_FLOAT);
	APUUIAttribute uiAttrs = param->getUIAttributes();

	EXPECT_TRUE(((uiAttrs & APU_UI_TYPE_SLIDER) > 0));
	EXPECT_TRUE(((uiAttrs & APU_UI_ORIENTATION_HORIZONTAL) > 0));
	EXPECT_TRUE(((uiAttrs & APU_UI_TYPE_SWITCH) == 0));
}

TEST_F(APUParameterTestFixture, test_units)
{
	APUParameter *param = new APUParameter("param", APUNUM_FLOAT, -5, 5, 0);
	param->setUnits("Hertz");
	EXPECT_EQ("Hertz", std::string(param->getUnits()));

	//Test max size = 16
	param->setUnits("12345678901234567");
	EXPECT_EQ("1234567890123456", std::string(param->getUnits()));

	delete param;
}

TEST_F(APUParameterTestFixture, test_enum_param)
{
	APUStringList strings;
	strings.append("OFF");
	strings.append("ON");
	APUEnumParameter enumParam("enumParam", strings, NULL);

	std::string value0Str = enumParam.stringForValue(0);
	EXPECT_EQ("OFF", value0Str);

	std::string value1Str = enumParam.stringForValue(1);
	EXPECT_EQ("ON", value1Str);

	EXPECT_EQ(NULL, enumParam.stringForValue(2));
}
