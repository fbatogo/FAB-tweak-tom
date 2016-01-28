#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void actionBedLevelMenuSelection();
    void actionGCodeTweakingSelection();
    void slotBedLevelMillSizeSpinBoxChanged();
    void slotBedLevelFileSelectClicked();
    void slotBedLevelCreateClicked();

    void slotFeedRateTweakingInputFileClicked();
    void slotFeedRateTweakingOutputFileClicked();
    void slotFeedRateTweakingCreateButtonClicked();

private:
    void connectSignalsAndSlots();
    void disconnectSignalsAndSlots();
    void getNewSaveFile(QLineEdit *toUpdateLineEdit);

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
