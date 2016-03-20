#include "gcodeeditor.h"
#include "logger.h"

#include <QFile>
#include <QTextStream>

GCodeEditor::GCodeEditor()
{
    mXYFeedRate = 0;
    mZFeedRate = 0;
    mCursorLocation = 0;
    mGCodeFile.clear();
}

/**
 * @brief GCodeEditor::createNewFile - Clear any state, and empty our QStringList so that we
 *      are prepared to create a new G-code file.
 */
void GCodeEditor::createNewFile()
{
    mGCodeFile.clear();
}

/**
 * @brief GCodeEditor::writeFile - Write the G-code in memory out to the named file.
 *
 * @param filename - The filename to write the G-code to.
 *
 * @return true if the file was written correctly.  false otherwise.
 */
bool GCodeEditor::writeFile(QString filename)
{
    QFile file(filename);
    QTextStream stream(&file);

    if (mGCodeFile.isEmpty() == true) {
        logger.addLine("[ERROR] Attempted to save the G-code when the G-code buffer was empty!");
        return false;
    }

    if (file.open(QIODevice::WriteOnly) == false) {
        logger.addLine("[ERROR] Unable to open the file " + filename + " to write the G-code buffer!");
        return false;
    }

    for (int i = 0; i < mGCodeFile.size(); i++) {
        stream << mGCodeFile.at(i) + "\n";
    }

    file.close();
    logger.addLine("Wrote the G-code buffer to " + filename + ".");

    return true;
}

/**
 * @brief GCodeEditor::moveCursorToTop - Move the internal cursor to the top of the G-code file.
 */
void GCodeEditor::moveCursorToTop()
{
    mCursorLocation = 0;
}

/**
 * @brief GCodeEditor::moveCursorToBottom - Move the internal cursor to the bottom of the G-code file.
 *  (Note : The 'bottom' is one line beyond the last line of the file.  This is a suitable location
 *      for adding new lines.)
 */
void GCodeEditor::moveCursorToBottom()
{
    mCursorLocation = mGCodeFile.size();
}

/**
 * @brief GCodeEditor::moveCursorToLine - Move the internal cursor to a location inside the G-code file.
 *      If an attempt is made to move the cursor beyond the end of the file, the cursor will instead
 *      be positioned at the 'bottom' of the file.
 *
 * @param index - The new location to move the cursor to.
 */
void GCodeEditor::moveCursorToLine(int index)
{
    if (index > mGCodeFile.size()) {
        mCursorLocation = mGCodeFile.size();
    } else {
        mCursorLocation = index;
    }
}

/**
 * @brief GCodeEditor::getLineCount - Returns the number of lines that are currently in the G-code file
 *      buffer.
 *
 * @return size_t containing the number of lines in the buffer.
 */
size_t GCodeEditor::getLineCount()
{
    return mGCodeFile.size();
}

/**
 * @brief GCodeEditor::setUnitsToMillimeters - Write the G-code to set the units used to be millimeters.
 */
void GCodeEditor::setUnitsToMillimeters()
{
    addOrEditGCodeLine("G21");
}

/**
 * @brief GCodeEditor::setToAbsolutePositioning - Write the G-code to set the device to absolute positioning.
 */
void GCodeEditor::setToAbsolutePositioning()
{
    addOrEditGCodeLine("G90");
}

/**
 * @brief GCodeEditor::setFeedRateModeUnitsPerMinute
 */
void GCodeEditor::setFeedRateModeUnitsPerMinute(double feedrate)
{
    // Save the feed rate to both our XY and Z variables.
    mXYFeedRate = feedrate;
    mZFeedRate = feedrate;

    // Then, write our setting.
    addOrEditGCodeLine("G94 F" + QString::number(feedrate, 'f', 2));
}

/**
 * @brief GCodeEditor::setNonContactMove - Write the G-code to move the milling head to a new X/Y/Z location.
 *      If any of the x, y, or z, variables is 0, the parameter will be omitted from the G-code, unless all
 *      three are set to 0.   Also, if a current feedrate has been set, that feed rate will be used.  If there
 *      is a different feed rate for X/Y movement, and for Z movement, and there is movement on all three axes,
 *      the slowest feedrate will be used.
 *
 * @param x - Where to move in the X direction.
 * @param y - Where to move in the Y direction.
 * @param z - Where to move in the Z direction.
 */
void GCodeEditor::setNonContactMove(double x, double y, double z)
{
    setMove(x, y, z, false);
}

/**
 * @brief GCodeEditor::setMove - Write the G-code to move the milling head to a new X/Y/Z location.
 *      If any of the x, y, or z, variables is 0, the parameter will be omitted from the G-code, unless all
 *      three are set to 0.   Also, if a current feedrate has been set, that feed rate will be used.  If there
 *      is a different feed rate for X/Y movement, and for Z movement, and there is movement on all three axes,
 *      the slowest feedrate will be used.
 *
 * @param x - Where to move in the X direction.
 * @param y - Where to move in the Y direction.
 * @param z - Where to move in the Z direction.
 * @param contactMove - If true, then the G01 G-code will be used, if false, the G00 G-code will be used.
 */
void GCodeEditor::setMove(double x, double y, double z, bool contactMove)
{
    double effectiveFeedRate = 0;
    QString gcode;

    if (((x != 0) || (y != 0)) && (z != 0)) {
        // We are moving in all three directions, find the lowest of the two feed rates.
        if (mXYFeedRate < mZFeedRate) {
            effectiveFeedRate = mXYFeedRate;
        } else {
            effectiveFeedRate = mZFeedRate;
        }
    } else {
        logger.addLine("Reached an unexpected feed rate setting with parameters (" + QString::number(x, 'f', 4) + "," + QString::number(y, 'f', 4) + "," + QString::number(z, 'f', 4) + ")");

        // Just use XY rate.
        effectiveFeedRate = mXYFeedRate;
    }

    if (contactMove == false) {
        gcode = "G00 ";
    } else {
        gcode = "G01 ";
    }

    if (z == 0) {
        gcode += "X" + QString::number(x, 'f', 4) + " ";

        gcode += "Y" + QString::number(y, 'f', 4) + " ";
    }

    if (z != 0) {
        gcode += "Z" + QString::number(z, 'f', 4) + " ";
    }

    if (effectiveFeedRate != 0) {
        gcode += "F" + QString::number(effectiveFeedRate, 'f', 4);
    }

    // Finally, write it to our file.
    addOrEditGCodeLine(gcode);
}

/**
 * @brief GCodeEditor::setContactMove - Write the G-code to move the milling head to a new X/Y/Z location,
 *      while the head is removing material.
 *
 * @param x - Where to move in the X direction.
 * @param y - Where to move in the Y direction.
 * @param z - Where to move in the Z direction.
 */
void GCodeEditor::setContactMove(double x, double y, double z)
{
    setMove(x, y, z, true);
}

/**
 * @brief GCodeEditor::setStartSpindleClockwise - Write the G-code to start the spindle spinning at the
 *      specified speed (in RPM)
 *
 * @param rpm - The speed to start spinning the spindle at (in a clockwise direction).
 */
void GCodeEditor::setStartSpindleClockwise(unsigned int rpm)
{
    addOrEditGCodeLine("M03 S" + QString::number(rpm));
}

/**
 * @brief GCodeEditor::setDwellInMilliseconds - Write the G-code to 'dwell' at the current location for
 *      a period of milliseconds.
 *
 * @param milliseconds - The number of milliseconds to wait.
 */
void GCodeEditor::setDwellInMilliseconds(unsigned int milliseconds)
{
    addOrEditGCodeLine("G04 P" + QString::number(milliseconds));
}

void GCodeEditor::setDwellInSeconds(unsigned int seconds)
{
    addOrEditGCodeLine("G04 S" + QString::number(seconds));
}

/**
 * @brief GCodeEditor::setXYFeedRate - Set the value that should be used for the X/Y feed rate.
 *
 * @param feedrate - The new feed rate to use.
 */
void GCodeEditor::setXYFeedRate(double feedrate)
{
    mXYFeedRate = feedrate;
}

/**
 * @brief GCodeEditor::setZFeedRate - Set the value that should be used for the Z feed rate.
 *
 * @param feedrate - The new Z feed rate to use.
 */
void GCodeEditor::setZFeedRate(double feedrate)
{
    mZFeedRate = feedrate;
}

/**
 * @brief GCodeEditor::addOrEditGCodeLine - Either edit an existing line, or add a new one (depending
 *      on where the cursor is currently located.)
 *
 * @param line - The line to either add or edit in the G-code.
 */
void GCodeEditor::addOrEditGCodeLine(QString line)
{
    if (mCursorLocation < mGCodeFile.size()) {
        // We are replacing a line.
        mGCodeFile.replace(mCursorLocation, line);
    } else {
        // We are adding a new line.
        mGCodeFile.append(line);
    }

    // Then, move our cursor to the next line.
    mCursorLocation++;
}

/**
 * @brief GCodeEditor::loadExistingFile - Attempt to open an existing file, and load it in to
 *      our QStringList.
 *
 * @param filename - The filename to load G-code data from.
 *
 * @return true if the file was opened, and loaded in to our QStringList.  false otherwise.
 */
bool GCodeEditor::loadExistingFile(QString filename)
{
    QFile file(filename);
    QTextStream stream(&file);

    // Clear our string list so that we can populate it with new data.
    mGCodeFile.clear();

    if (file.open(QIODevice::ReadOnly) == false) {
        logger.addLine("[ERROR] Unable to open the file " + filename + " for reading.");
        return false;
    }

    // Read the data.
    while (stream.atEnd() == false) {
        mGCodeFile.append(stream.readLine());
    }

    file.close();
    logger.addLine("Loaded the G-code from file '" + filename + "'.");
    return true;
}

