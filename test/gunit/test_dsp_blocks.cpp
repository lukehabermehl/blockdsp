#include "gtest/gtest.h"
#include "blockdsp.h"

class DSPBlockTestFixture : public ::testing::Test
{
public:
	float *inputFrame;
	float *outputFrame;

	BlockDSPInputNode *testInputNode;

protected:
	void SetUp() {
		inputFrame = new float[2];
		inputFrame[0] = 0; inputFrame[1] = 0;

		outputFrame = new float[2];
		outputFrame[0] = 0; outputFrame[1] = 0;

		testInputNode = new BlockDSPInputNode(2);
		testInputNode->inputBuffer = inputFrame;
	}

	void TearDown() {
		delete testInputNode;
		delete []inputFrame;
		delete []outputFrame;
	}

	void SetInputFrame(float left, float right) {
		testInputNode->inputBuffer[0] = left;
		testInputNode->inputBuffer[1] = right;
	}
};

TEST_F(DSPBlockTestFixture, test_input_node)
{
	SetInputFrame(0.5, -0.5);
	EXPECT_EQ(inputFrame[0], testInputNode->valueForChannel(0));
	EXPECT_EQ(inputFrame[1], testInputNode->valueForChannel(1));
}

TEST_F(DSPBlockTestFixture, test_multiplier_node)
{
	SetInputFrame(0.5, -0.5);

	APUNumber coeff;
	coeff.setFloatValue(0.5);

	BlockDSPMultiplierNode multiplierNode(2);
	multiplierNode.coefficient = coeff;
	multiplierNode.connectInput(testInputNode);

	EXPECT_EQ(0.5 * inputFrame[0], multiplierNode.valueForChannel(0));
	EXPECT_EQ(0.5 * inputFrame[1], multiplierNode.valueForChannel(1));
}

TEST_F(DSPBlockTestFixture, test_summer_node)
{
	SetInputFrame(0.5, -0.5);

	BlockDSPInputNode inputNode2(2);
	float inputBuf2[] = {0.2, 0.2};
	inputNode2.inputBuffer = inputBuf2;

	BlockDSPSummerNode summerNode(2);
	summerNode.connectInput(testInputNode);
	summerNode.connectInput(&inputNode2);

	EXPECT_EQ(inputFrame[0] + inputBuf2[0], summerNode.valueForChannel(0));
	EXPECT_EQ(inputFrame[1] + inputBuf2[1], summerNode.valueForChannel(1));
}

TEST_F(DSPBlockTestFixture, test_delay_line)
{
	SetInputFrame(0.5, -0.5);

	BlockDSPDelayLine delayLine(2);
	delayLine.setSize(1);
	delayLine.inputNode = testInputNode;

	EXPECT_EQ(0, delayLine.valueForDelayIndex(1, 0));
	EXPECT_EQ(0, delayLine.valueForDelayIndex(1, 1));

	delayLine.shuffle();
	SetInputFrame(0, 0);

	EXPECT_EQ(0.5, delayLine.valueForDelayIndex(1, 0));
	EXPECT_EQ(-0.5, delayLine.valueForDelayIndex(1, 1));
}
