/*
workercontroller.cpp

Definitions for workercontroller.h
Responsible for handing the worker process and it's signals, output, and errors.
Module is to be used with Qt libraries.
Written by Isiah Torres with Qt Creator.

Consulted with ChatGPT.
*/
//=================================================================================================

#include "workercontroller.h"
#include <QDebug>


//=================================================================================================

WorkerController::WorkerController(QObject *parent)
    : QObject(parent)
{
}

WorkerController::~WorkerController()
{
    if (workerThread && workerThread->isRunning()) {
        workerThread->quit();
        workerThread->wait();
    }
}

void WorkerController::startWorker(const QString &pythonPath,
                                   const QString &target,
                                   const QString &sourcesArgument,
                                   const QStringList &additionalArguments)
{
    if (isRunning()) {
        emit error("Worker is already running.");
        return;
    }

    // Create new worker and thread
    worker = new PythonWorker();
    worker->setParameters(pythonPath, target, sourcesArgument, additionalArguments);

    workerThread = new QThread(this);
    worker->moveToThread(workerThread);

    // Connect signals and slots
    connect(workerThread, &QThread::started, worker, &PythonWorker::process);
    connect(worker, &PythonWorker::outputReceived, this, &WorkerController::handleWorkerOutput);
    connect(worker, &PythonWorker::finished, this, &WorkerController::handleWorkerFinished);
    connect(worker, &PythonWorker::error, this, &WorkerController::handleWorkerError);
    connect(worker, &PythonWorker::finished, workerThread, &QThread::quit);
    connect(worker, &PythonWorker::error, workerThread, &QThread::quit);

    connect(workerThread, &QThread::finished, workerThread, &QObject::deleteLater);
    connect(workerThread, &QThread::finished, worker, &QObject::deleteLater);

    workerThread->start();
}

bool WorkerController::isRunning() const {
    return workerThread && workerThread->isRunning();
}

void WorkerController::handleWorkerOutput(const QString &output)
{    
    emit outputReceived(output);
}

void WorkerController::handleWorkerFinished()
{
    qDebug() << "Worker finished.";
    worker = nullptr;
    workerThread = nullptr;
    emit finished();
}

void WorkerController::handleWorkerError(const QString &errorMessage)
{
    qDebug() << "Worker encountered an error.";
    worker = nullptr;
    workerThread = nullptr;
    emit error(errorMessage);
}

//=================================================================================================
