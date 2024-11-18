
#include "pythonworker.h"
#include <QProcess>
#include <QDir>
#include <QThread>
#include <QDebug>


PythonWorker::PythonWorker(QObject *parent)
    : QObject(parent), harvesterProcess(nullptr)
{
}

PythonWorker::~PythonWorker()
{
}

void PythonWorker::setParameters(const QString &pythonPath,
                                 const QString &target,
                                 const QString &sourcesArgument,
                                 const QStringList &additionalArguments)
{
    this->pythonPath = pythonPath;
    this->target = target;
    this->sourcesArgument = sourcesArgument;
    this->additionalArguments = additionalArguments;
}

void PythonWorker::process()
{

    qDebug() << "Worker thread started.";

    // Check if Python executable exists
    QFileInfo pythonExecutable(pythonPath);
    if (!pythonExecutable.exists() || !pythonExecutable.isExecutable()) {
        qDebug() << "Python executable not found or not executable at: " << pythonPath;
        emit error("Error: Python interpreter not found or not executable at: " + pythonPath);
        return;
    }

    // Get the path th theHarvester script
    QString harvesterPath = QDir::currentPath() + "/theHarvester/theHarvester.py";
    qDebug() << "theHarvester script path:" << harvesterPath;


    // Check if theHarvester exists
    QFileInfo harvesterFiles(harvesterPath);
    if (!harvesterFiles.exists()) {
        qDebug() << "theHarvester script not found at:" << harvesterPath;
        emit error("Error: theHarvester script not found at: " + harvesterPath);
        return;
    }

    // Prepare the arguments
    // "-u" to force Python to run in unbuffered mode to incrementally display output as it's ready
    QStringList arguments;
    arguments << "-u" << harvesterPath
              << "-d" << target
              << "-b" << sourcesArgument;

    arguments.append(additionalArguments);

    qDebug() << "Process arguments:" << arguments;

    // Create a QProcess to run theHarvester
    harvesterProcess = new QProcess();

    // Configure the process
    harvesterProcess->setProgram(pythonPath);
    harvesterProcess->setArguments(arguments);

    // Capture output
    harvesterProcess->setProcessChannelMode(QProcess::MergedChannels);
    harvesterProcess->setWorkingDirectory(QDir::currentPath());

    // Connect signals to slots
    connect(harvesterProcess, &QProcess::readyReadStandardOutput, this, &PythonWorker::handleReadyReadStandardOutput);
    connect(harvesterProcess, &QProcess::readyReadStandardError, this, &PythonWorker::handleReadyReadStandardError);
    connect(harvesterProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &PythonWorker::handleProcessFinished);
    connect(harvesterProcess, &QProcess::errorOccurred, this, [this](QProcess::ProcessError processError) {

        // Handle specific errors
        QString errorMessage;

        switch (processError) {
            case QProcess::FailedToStart:
                errorMessage = "Failed to start theHarvester process.";
                break;
            case QProcess::Crashed:
                errorMessage = "theHarvester process crashed.";
                break;
            case QProcess::Timedout:
                errorMessage = "theHarvester process timed out.";
                break;
            case QProcess::WriteError:
                errorMessage = "Write error occurred in theHarvester process.";
                break;
            case QProcess::ReadError:
                errorMessage = "Read error occurred in theHarvester process.";
                break;
            case QProcess::UnknownError:
            default:
                errorMessage = "An unknown error occured in theHarvester process.";
                break;
        }
        qDebug() << errorMessage;
        emit error(errorMessage);
    });

    // Start the process
    harvesterProcess->start();
    qDebug() << "Executing command:" << harvesterProcess->program() << harvesterProcess->arguments();


    if (!harvesterProcess->waitForStarted()) {
        emit error("Error: Could not start theHarvester process.");
        return;
    }

    qDebug() << "Worker thread finished.";

}

void PythonWorker::handleReadyReadStandardOutput()
{
    if(harvesterProcess) {
        QByteArray outputData = harvesterProcess->readAllStandardOutput();
        QString output = QString::fromLocal8Bit(outputData);

        qDebug() << "Emitting outputReceived signal with output:";
        qDebug() << output;

        emit outputReceived(output);
    }
}

void PythonWorker::handleProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    qDebug() << "Process finished with exit code: " << exitCode;
    qDebug() << "Exit status:" << exitStatus;

    // Read any remaining output
    QByteArray outputData = harvesterProcess->readAll();
    QString output = QString::fromLocal8Bit(outputData);

    if (!output.isEmpty()) {
        qDebug() << "Emitting final outputReceived signal with output:";
        qDebug() << output;
        emit outputReceived(output);
    }

    // Check for errors
    if (exitStatus == QProcess::CrashExit) {
        emit error("Error: theHarvester process crashed.");
    } else {
        emit finished();
    }

    harvesterProcess = nullptr;

    qDebug() << "Worker thread finshed.";
}

void PythonWorker::handleReadyReadStandardError()
{
    if (harvesterProcess) {
        qDebug() << "handleReadyReadStandardError called.";
        QByteArray errorData = harvesterProcess->readAllStandardError();
        QString errorOutput = QString::fromLocal8Bit(errorData);

        qDebug() << "Error output data:" << errorOutput;

        emit outputReceived(errorOutput);
    }
}


