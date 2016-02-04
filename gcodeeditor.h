#ifndef GCODEEDITOR_H
#define GCODEEDITOR_H

#include <QStringList>

class GCodeEditor
{
public:
    GCodeEditor();

    void createNewFile();
    bool loadExistingFile(QString filename);
    bool writeFile(QString filename);

    void moveCursorToTop();
    void moveCursorToBottom();
    void moveCursorToLine(int index);

    size_t getLineCount();

    void setUnitsToMillimeters();
    void setToAbsolutePositioning();
    void setFeedRateModeUnitsPerMinute(double feedrate);
    void setNonContactMove(double x, double y, double z);
    void setContactMove(double x, double y, double z);
    void setStartSpindleClockwise(unsigned int rpm);
    void setDwellInMilliseconds(unsigned int milliseconds);
    void setXYFeedRate(double feedrate);
    void setZFeedRate(double feedrate);

private:
    void addOrEditGCodeLine(QString line);
    void setMove(double x, double y, double z, bool contactMove);

    QStringList mGCodeFile;
    int mCursorLocation;
    double mXYFeedRate;
    double mZFeedRate;
};

#endif // GCODEEDITOR_H
