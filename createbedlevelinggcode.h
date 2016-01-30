#ifndef CREATEBEDLEVELINGGCODE_H
#define CREATEBEDLEVELINGGCODE_H

#include <QString>

class CreateBedLevelingGCode
{
public:
    CreateBedLevelingGCode();

    void setMillSize(double newSize);
    void setOverlapSize(double newSize);
    void setCutDepth(double newDepth);
    void setLevelWidth(double newSize);
    void setLevelHeight(double newSize);
    void setSpindleSpeed(unsigned int newSpeed);
    void setXYFeedRate(double newRate);
    void setZFeedRate(double newRate);

    QString createGCodeFile(QString filename);

private:
    bool requiredValuesSet();

    double mMillSize;       // The diameter of the mill in use.
    double mOverlapSize;    // The amount to overlap each mill line.
    double mCutDepth;       // How deep we should cut in, relative to the starting Z location.
    double mLevelWidth;     // How wide should we mill out the leveled area.
    double mLevelHeight;    // How tall should we mill out the leveled area. ("Tall" = Y offset)
    unsigned int mSpindleSpeed;   // How fast should we spin the spindle while leveling the area.
    double mXYFeedRate;     // How fast should we move in the X and Y direction.
    double mZFeedRate;      // How fast should we move in the Z direction.

    double mCurrentX;
    double mCurrentY;
};

#endif // CREATEBEDLEVELINGGCODE_H
