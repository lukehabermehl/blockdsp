#include "gtest/gtest.h"
#include "blockdsp.h"

#include <string.h>

class CodeBuilderTestFixture : public ::testing::Test {
};

TEST_F(CodeBuilderTestFixture, test_class_name)
{
	BDCodeBuilder *cb = new BDCodeBuilder("SimpleName", "./");
	EXPECT_TRUE((strcmp("BDSP_APU_SimpleName", cb->className()) == 0));
	delete cb;

	cb = new BDCodeBuilder("A Name With Spaces", "./");
	EXPECT_TRUE((strcmp("BDSP_APU_A_Name_With_Spaces", cb->className()) == 0));
	delete cb;

	cb = new BDCodeBuilder("Name/With!@Symbols$", "./");
	EXPECT_TRUE((strcmp("BDSP_APU_Name_With__Symbols_", cb->className()) == 0));
}