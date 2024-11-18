/*
fileoperations.cpp

Definitions for fileoperations.h
Responsible for file operations in a program such as saveAs, print, copy, etc.
Module is to be used with Qt libraries.
Written by Isiah Torres with Qt Creator.

Consulted with ChatGPT.
*/
//=================================================================================================

#include "fileoperations.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QPrinter>
#include <QPrintDialog>

//=================================================================================================

FileOperations::FileOperations(QObject *parent)
    : QObject(parent)
{
}

void FileOperations::saveAs(const QTextEdit &textEdit, QWidget *parent)
{
    QString fileName = QFileDialog::getSaveFileName(parent, "Select Save Location");
    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(parent, "Warning", "Cannot open file: " + file.errorString());
        emit errorOccurred("Cannot open file: " + file.errorString());
        return;
    }

    QTextStream output(&file);
    output << textEdit.toPlainText();
    file.close();
}

void FileOperations::print(const QTextEdit &textEdit, QWidget *parent)
{
    QPrinter printer;
    QPrintDialog printDialog(&printer, parent);
    if (printDialog.exec() == QDialog::Rejected) {
        return;
    }
    textEdit.print(&printer);
}

void FileOperations::copy(QTextEdit &textEdit)
{
    textEdit.copy();
}
