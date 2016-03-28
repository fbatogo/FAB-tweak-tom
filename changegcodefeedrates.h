#ifndef CHANGEGCODEFEEDRATES_H
#define CHANGEGCODEFEEDRATES_H

#include <QString>
#include "logger.h"

// Result values that can be retured from the processGCodeFile() call.
#define CHANGE_GCODE_NOTHING_TO_DO           1
#define CHANGE_GCODE_SUCCESS                 0
#define CHANGE_GCODE_INPUT_MISSING           -1
#define CHANGE_GCODE_OUTPUT_MISSING          -2
#define CHANGE_GCODE_CLEANUP_INVALID         -3
#define CHANGE_GCODE_REDEFINE_INVALID        -4
#define CHANGE_GCODE_NO_VALID_FEED_RATES     -5
#define CHANGE_GCODE_UNABLE_TO_OPEN_IN_FILE  -6
#define CHANGE_GCODE_UNABLE_TO_OPEN_OUT_FILE -7

class ChangeGCodeFeedRates
{
public:
    ChangeGCodeFeedRates();

    void setCleanUpGCode(bool newval);
    void setFeedRateSameLine(bool newval);
    void setReplaceM05(bool newval);
    void setRedefineFeedRates(bool newval);
    void setNewXYFeedRate(QString newval);
    void setNewZFeedRate(QString newval);

    void setInputFile(QString filename);
    void setOutputFile(QString filename);

    QString resultCodeAsString(int resultCode);

    int processGCodeFile();

protected:
    int validateInputValues();
    QString processOneGCodeLine(QString inputLine);

private:
    bool mCleanupGCode;
    bool mFeedRatesSameLine;
    bool mReplaceM05;
    bool mRedefineFeedRates;
    bool mOnlyReplaceExistingFeedRates;
    QString mNewXYFeedRate;
    QString mNewZFeedRate;

    QString mInputFile;
    QString mOutputFile;
};

#endif // CHANGEGCODEFEEDRATES_H
