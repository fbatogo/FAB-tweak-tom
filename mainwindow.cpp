#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connectSignalsAndSlots();

    // Set our default widget.
    ui->stackedWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    disconnectSignalsAndSlots();

    delete ui;
}

/**
 * @brief MainWindow::connectSignalsAndSlots - Hook up all of the signals and slots that are used in
 *      the main window of our app.
 */
void MainWindow::connectSignalsAndSlots()
{
    // Connect menu slots/signals.
    connect(ui->actionAdd_Edit_feed_rates_in_a_G_code_file, SIGNAL(triggered(bool)), this, SLOT(actionGCodeTweakingSelection()));
    connect(ui->actionCreate_Bed_Leveling_G_code, SIGNAL(triggered(bool)), this, SLOT(actionBedLevelMenuSelection()));
    connect(ui->action_Quit, SIGNAL(triggered(bool)), this, SLOT(close()));

    // Bed leveling slots/signals.
    connect(ui->bedLevelMillSizeSpinbox, SIGNAL(valueChanged(double)), this, SLOT(slotBedLevelMillSizeSpinBoxChanged()));
    connect(ui->bedLevelFileSelectButton, SIGNAL(clicked(bool)), this, SLOT(slotBedLevelFileSelectClicked()));
    connect(ui->bedLevelCreatePushButton, SIGNAL(clicked(bool)), this, SLOT(slotBedLevelCreateClicked()));

    // Feed rate tweaking slots/signals.
    connect(ui->feedRateTweakingInputFileButton, SIGNAL(clicked(bool)), this, SLOT(slotFeedRateTweakingInputFileClicked()));
    connect(ui->feedRateTweakerOutputFileButton, SIGNAL(clicked(bool)), this, SLOT(slotFeedRateTweakingOutputFileClicked()));
    connect(ui->feedRateTweakingCreateButton, SIGNAL(clicked(bool)), this, SLOT(slotFeedRateTweakingCreateButtonClicked()));
}

/**
 * @brief MainWindow::disconnectSignalsAndSlots - Disconnect all of the signals and slots that are used
 *      in the main window of our app.
 */
void MainWindow::disconnectSignalsAndSlots()
{
    // Disconnect menu slots/signals.
    disconnect(ui->actionAdd_Edit_feed_rates_in_a_G_code_file, SIGNAL(triggered(bool)), this, SLOT(actionGCodeTweakingSelection()));
    disconnect(ui->actionCreate_Bed_Leveling_G_code, SIGNAL(triggered(bool)), this, SLOT(actionBedLevelMenuSelection()));
    disconnect(ui->action_Quit, SIGNAL(triggered(bool)), this, SLOT(close()));

    // Bed leveling slots/signals.
    disconnect(ui->bedLevelMillSizeSpinbox, SIGNAL(valueChanged(double)), this, SLOT(slotBedLevelMillSizeSpinBoxChanged()));
    disconnect(ui->bedLevelFileSelectButton, SIGNAL(clicked(bool)), this, SLOT(slotBedLevelFileSelectClicked()));
    disconnect(ui->bedLevelCreatePushButton, SIGNAL(clicked(bool)), this, SLOT(slotBedLevelCreateClicked()));

    // Feed rate tweaking slots/signals.
    disconnect(ui->feedRateTweakingInputFileButton, SIGNAL(clicked(bool)), this, SLOT(slotFeedRateTweakingInputFileClicked()));
    disconnect(ui->feedRateTweakerOutputFileButton, SIGNAL(clicked(bool)), this, SLOT(slotFeedRateTweakingOutputFileClicked()));
    disconnect(ui->feedRateTweakingCreateButton, SIGNAL(clicked(bool)), this, SLOT(slotFeedRateTweakingCreateButtonClicked()));
}

/**
 * @brief MainWindow::getNewSaveFile - Given a line edit, create a "New save file" dialog, populate it with the
 *      current line edit value, and if the result is updated, update the line edit with the new value.
 * @param toUpdateLineEdit
 */
void MainWindow::getNewSaveFile(QLineEdit *toUpdateLineEdit)
{
    QString existingFilename;
    QString newFilename;

    existingFilename = toUpdateLineEdit->text();

    newFilename = QFileDialog::getSaveFileName(this, tr("Create a new G-Code File"), existingFilename, "*.gcode");
    if (newFilename.isEmpty() == false) {
        // Update what is shown in our edit text field.
        if (newFilename.toLower().endsWith(".gcode") == false) {
            // Add our extension.
            newFilename += ".gcode";
        }

        toUpdateLineEdit->setText(newFilename);
    }
}

/**
 * @brief MainWindow::actionBedLevelMenuSelection - Called when the user selects the menu option to create
 *      a bed leveling gcode file.  It should change our active stacked widget, and set up any associated values.
 */
void MainWindow::actionBedLevelMenuSelection()
{
    // Set the active widget.
    ui->stackedWidget->setCurrentIndex(1);
}

/**
 * @brief MainWindow::actionGCodeTweakingSelection - Called when the user selects the menu option to tweak
 *      an existing Gcode file. It should change the active stacked widget, and set up any associated values.
 */
void MainWindow::actionGCodeTweakingSelection()
{
    // Set the active widget.
    ui->stackedWidget->setCurrentIndex(2);
}

/**
 * @brief MainWindow::slotBedLevelMillSizeSpinBoxChanged - Called when the mill size of the bed leveling widget
 *      is changed.  We will use the new value to calculate the overlap offset and populate that box with the
 *      value.  (Which is basically 1/2 the mill size value.)
 */
void MainWindow::slotBedLevelMillSizeSpinBoxChanged()
{
    double millSize;
    double overlapSize;

    millSize = ui->bedLevelMillSizeSpinbox->value();

    overlapSize = millSize / 2;

    ui->bedLevelOverlapSpinBox->setValue(overlapSize);
}

/**
 * @brief MainWindow::slotBedLevelFileSelectClicked - Called when a user clicks on the "..." button to select
 *      a file to create.
 */
void MainWindow::slotBedLevelFileSelectClicked()
{
    getNewSaveFile(ui->bedLevelFileToCreateField);
}

/**
 * @brief MainWindow::slotBedLevelCreateClicked - Called when a user clicks on the "Create" button at the bottom
 *      of the bed level creation widget.  It should check that all the values look reasonable, and then start
 *      the process of creating the bed leveling G-code.
 */
void MainWindow::slotBedLevelCreateClicked()
{
   QMessageBox::information(this, "Not implemented", "This isn't implemented!  How lucky for you!");
}

/**
 * @brief MainWindow::slotFeedRateTweakingInputFileClicked - Called when the user clicks on the "..." button to
 *      select a new input file.  It should update the line edit next to the button.
 */
void MainWindow::slotFeedRateTweakingInputFileClicked()
{
    QString existingFilename;
    QString newFilename;

    existingFilename = ui->feedRateTweakingInputFileField->text();

    newFilename = QFileDialog::getOpenFileName(this, tr("Open an existing G-Code File"), existingFilename, "*.gcode");
    if (newFilename.isEmpty() == false) {
        // Update what is shown in our edit text field.
        if (newFilename.toLower().endsWith(".gcode") == false) {
            // Add our extension.
            newFilename += ".gcode";
        }

        ui->feedRateTweakingInputFileField->setText(newFilename);
    }
}

/**
 * @brief MainWindow::slotFeedRateTweakingOutputFileClicked - Called when the user clicks on the "..." button to
 *      select a new output file.  It should update the line edit next to the button.
 */
void MainWindow::slotFeedRateTweakingOutputFileClicked()
{
    getNewSaveFile(ui->feedRateTweakerOutputFileField);
}

/**
 * @brief MainWindow::slotFeedRateTweakingCreateButtonClicked - Called when the user clicks on the "Create" button
 *      on the feed rate editing widget.  It should validate the available settings, and then create the edited
 *      g-code file.
 */
void MainWindow::slotFeedRateTweakingCreateButtonClicked()
{
    QMessageBox::information(this, "Not implemented", "This isn't implemented!  How lucky for you!");
}
