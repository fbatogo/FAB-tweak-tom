#include "createbedlevelinggcode.h"

CreateBedLevelingGCode::CreateBedLevelingGCode()
{

}

void CreateBedLevelingGCode::setMillSize(double newSize)
{
    mMillSize = newSize;
}

void CreateBedLevelingGCode::setOverlapSize(double newSize)
{
    mOverlapSize = newSize;
}

void CreateBedLevelingGCode::setCutDepth(double newDepth)
{
    mCutDepth = newDepth;
}

void CreateBedLevelingGCode::setLevelWidth(double newSize)
{
    mLevelWidth = newSize;
}

void CreateBedLevelingGCode::setLevelHeight(double newSize)
{
    mLevelHeight = newSize;
}

void CreateBedLevelingGCode::setSpindleSpeed(double newSpeed)
{
    mSpindleSpeed = newSpeed;
}

void CreateBedLevelingGCode::setXYFeedRate(double newRate)
{
    mXYFeedRate = newRate;
}

void CreateBedLevelingGCode::setZFeedRate(double newRate)
{
    mZFeedRate = newRate;
}

