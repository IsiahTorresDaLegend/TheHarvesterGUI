/*
sourcesmanager.h

This class handles all operations related to inquiring from multiple sources.
Module is to be used with Qt libraries.
Written by Isiah Torres with Qt Creator.

Consulted with ChatGPT.
*/
//=================================================================================================

#ifndef SOURCESMANAGER_H
#define SOURCESMANAGER_H

//=================================================================================================

#include <QObject>
#include <QListWidget>

//=================================================================================================

class SourcesManager : public QObject
{
    Q_OBJECT
public:
    explicit SourcesManager(QListWidget &listWidget, QObject *parent = nullptr);
    void populateSourcesList(const QStringList &availableSources);
    void clearSources();
    void saveSelectedSources();
    QStringList getSelectedSources() const;

signals:
    void sourcesChanged();

private slots:
    void onItemChanged(QListWidgetItem *item);

private:
    QListWidget &listWidgetSources;

};

//=================================================================================================
#endif // SOURCESMANAGER_H
