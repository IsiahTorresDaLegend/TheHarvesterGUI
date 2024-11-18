/*
formatsettings.cpp

Definitions for formatsettings.h
Responsible for applying format changes in the program such as text color, font, background color,
etc.
Module is to be used with Qt Libraries.
Written by Isiah Torres with Qt Creator.

Consulted with ChatGPT.
*/
//=================================================================================================

#include "formatsettings.h"
#include <QFontDialog>
#include <QColorDialog>
#include <QPalette>

//=================================================================================================

FormatSettings::FormatSettings(QObject *parent)
    : QObject(parent)
{
}

void FormatSettings::changeFont(QTextEdit &textEdit, QWidget *parent)
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, textEdit.font(), parent);
    if (ok) {
        textEdit.setFont(font);
    }
}

void FormatSettings::changeTextColor(QTextEdit &textEdit, QWidget *parent)
{
    QColor color = QColorDialog::getColor(textEdit.textColor(), parent, "Choose Text Color");
    if (color.isValid()) {
        textEdit.setTextColor(color);
    }
}

void FormatSettings::changeTextBackgroundColor(QTextEdit &textEdit, QWidget *parent)
{
    QColor color = QColorDialog::getColor(textEdit.textBackgroundColor(), parent,
                                          "Choose Text Background Color");
    if (color.isValid()) {
        textEdit.setTextBackgroundColor(color);
    }
}

void FormatSettings::changeWindowBackgroundColor(QTextEdit &textEdit, QWidget *parent)
{
    QColor color = QColorDialog::getColor(textEdit.palette().color(QPalette::Base), parent,
                                          "Choose Window Background Color");
    if (color.isValid()) {
        QPalette p = textEdit.palette();
        p.setColor(QPalette::Base, color);
        textEdit.setPalette(p);
    }
}

//=================================================================================================
