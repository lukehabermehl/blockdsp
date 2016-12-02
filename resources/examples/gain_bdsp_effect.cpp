#include <blockdsp.h>

class GainEffect : public BlockDSPAPU
{
public:
	enum BlockID
	{
		MULTIPLIER_ID = kFirstUserBlockID,
		INVALID_ID //I like to have a value to represent the end of an enum
	};

	GainEffect(BlockDSPSystem *system) : BlockDSPAPU(system)
	{
		//Create a gain parameter
		APUParameter *gainParameter = new APUParameter(APUNUM_FLOAT, 0.0, 2.0, 1.0);
		addParameter(gainParameter);

		//Create the multiplier block
		BlockDSPMultiplierNode *multiplierNode = system->createMultiplierNode(MULTIPLIER_ID);
		multiplierNode->connectInput(system->mainInputNode);
		multiplierNode->useParameter = true;
		multiplierNode->parameter = gainParameter;

		//Set the multiplier node to be the output source
		system->mainOutputNode = multiplierNode;
	}
};