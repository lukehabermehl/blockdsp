#include "gtest/gtest.h"
#include "blockdsp.h"

class BDSPParameterTestFixture : public ::testing::Test
{
protected:
	class TestAPU : public BlockDSPAPU
	{
	public:
		BlockDSPParameter *parameter;

		TestAPU(BlockDSPSystem *system) : BlockDSPAPU(system) {
			this->parameter = new BlockDSPParameter(APUNUM_FLOAT, "param", this, APUNUM_FLOAT(-100), APUNUM_FLOAT(100), APUNUM_FLOAT(0));
			addParameter(this->parameter);
		}
		APUNumber lastParamValue;
		APUNumber callbackValue;

		void onParameterChanged(BlockDSPParameter *param);
	};

	TestAPU *apu;
	void SetUp() {
		apu = new TestAPU(new BlockDSPSystem());
	}

	void TearDown() {
		delete apu;
	}

	static void CallbackFn(BlockDSPAPU *apu, BlockDSPParameter *param) {
		TestAPU *tAPU = (TestAPU *)apu;
		tAPU->callbackValue.setFloatValue(param->getCurrentValue().floatValue());
	}
};

void BDSPParameterTestFixture::TestAPU::onParameterChanged(BlockDSPParameter *param) {
	lastParamValue.setFloatValue(param->getCurrentValue().floatValue());
	APUNumber num = APUNUM_FLOAT(3.5);
}

TEST_F(BDSPParameterTestFixture, test_essentials)
{
	ASSERT_TRUE((apu->parameter != NULL));
	EXPECT_EQ(1, apu->getNumParameters());
	BlockDSPParameter *param = (BlockDSPParameter *)apu->getParameterWithName("param");
	ASSERT_TRUE((param != NULL));
	APUNumber val = APUNUM_FLOAT(5.0);
	param->setValue(val);
	EXPECT_EQ(5.0, param->getCurrentValue().floatValue());
	EXPECT_EQ(5.0, apu->lastParamValue.floatValue());
}

TEST_F(BDSPParameterTestFixture, test_callback_fn)
{
	apu->parameter->callback = BDSPParameterTestFixture::CallbackFn;
	APUNumber val = APUNUM_FLOAT(4.5);
	apu->parameter->setValue(val);
	EXPECT_EQ(4.5, apu->callbackValue.floatValue());
}


