#include "gtest/gtest.h"
#include "blockdsp.h"

class BDSPSystemTestFixture : public testing::Test {
protected:
	BlockDSPSystem * system;
	BlockDSPAPU *contextAPU;
	void SetUp() {
		system = new BlockDSPSystem();
		contextAPU = new BlockDSPAPU(system);
	}
	void TearDown() {
		delete system;
	}
};

TEST_F(BDSPSystemTestFixture, test_system_number)
{
	APUNumber num = APUNumber::numberForInteger(5);
	system->addNumber("num", num);
	bool success;
	APUNumber lookup = system->numberWithName("num", success);
	EXPECT_TRUE(success);
	EXPECT_EQ(num.integerValue(), lookup.integerValue());
}

TEST_F(BDSPSystemTestFixture, test_system_delay_line)
{
	BlockDSPDelayLine * dl = system->createDelayLine(1, system->mainInputNode);
	ASSERT_TRUE((dl != NULL));
	BlockDSPDelayLine *lookup = system->delayLineWithID(dl->getID());
	EXPECT_EQ(dl, lookup);
}

TEST_F(BDSPSystemTestFixture, test_system_node)
{
	BlockDSPInputNode *node = system->createInputNode(1);
	ASSERT_TRUE((node != NULL));
	BlockDSPNode * lookup = system->nodeWithID(node->getID());
	EXPECT_EQ(node, lookup);

	system->removeNode(node);
	ASSERT_TRUE((node != NULL));
	lookup = system->nodeWithID(node->getID());
	EXPECT_EQ(NULL, lookup);

	system->addNode(node);
	lookup = system->nodeWithID(node->getID());
	EXPECT_EQ(node, lookup);
}

TEST_F(BDSPSystemTestFixture, test_system_next)
{
	BlockDSPInputNode * node = system->mainInputNode;
	float frame[] = {0.5};
	node->inputBuffer = &frame[0];

	BlockDSPDelayLine * dl = system->createDelayLine(1, system->mainInputNode);
	dl->setSize(1);
	system->mainOutputNode = dl->nodeForDelayIndex(2, 1);

	EXPECT_EQ(0, system->mainOutputNode->valueForChannel(0));
	system->next();
	EXPECT_EQ(0.5, system->mainOutputNode->valueForChannel(0));
}

TEST_F(BDSPSystemTestFixture, test_system_reset)
{
	BlockDSPInputNode * node = system->mainInputNode;
	float frame[] = {0.5};
	node->inputBuffer = &frame[0];

	BlockDSPDelayLine * dl = system->createDelayLine(1, system->mainInputNode);
	dl->setSize(1);
	system->mainOutputNode = dl->nodeForDelayIndex(2, 1);

	EXPECT_EQ(0, system->mainOutputNode->valueForChannel(0));
	system->next();
	EXPECT_EQ(0.5, system->mainOutputNode->valueForChannel(0));
	system->reset();

	EXPECT_EQ(0, system->mainOutputNode->valueForChannel(0));
	EXPECT_EQ(0.5, system->mainInputNode->valueForChannel(0));
}
