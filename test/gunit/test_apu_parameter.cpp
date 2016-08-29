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
	: APUParameter(name, type) {}
	APUUIAttribute getUIAttributes() {
		return (APU_UI_TYPE_SLIDER ^ APU_UI_ORIENTATION_HORIZONTAL);
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

TEST_F(APUParameterTestFixture, test_ui_attrs)
{
	SliderParameter *param = new SliderParameter("param", APUNUM_FLOAT);
	APUUIAttribute uiAttrs = param->getUIAttributes();

	EXPECT_TRUE(((uiAttrs & APU_UI_TYPE_SLIDER) > 0));
	EXPECT_TRUE(((uiAttrs & APU_UI_ORIENTATION_HORIZONTAL) > 0));
	EXPECT_TRUE(((uiAttrs & APU_UI_TYPE_SWITCH) == 0));
}