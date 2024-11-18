/*
fileoperations.h

This class handles all file-related actions such as
methods for saving files, printing, copying text, etc.
Module is to be used with Qt libraries.
Written by Isiah Torres with Qt Creator.

Consulted with ChatGPT.
*/
//=================================================================================================

#ifndef FILEOPERATIONS_H
#define FILEOPERATIONS_H

//=================================================================================================

#include <QObject>
#include <QString>
#include <QTextEdit>

//=================================================================================================

class FileOperations : public QObject
{
    Q_OBJECT
public:
    explicit FileOperations(QObject *parent = nullptr);
    void saveAs(const QTextEdit &textEdit, QWidget *parent);
    void print(const QTextEdit &textEdit, QWidget *parent);
    void copy(QTextEdit &textEdit);

signals:
    void errorOccurred(const QString &message);
};

//=================================================================================================
#endif // FILEOPERATIONS_H
