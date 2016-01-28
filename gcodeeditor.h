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

private:
    QStringList mGCodeFile;
};

#endif // GCODEEDITOR_H
