#include "gtest/gtest.h"
#include "blockdsp.h"

static const char *kFilePath = "../resources/guitar.wav";

class AudioFileTestFixture : public ::testing::Test
{
public:
	AudioFile *audioFile;
protected:
	void SetUp()
	{
		audioFile = new AudioFile(kFilePath, AudioFileModeReadOnly);
		ASSERT_TRUE(audioFile != NULL);
	}

	void TearDown()
	{
		audioFile->close();
		delete audioFile;
	}
};

TEST_F(AudioFileTestFixture, test_file_info)
{
	EXPECT_EQ(44100, audioFile->sampleRate());
	EXPECT_EQ(2, audioFile->numChannels());
	EXPECT_EQ(AudioFileModeReadOnly, audioFile->mode());
}

