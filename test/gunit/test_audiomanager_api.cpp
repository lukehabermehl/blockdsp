#include "gtest/gtest.h"
#include "blockdsp.h"

class AudioManagerAPITestFixture : public ::testing::Test 
{
public:
	AudioManager audioManager;
protected:
	void SetUp() {}
	void TearDown() {}
};

TEST_F(AudioManagerAPITestFixture, test_get_devices)
{
	AudioDeviceInfoRef deviceInfo = audioManager.getDevices();
	EXPECT_TRUE((bool)deviceInfo); //Expect not null

	while (deviceInfo) {
		EXPECT_TRUE(deviceInfo->name != "");
		EXPECT_TRUE(deviceInfo->index >= 0);
		deviceInfo = deviceInfo->next;
	}

}

TEST_F(AudioManagerAPITestFixture, test_input_mode)
{
	audioManager.setInputMode(AudioInputModeFile);
	EXPECT_EQ(-1, audioManager.getInputDeviceIndex());
	AudioDeviceInfoRef deviceInfo = audioManager.getDevices();
	audioManager.setInputMode(AudioInputModeDevice);
	audioManager.setInputDeviceIndex(deviceInfo->index);
	EXPECT_EQ(deviceInfo->index, audioManager.getInputDeviceIndex());
}