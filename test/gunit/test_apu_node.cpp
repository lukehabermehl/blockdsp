#include "gtest/gtest.h"
#include "blockdsp.h"
#include <aputils/aputils.h>

class TestAPU : public BlockDSPAPU
{
public:
	TestAPU() : BlockDSPAPU(new BlockDSPSystem) {
		BlockDSPMultiplierNode *mulNode = getSystem()->createMultiplierNode(1);
		mulNode->connectInput(getSystem()->mainInputNode);
		mulNode->coefficient.setFloatValue(0.5);
		getSystem()->mainOutputNode = mulNode;
	}
};

class APUNodeTestFixture : public ::testing::Test
{
protected:
	TestAPU *apu;

	void SetUp() {
		apu = new TestAPU();
	}
	void TearDown() {
		delete apu;
	}
};

TEST_F(APUNodeTestFixture, test_apu_block_node)
{
	BlockDSPAPUNode *apuNode = new BlockDSPAPUNode(2, apu, 2, 2);
	BlockDSPInputNode *inputNode = new BlockDSPInputNode(3, 2);

	float *inputBuffer = new float[2];
	inputNode->inputBuffer = inputBuffer;

	inputBuffer[0] = 0.5;
	inputBuffer[1] = -0.5;

	apuNode->connectInput(inputNode);

	EXPECT_EQ(0.25, apuNode->valueForChannel(0));
	EXPECT_EQ(-0.25, apuNode->valueForChannel(1));

	inputBuffer[0] = -0.5;
	inputBuffer[1] = 0.5;

	apuNode->onNext();
	EXPECT_EQ(-0.25, apuNode->valueForChannel(0));
	EXPECT_EQ(0.25, apuNode->valueForChannel(1));

	delete []inputBuffer;
	delete inputNode;
	delete apuNode;
}

