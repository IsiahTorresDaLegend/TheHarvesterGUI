#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>
#include <QFile>
#include <QDebug>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);



    // Load the dark theme style sheet
    QFile styleFile(":/styles/darktheme.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(styleFile.readAll());
        a.setStyleSheet(styleSheet);
        styleFile.close();
    } else {
        qWarning() << "Could not open the style sheet.";
    }


    a.setStyle(QStyleFactory::create("Fusion"));

    // Create the dark palette
    QPalette darkPalette;

    darkPalette.setColor(QPalette::Window, QColor(45,45,48));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(30,30,30));
    darkPalette.setColor(QPalette::AlternateBase, QColor(45,45,48));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(45,45,48));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);

    darkPalette.setColor(QPalette::Link, QColor(0,122,204));

    darkPalette.setColor(QPalette::Highlight, QColor(0,122,204));
    darkPalette.setColor(QPalette::HighlightedText, Qt::white);

    // Apply the dark palette
    a.setPalette(darkPalette);

    // Set the default font
    QFont defaultFont("Segoe UI", 12);
    a.setFont(defaultFont);

    MainWindow w;
    w.show();

    return a.exec();
}
