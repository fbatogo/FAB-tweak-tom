#include "createbedlevelinggcode.h"

#include "logger.h"
#include "gcodeeditor.h"

CreateBedLevelingGCode::CreateBedLevelingGCode()
{
    mMillSize = 0;
    mOverlapSize = 0;
    mCutDepth = 0;
    mLevelWidth = 0;
    mLevelHeight = 0;
    mSpindleSpeed = 0;
    mXYFeedRate = 0;
    mZFeedRate = 0;
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

void CreateBedLevelingGCode::setSpindleSpeed(unsigned int newSpeed)
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

/**
 * @brief CreateBedLevelingGCode::createGCodeFile - Go through the steps to create the G-code file for
 *      milling a level bed.
 *
 * @param filename - The file name to use for the newly created G-code for milling a level bed.
 *
 * @return QString containing an error, if there was an error.  Otherwise, on success, the QString will
 *      be empty.
 */
QString CreateBedLevelingGCode::createGCodeFile(QString filename)
{
    GCodeEditor gcode;
    double left, bottom, right, top;

    gcode.createNewFile();

    // Start out by configuring things how we want them.
    gcode.setUnitsToMillimeters();
    gcode.setToAbsolutePositioning();
    gcode.setFeedRateModeUnitsPerMinute(mXYFeedRate);

    // Make sure our feed rates get pushed over.
    gcode.setXYFeedRate(mXYFeedRate);
    gcode.setZFeedRate(mZFeedRate);

    // Make sure there is room to spin up the head.
    gcode.setNonContactMove(0, 0, 1);

    // Then, spin it up.
    gcode.setStartSpindleClockwise(mSpindleSpeed);

    // And, wait for it to be spun up.
    gcode.setDwellInMilliseconds(1);

    // Then, move the head to the correct depth.
    gcode.setContactMove(0, 0, mCutDepth);

    left = 0;
    bottom = 0;

    // set our height and width
    right = mLevelWidth;
    top = mLevelHeight;

    while ((left < right) && (bottom < top)) {
        // Do one complete square.
        gcode.setContactMove(left, bottom, mCutDepth);
        gcode.setContactMove(left, top, mCutDepth);
        gcode.setContactMove(right, top, mCutDepth);
        gcode.setContactMove(right, bottom, mCutDepth);
        gcode.setContactMove(left, bottom, mCutDepth);

        // Move one overlap unit in each direction.
        if (left < right) {
            left += mOverlapSize;
            right -= mOverlapSize;
        }

        if (bottom < top) {
            bottom += mOverlapSize;
            top -= mOverlapSize;
        }
    }

    if (gcode.writeFile(filename) == false) {
        return "Unable to write the G-code to a file!";
    }

    // Everything is good.
    return "";
}

/**
 * @brief CreateBedLevelingGCode::requiredValuesSet - Verify that the values that have been provided
 *      are all set as needed.
 *
 * @return true if all values look correct.  false otherwise.
 */
bool CreateBedLevelingGCode::requiredValuesSet()
{
    // We don't actually use mill size, so don't check it.

    if (mOverlapSize <= 0) {
        log.addLine("No valid overlap size was provided while trying to mill a level bed.");
        return false;
    }

    if (mCutDepth <= 0) {
        log.addLine("No valid cut depth was provided while trying to mill a level bed.");
        return false;
    }

    if (mLevelWidth <= 0) {
        log.addLine("No valid width was provided while trying to mill a level bed.");
        return false;
    }

    if (mLevelHeight <= 0) {
        log.addLine("No valid height was provided while trying to mill a level bed.");
        return false;
    }

    if (mSpindleSpeed <= 0) {
        log.addLine("No valid spindle speed was provided while trying to mill a level bed.");
        return false;
    }

    if ((mXYFeedRate == 0) && (mZFeedRate == 0)) {
        log.addLine("No feed rate was provided while trying to mill a level bed.");
        return false;
    }

    return true;
}

