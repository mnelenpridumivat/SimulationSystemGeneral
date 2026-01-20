#include "Base/GraphTargetPositionFragment.h"

FGraphTargetPositionFragment::FGraphTargetPositionFragment(const FGraphWay& OtherWay)
{
	CurrentPosition = OtherWay.CurrentVertexIndex;
	this->Way = OtherWay.Way;
}

FGraphTargetPositionFragment& FGraphTargetPositionFragment::operator=(const FGraphWay& OtherWay)
{
	CurrentPosition = OtherWay.CurrentVertexIndex;
	this->Way = OtherWay.Way;
	return *this;
}
