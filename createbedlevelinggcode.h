#ifndef CREATEBEDLEVELINGGCODE_H
#define CREATEBEDLEVELINGGCODE_H


class CreateBedLevelingGCode
{
public:
    CreateBedLevelingGCode();

    void setMillSize(double newSize);
    void setOverlapSize(double newSize);
    void setCutDepth(double newDepth);
    void setLevelWidth(double newSize);
    void setLevelHeight(double newSize);
    void setSpindleSpeed(double newSpeed);
    void setXYFeedRate(double newRate);
    void setZFeedRate(double newRate);

private:
    double mMillSize;       // The diameter of the mill in use.
    double mOverlapSize;    // The amount to overlap each mill line.
    double mCutDepth;       // How deep we should cut in, relative to the starting Z location.
    double mLevelWidth;     // How wide should we mill out the leveled area.
    double mLevelHeight;    // How tall should we mill out the leveled area. ("Tall" = Y offset)
    double mSpindleSpeed;   // How fast should we spin the spindle while leveling the area.
    double mXYFeedRate;     // How fast should we move in the X and Y direction.
    double mZFeedRate;      // How fast should we move in the Z direction.
};

#endif // CREATEBEDLEVELINGGCODE_H
