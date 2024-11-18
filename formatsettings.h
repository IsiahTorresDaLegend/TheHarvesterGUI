/*
formatsettings.h

This class handles all text and color related customizations for text editors.
Module is to be used with Qt libraries.
Written by Isiah Torres with Qt Creator.

Consulted with ChatGPT.
*/
//=================================================================================================

#ifndef FORMATSETTINGS_H
#define FORMATSETTINGS_H

//=================================================================================================

#include <QObject>
#include <QTextEdit>

//=================================================================================================

class FormatSettings : public QObject
{
    Q_OBJECT
public:
    explicit FormatSettings(QObject *parent = nullptr);
    void changeFont(QTextEdit &textEdit, QWidget *parent);
    void changeTextColor(QTextEdit &textEdit, QWidget *parent);
    void changeTextBackgroundColor(QTextEdit &textEdit, QWidget *parent);
    void changeWindowBackgroundColor(QTextEdit &textEdit, QWidget *parent);

signals:
    void errorOccurred(const QString &message);
};

//=================================================================================================
#endif // FORMATSETTINGS_H
