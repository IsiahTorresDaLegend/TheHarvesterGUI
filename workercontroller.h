/*
workercontroller.h

This class handles starting a worker process, processing its output, and handles errors.
In this case, it works with a python process in a C++ program.
Module is to be used with Qt libraries.
Written by Isiah Torres with Qt Creator.

Consulted with ChatGPT.
*/
//=================================================================================================

#ifndef WORKERCONTROLLER_H
#define WORKERCONTROLLER_H

//=================================================================================================

#include <QObject>
#include <QThread>
#include <QPointer>
#include "pythonworker.h"

//=================================================================================================

class WorkerController : public QObject
{
    Q_OBJECT
public:
    explicit WorkerController(QObject *parent = nullptr);
    ~WorkerController();
    void startWorker(const QString &pythonPath,
                     const QString &target,
                     const QString &sourcesArgument,
                     const QStringList &additionalArguments);

    bool isRunning() const;

signals:
    void outputReceived(const QString &output);
    void finished();
    void error(const QString &errorMessage);

private slots:
    void handleWorkerOutput(const QString &output);
    void handleWorkerFinished();
    void handleWorkerError(const QString &errorMessage);

private:
    QPointer<PythonWorker> worker;
    QPointer<QThread> workerThread;
};

//=================================================================================================
#endif // WORKERCONTROLLER_H
