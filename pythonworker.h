#ifndef PYTHONWORKER_H
#define PYTHONWORKER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QProcess>
#include <QPointer>

class PythonWorker : public QObject
{
    Q_OBJECT
public:
    explicit PythonWorker(QObject *parent = nullptr);
    ~PythonWorker();

    void setParameters(const QString &pythonPath,
                       const QString &target,
                       const QString &sourcesArgument,
                       const QStringList &additionalArguments);

public slots:
    void process();
    void handleReadyReadStandardOutput();
    void handleReadyReadStandardError();
    void handleProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);

signals:
    void outputReceived(const QString &output);
    void finished();
    void error(const QString &errorMessage);

private:
    QString pythonPath;
    QString target;
    QString sourcesArgument;
    QStringList additionalArguments;

    QPointer<QProcess> harvesterProcess;
};

#endif // PYTHONWORKER_H
