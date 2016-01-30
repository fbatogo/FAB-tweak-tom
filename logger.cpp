#include "logger.h"

#include <iostream>

Logger::Logger() :
    mLogFile("fabtweaktom.log"), mTextStream(&mLogFile)
{
    if (mLogFile.open(QIODevice::WriteOnly) == false) {
        std::cerr << "Unable to open fabtweaktom.log!\n";
        return;
    }

    addLine("FAB-tweak-tom -- G-code tweaker for the FABtotum 3D Printer");
}

/**
 * @brief Logger::addLine - Write a line to our log file.
 *
 * @param logline - The log line to write.
 */
void Logger::addLine(QString logline)
{
    if (mLogFile.isOpen() == false) {
        // Nothing we can do.. :-(
        return;
    }

    // Make sure the line ends with a newline.
    if (logline.endsWith("\n") == false) {
        logline += "\n";
    }

    // Write it to the log.
    mTextStream << logline;
}

