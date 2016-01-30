#ifndef LOGGER_H
#define LOGGER_H

#include <QFile>
#include <QTextStream>

class Logger
{
public:
    Logger();

    void addLine(QString logline);

private:
    QFile mLogFile;
    QTextStream mTextStream;
};

static Logger log;

#endif // LOGGER_H
