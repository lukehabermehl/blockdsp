#include <blockdsp.h>
#include <blockdsp/dsputil_delaybuffer.hpp>

static const unsigned kMaxDelaySamples = 20000;

class ExampleEchoEffect : public AudioProcessingUnit
						, public APUParameterCallback
{
public:
	ExampleEchoEffect() : AudioProcessingUnit()
	{
		for (int i=0; i<2; i++)
		{
			delayBuffers[i].initWithSize(kMaxDelaySamples);
		}

		delayParameter = new APUParameter("Delay Time", APUNUM_FLOAT,
										APUNUM_FLOAT(0),
										APUNUM_FLOAT(10),
										APUNUM_FLOAT(1),
										this); //Use callback

		mixParameter = new APUParameter("Mix", APUNUM_FLOAT,
										APUNUM_FLOAT(0),
										APUNUM_FLOAT(1),
										APUNUM_FLOAT(0.5));

		addParameter(delayParameter);
		addParameter(mixParameter);
	}

	virtual const char * getName()
	{
		return "Example Echo Effect";
	}

	virtual void setupInitialState()
	{
		for (int i=0; i<2; i++)
		{
			delayBuffers[i].reset();
		}
	}

	virtual void processAudio(float *inputBuffer, float *outputBuffer,
								int numInputChannels, int numOutputChannels)
	{
		float mix = mixParameter->getCurrentValue().floatValue();
		float dryGain = 1.0 - mix;

		outputBuffer[0] = inputBuffer[0] * dryGain + (delayBuffers[0].sampleAtDelayIndex(delaySamples));

		if (numOutputChannels == 2)
		{
			if (numInputChannels == 2)
			{
				outputBuffer[1] = outputBuffer[0];
			}
			else
			{
				outputBuffer[1] = inputBuffer[1] * dryGain + (delayBuffers[1].sampleAtDelayIndex(delaySamples));
			}
		}

	}

	virtual void onParameterChanged(APUParameter *parameter)
	{
		if (parameter == delayParameter)
		{
			updateDelayTime();
		}
	}

	virtual void onSampleRateChanged()
	{
		updateDelayTime();
	}

protected:
	DelayBuffer delayBuffers[2];
	size_t delaySamples;

	APUParameter *delayParameter;
	APUParameter *mixParameter;

	void updateDelayTime()
	{
		float delayTimeSec = delayParameter->getCurrentValue().floatValue();
		delaySamples = delayTimeSec / getSampleRate();
	}
};

