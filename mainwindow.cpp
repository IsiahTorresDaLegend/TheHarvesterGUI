#include "mainwindow.h"
#include "qapplication.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QRegularExpression>
#include <QThread>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include <QSpinBox>
#include <QLineEdit>
#include <QPushButton>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , fileOps(nullptr)
    , formatSettings(nullptr)
    , sourcesManager(nullptr)
    , workerController(nullptr)

{
    ui->setupUi(this);

    fileOps = new FileOperations(this);
    formatSettings = new FormatSettings(this);
    sourcesManager = new SourcesManager(*ui->listWidgetSources, this);
    workerController = new WorkerController(this);

    // List of available sources
    QStringList availableSources = {
        "all",
        "anubis", "baidu", "bevigil", "binaryedge", "bing", "bingapi", "bufferoverun", "brave",
        "censys", "certspotter", "criminalip", "crtsh", "dnsdumpster", "duckduckgo", "fullhunt",
        "github-code", "hackertarget", "hunter", "hunterhow", "intelx", "netlas", "onyphe",
        "otx", "pentesttools", "projectdiscovery", "rapiddns", "rocketreach", "securityTrails",
        "sitedossier", "subdomaincenter", "subdomainfinderc99", "threatminer", "tomba", "urlscan",
        "virustotal", "yahoo", "zoomeye"
    };

    // Populate sources list widget
    sourcesManager->populateSourcesList(availableSources);

    // Connect worker signals to MainWindow slots
    connect(workerController, &WorkerController::outputReceived, this,
            &MainWindow::handleWorkerOutput);
    connect(workerController, &WorkerController::finished, this,
            &MainWindow::handleWorkerFinished);
    connect(workerController, &WorkerController::error, this, &MainWindow::handleWorkerError);

    // Create the advanced options collapsible group box
    advancedOptionsGroup = ui->advancedOptionsGroup;

    // Create the layout for the advanced options
    QVBoxLayout *advancedLayout = new QVBoxLayout();

    // Check for virtual hosts
    checkBoxVirtualHosts = new QCheckBox("Check for virtual hosts using DNS resolution", this);
    advancedLayout->addWidget(checkBoxVirtualHosts);

    // Limit search results
    spinBoxLimit = new QSpinBox(this);
    spinBoxLimit->setMinimum(0);
    spinBoxLimit->setMaximum(10000);
    spinBoxLimit->setValue(0); // 0 means no limit
    QLabel *labelLimit = new QLabel("Limit search results to:", this);
    QHBoxLayout *limitLayout = new QHBoxLayout();
    limitLayout->addWidget(labelLimit);
    limitLayout->addWidget(spinBoxLimit);
    advancedLayout->addLayout(limitLayout);

    // Start at a result number
    spinBoxStartIndex = new QSpinBox(this);
    spinBoxStartIndex->setMinimum(0);
    spinBoxStartIndex->setMaximum(10000);
    spinBoxStartIndex->setValue(0);
    QLabel *labelStartIndex = new QLabel("Start at result number:", this);
    QHBoxLayout *startIndexLayout = new QHBoxLayout();
    startIndexLayout->addWidget(labelStartIndex);
    startIndexLayout->addWidget(spinBoxStartIndex);
    advancedLayout->addLayout(startIndexLayout);

    // Use proxies
    checkBoxUseProxies = new QCheckBox("Use proxies for requests", this);
    advancedLayout->addWidget(checkBoxUseProxies);

    // Configure proxies button
    buttonConfigureProxies = new QPushButton("Configure Proxies...", this);
    advancedLayout->addWidget(buttonConfigureProxies);

    // Use Shodan for host discovery
    checkBoxUseShodan = new QCheckBox("Use Shodan for host discovery", this);
    advancedLayout->addWidget(checkBoxUseShodan);

    // DNS server for lookup
    lineEditDnsServer = new QLineEdit(this);
    QLabel *labelDnsServer = new QLabel("DNS server for lookup:", this);
    QHBoxLayout *dnsServerLayout = new QHBoxLayout();
    dnsServerLayout->addWidget(labelDnsServer);
    dnsServerLayout->addWidget(lineEditDnsServer);
    advancedLayout->addLayout(dnsServerLayout);

    // Check for takeovers
    checkBoxTakeovers = new QCheckBox("Check for takeovers", this);
    advancedLayout->addWidget(checkBoxTakeovers);

    // Perform DNS Resolution on subdomains
    checkBoxDnsResolution = new QCheckBox("Perform DNS resolution on subdomains", this);
    advancedLayout->addWidget(checkBoxDnsResolution);

    // Enable DNS server lookup
    checkBoxDnsLookup = new QCheckBox("Enable DNS server lookup", this);
    advancedLayout->addWidget(checkBoxDnsLookup);

    // Perform DNS Brute force on the domain
    checkBoxDnsBruteForce = new QCheckBox("Perform DNS brute force on the domain", this);
    advancedLayout->addWidget(checkBoxDnsBruteForce);

    // Save results to XML and JSON files
    checkBoxSaveResults = new QCheckBox("Save results to XML and JSON files", this);
    advancedLayout->addWidget(checkBoxSaveResults);

    advancedOptionsGroup->setContentLayout(advancedLayout);

    connect(buttonConfigureProxies, &QPushButton::clicked, this, &MainWindow::configureProxies);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::configureProxies()
{
    QMessageBox::information(this, "Configure Proxies", "Proxy configuration is not yet implemented.");
}

void MainWindow::on_actionNew_Job_triggered()
{
    ui->textBrowserResults->clear();
    sourcesManager->clearSources();
}

void MainWindow::on_actionSave_As_triggered()
{
    fileOps->saveAs(*ui->textBrowserResults, this);
}

void MainWindow::on_actionPrint_triggered()
{
    fileOps->print(*ui->textBrowserResults, this);
}

void MainWindow::on_actionCopy_triggered()
{
    fileOps->copy(*ui->textBrowserResults);
}

void MainWindow::on_actionFont_triggered()
{
   formatSettings->changeFont(*ui->textBrowserResults, this);
}

void MainWindow::on_actionText_Color_triggered()
{
    formatSettings->changeTextColor(*ui->textBrowserResults, this);
}

void MainWindow::on_actionText_Background_Color_triggered()
{
    formatSettings->changeTextBackgroundColor(*ui->textBrowserResults, this);
}

void MainWindow::on_actionWindow_Background_Color_triggered()
{
    formatSettings->changeWindowBackgroundColor(*ui->textBrowserResults, this);
}

// Displays information about the program.
void MainWindow::on_actionAbout_triggered()
{
    QString aboutText;
    aboutText = "The Harvester GUI (alpha)\n";
    aboutText += "Author: Isiah Torres\n";
    aboutText += "\nThe Harvester is a command line tool written by Christian Martorella. ";
    aboutText += "This application builds on this tool by providing a graphical interface ";
    aboutText += "to make it easier for users to set up and use the tool. ";

    QMessageBox::about(this, "About The Harvester GUI", aboutText);
}

// Clear sources button
void MainWindow::on_clearButton_clicked()
{
    sourcesManager->clearSources();
}

void MainWindow::on_saveSourcesButton_clicked()
{
    sourcesManager->saveSelectedSources();
    QMessageBox::information(this, "Preferences Saved", "Your source preferences have been saved.");
}

void MainWindow::handleWorkerOutput(const QString &output)
{
    ui->textBrowserResults->append(output);
}

void MainWindow::handleWorkerFinished()
{
    QApplication::restoreOverrideCursor();
    ui->statusbar->showMessage("Process completed.", 5000);
    ui->runButton->setEnabled(true);
}

void MainWindow::handleWorkerError(const QString &errorMessage)
{
    QApplication::restoreOverrideCursor();
    ui->textBrowserResults->append(errorMessage);
    ui->statusbar->showMessage("Process encountered an error.", 5000);
    ui->runButton->setEnabled(true);
}

// Run button slot
void MainWindow::on_runButton_clicked()
{

    // The target value from the line edit
    // Sanitize and validate the target input
    QString target = ui->lineEdit->text().trimmed();
    if (target.isEmpty()) {
        ui->textBrowserResults->setPlainText("Error: Target cannot be empty.");
        return;
    }

    // Validate the target using a regex for domain names or IP addresses
    QRegularExpression regex("^[a-zA-Z0-9.-]+$");
    if (!regex.match(target).hasMatch()) {
        ui->textBrowserResults->setPlainText("Error: Invalid target format.");
        return;
    }

    // Get of selected sources
    QStringList selectedSources = sourcesManager->getSelectedSources();
    if (selectedSources.isEmpty()) {
        selectedSources << "all";
    }

    // Convert list of selected sources into a comma-separated string, as required by theHarvester
    QString sourcesArgument = selectedSources.join(",");

    // Get the Python interpreter path from compile definitions
    QString pythonPath = QStringLiteral(PYTHON_EXECUTABLE);

    // Collect advanced options
    QStringList additionalArguments;

    // Check for virtual hosts
    if (checkBoxVirtualHosts->isChecked()) {
        additionalArguments << "-v";
    }

    // Limit search results
    int limit = spinBoxLimit->value();
    if (limit > 0) {
        additionalArguments << "-l" << QString::number(limit);
    }

    // Start at result number
    int startIndex = spinBoxStartIndex->value();
    if (startIndex > 0) {
        additionalArguments << "-S" << QString::number(startIndex);
    }

    // Use proxies
    if (checkBoxUseProxies->isChecked()) {
        additionalArguments << "-p";
        // Ensure proxies are configured properly
        QMessageBox::information(this, "Proxies", "Using proxies is not yet implemented.");
    }

    // Use Shodan for host discovery
    if (checkBoxUseShodan->isChecked()) {
        additionalArguments << "-s";
    }

    // DNS server for lookup
    QString dnsServer = lineEditDnsServer->text().trimmed();
    if (!dnsServer.isEmpty()) {
        additionalArguments << "-e" << dnsServer;
    }

    // Check for takeovers
    if (checkBoxTakeovers->isChecked()) {
        additionalArguments << "-t";
    }

    // Perform DNS resolution on subdomains
    if (checkBoxDnsResolution->isChecked()) {
        additionalArguments << "-r";
    }

    // Enable DNS server lookup
    if (checkBoxDnsLookup->isChecked()) {
        additionalArguments << "-n";
    }

    // Perform DNS brute force on the domain
    if (checkBoxDnsBruteForce->isChecked()) {
        additionalArguments << "-c";
    }

    // Save results to XML and JSON files
    if (checkBoxSaveResults->isChecked()) {
        additionalArguments << "-f";
    }

    // Disable the Run button to prevent re-entry
    ui->runButton->setEnabled(false);

    // Display a message to the user
    ui->statusbar->showMessage("Running theHarvester. Please wait...");
    QApplication::setOverrideCursor(Qt::WaitCursor);

    // Start the worker
    workerController->startWorker(pythonPath, target, sourcesArgument, additionalArguments);
}

