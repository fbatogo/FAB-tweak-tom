#include "changegcodefeedrates.h"
#include "logger.h"

ChangeGCodeFeedRates::ChangeGCodeFeedRates()
{
    // Set our default values.
    mCleanupGCode = true;
    mFeedRatesSameLine = true;
    mReplaceM05 = true;
    mOnlyReplaceExistingFeedRates = false;
    mRedefineFeedRates = true;              // Probably should be false?

    mNewXYFeedRate.clear();
    mNewZFeedRate.clear();
    mInputFile.clear();
    mOutputFile.clear();
}

void ChangeGCodeFeedRates::setCleanUpGCode(bool newval)
{
    mCleanupGCode = newval;
}

void ChangeGCodeFeedRates::setFeedRateSameLine(bool newval)
{
    mFeedRatesSameLine = newval;
}

void ChangeGCodeFeedRates::setReplaceM05(bool newval)
{
    mReplaceM05 = newval;
}

void ChangeGCodeFeedRates::setRedefineFeedRates(bool newval)
{
    mRedefineFeedRates = newval;
}

void ChangeGCodeFeedRates::setNewXYFeedRate(QString newval)
{
    mNewXYFeedRate = newval;
}

void ChangeGCodeFeedRates::setNewZFeedRate(QString newval)
{
    mNewZFeedRate = newval;
}

void ChangeGCodeFeedRates::setInputFile(QString filename)
{
    mInputFile = filename;
}

void ChangeGCodeFeedRates::setOutputFile(QString filename)
{
    mOutputFile = filename;
}

/**
 * @brief ChangeGCodeFeedRates::resultCodeAsString - Given one of the CHANGE_GCODE_* result code values, return
 *      a string describing what the code means.
 *
 * @param resultCode - One of the CHANGE_GCODE_* values.
 *
 * @return std::string containing the description of the result code.
 */
QString ChangeGCodeFeedRates::resultCodeAsString(int resultCode)
{
    switch (resultCode) {
    case CHANGE_GCODE_INPUT_MISSING:
        return "The input G-code file was not provided.";

    case CHANGE_GCODE_NOTHING_TO_DO:
        return "The combination of configuration values resulted in nothing to be done.";

    case CHANGE_GCODE_OUTPUT_MISSING:
        return "The output G-code file was not provided.";

    case CHANGE_GCODE_SUCCESS:
        return "The operation completed successfully.";

    case CHANGE_GCODE_CLEANUP_INVALID:
        return "No options were selected to clean up, but the option to clean up the G-code file was selected.";

    case CHANGE_GCODE_REDEFINE_INVALID:
        return "No options were selected to redefine, but the option to redefine feed rates was selected.";

    case CHANGE_GCODE_UNABLE_TO_OPEN_IN_FILE:
        return "Unable to open the input G-code file.";

    case CHANGE_GCODE_UNABLE_TO_OPEN_OUT_FILE:
        return "Unable to open the output G-code file.";

    default:
        return "An unknown result code was provided to resultCodeAsString()!";
    }
}

/**
 * @brief ChangeGCodeFeedRates::processGCodeFile - Actually handle processing the G-Code file based on the values
 *      that have been input through the set*() calls.
 *
 * @return int containing one of the CHANGE_GCODE_* values defined in the header.
 */
int ChangeGCodeFeedRates::processGCodeFile()
{
    int result;
    FILE *infile, *outfile;
    char oneLine[1024];         // Should be large enough for a standard g-code line.  (Unless *ALL* the g-codes are on a single line..  But, we don't handle that. ;)
    QString inputLine;
    unsigned long lineNumber = 0;
    unsigned long outputLineNumber = 0;
    QString outputLine;

    // Validate the data variables that were passed in to make sure that we can actually
    // process the available data.
    result = validateInputValues();
    if (result != CHANGE_GCODE_SUCCESS) {
        logger.addLine("Input validation failed while changing a G-code file : " + resultCodeAsString(result));
        return result;
    }

    // Open up the file we want to read in (in read only mode)
    infile = fopen(mInputFile.toStdString().c_str(), "r");
    if (infile == NULL) {
        logger.addLine("Unable to open the input G-code file : " + mInputFile);
        return CHANGE_GCODE_UNABLE_TO_OPEN_IN_FILE;
    }

    // Open up the file we want to write to.
    outfile = fopen(mOutputFile.toStdString().c_str(), "w");
    if (outfile == NULL) {
        logger.addLine("Unable to open the output G-code file : " + mOutputFile);

        // Clean up.
        fclose(infile);

        return CHANGE_GCODE_UNABLE_TO_OPEN_OUT_FILE;
    }

    logger.addLine("Parsed line log format [<input file line number>/<output file line number>] <input line to be processed>/<output line processed>");
    while (fgets(oneLine, sizeof(oneLine), infile) != NULL) {
        inputLine = oneLine;
        lineNumber++;

        if (inputLine.isEmpty() == true) {
            logger.addLine("[" + QString::number(lineNumber) + "/" + QString::number(outputLineNumber) + "] *** Input line is empty.  Skipping.");
        } else {
           outputLineNumber++;

           outputLine = processOneGCodeLine(inputLine);
           // XXX FINISH!
        }
    }

    // Clean up.
    fclose(infile);
    fclose(outfile);

    // Success!
    return CHANGE_GCODE_SUCCESS;
}

/**
 * @brief ChangeGCodeFeedRates::validateInputValues - Check the various combinations of input values to make sure
 *      that they have all of the data that they need to operate on the file.
 *
 * @return int containing one of the CHANGE_GCODE_* values defined in the header.
 */
int ChangeGCodeFeedRates::validateInputValues()
{
    if (mInputFile.isEmpty() == true) {
        logger.addLine("There is no input G-code file defined.  Cannot process G-code changes!");
        return CHANGE_GCODE_INPUT_MISSING;
    }

    if (mOutputFile.isEmpty() == true) {
        logger.addLine("There is no output G-code file defined.  Cannot process G-code changes!");
        return CHANGE_GCODE_OUTPUT_MISSING;
    }

    // If these are both false, all other options will be disabled.
    if ((mCleanupGCode == false) && (mRedefineFeedRates == false)) {
        logger.addLine("Neither the 'clean up G-code' nor the 'redefine feed rates' options are selected.  Nothing to do.");
        return CHANGE_GCODE_NOTHING_TO_DO;
    }

    // If "Cleanup G-code" is checked, make sure at least one option under it is checked as well.
    if (mCleanupGCode == true) {
        if ((mFeedRatesSameLine == false) && (mReplaceM05 == false)) {
            logger.addLine("The 'clean up G-code' option is selected, but none of the options for what to clean up are selected.  Nothing to do.");
            return CHANGE_GCODE_CLEANUP_INVALID;
        }
    }

    // If "redefine feed rates" is checked, make sure at least one option under it is checked as well.
    if (mRedefineFeedRates == true) {
        // Make sure we have at least one feed rate defined.
        if ((mNewXYFeedRate.isEmpty() == true) && (mNewZFeedRate.isEmpty())) {
            logger.addLine("The 'redefine feed rates' option is selected, but no replacement feed rates were defined.  Nothing to do.");
            return CHANGE_GCODE_NO_VALID_FEED_RATES;
        }
    }

    // Everything looks good!  Move on!
    return CHANGE_GCODE_SUCCESS;
}

QString ChangeGCodeFeedRates::processOneGCodeLine(QString inputLine)
{

}

