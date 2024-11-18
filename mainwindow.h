#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "fileoperations.h"
#include "formatsettings.h"
#include "sourcesmanager.h"
#include "workercontroller.h"
#include "collapsiblegroupbox.h"

#include <QCheckBox>
#include <QSpinBox>
#include <QLineEdit>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // UI action slots
    void on_actionNew_Job_triggered();
    void on_actionSave_As_triggered();
    void on_actionPrint_triggered();
    void on_actionCopy_triggered();
    void on_actionFont_triggered();
    void on_actionText_Color_triggered();
    void on_actionText_Background_Color_triggered();
    void on_actionWindow_Background_Color_triggered();
    void on_actionAbout_triggered();

    // Button slots
    void on_runButton_clicked();
    void on_clearButton_clicked();
    void on_saveSourcesButton_clicked();

    // Worker signals
    void handleWorkerOutput(const QString &output);
    void handleWorkerFinished();
    void handleWorkerError(const QString &errorMessage);

    // Advanced options slots
    void configureProxies();

private:
    Ui::MainWindow *ui;

    // Manager objects
    FileOperations *fileOps;
    FormatSettings *formatSettings;
    SourcesManager *sourcesManager;
    WorkerController *workerController;

    // Advanced options
    CollapsibleGroupBox *advancedOptionsGroup;

    // Advanced options widgets
    QCheckBox *checkBoxVirtualHosts;
    QSpinBox *spinBoxLimit;
    QSpinBox *spinBoxStartIndex;
    QCheckBox *checkBoxUseProxies;
    QPushButton *buttonConfigureProxies;
    QCheckBox *checkBoxUseShodan;
    QLineEdit *lineEditDnsServer;
    QCheckBox *checkBoxTakeovers;
    QCheckBox *checkBoxDnsResolution;
    QCheckBox *checkBoxDnsLookup;
    QCheckBox *checkBoxDnsBruteForce;
    QCheckBox *checkBoxSaveResults;

};

#endif // MAINWINDOW_H
