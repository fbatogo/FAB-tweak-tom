#include "gcodeeditor.h"

GCodeEditor::GCodeEditor()
{

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
    return false;
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
    // Clear our string list so that we can populate it with new data.
    mGCodeFile.clear();

    return false;
}

