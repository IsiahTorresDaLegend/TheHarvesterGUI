/*
sourcesmanager.cpp

Definitions for sourcemanager.h
Responsible for handling operations where additional sources are inquired.
Module is to be used with Qt Libraries.
Written by Isiah Torres with Qt Creator.

Consulted with ChatGPT.
*/
//=================================================================================================

#include "sourcesmanager.h"
#include <QSettings>
#include <QListWidget>

constexpr auto SOURCE_ALL = "all";

//=================================================================================================

SourcesManager::SourcesManager(QListWidget &listWidget, QObject *parent)
    : QObject(parent), listWidgetSources(listWidget)
{
    connect(&listWidgetSources, &QListWidget::itemChanged, this, &SourcesManager::onItemChanged);

    // Load the user's saved preferences
    QSettings settings("TheHarvesterDevGroup", "TheHarvesterGUI");
    QStringList savedSelectedSources = settings.value("selectedSources").toStringList();

    // Set the checkboxes based on saved preferences
    for (int i = 0; i < listWidgetSources.count(); ++i) {
        QListWidgetItem *item = listWidgetSources.item(i);
        if (savedSelectedSources.contains(item->text())) {
            item->setCheckState(Qt::Checked);
        } else {
            item->setCheckState(Qt::Unchecked);
        }
    }
}

void SourcesManager::populateSourcesList(const QStringList &availableSources) {
    listWidgetSources.clear();
    for (const QString &source : availableSources) {
        if (!source.isEmpty()) {
            QListWidgetItem *item = new QListWidgetItem(source, &listWidgetSources);
            item->setFlags(item->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
            item->setCheckState(Qt::Unchecked);
        }
    }
}

void SourcesManager::clearSources()
{
    for (int i = 0; i < listWidgetSources.count(); ++i) {
        QListWidgetItem *item = listWidgetSources.item(i);
        item->setCheckState(Qt::Unchecked);
    }
}

void SourcesManager::saveSelectedSources()
{
    QSettings settings("TheHarvesterDevGroup", "TheHarvesterGUI");
    QStringList selectedSources = getSelectedSources();
    settings.setValue("selectedSources", selectedSources);
}

QStringList SourcesManager::getSelectedSources() const
{
    QStringList selectedSources;
    for (int i = 0; i < listWidgetSources.count(); ++i) {
        QListWidgetItem *item = listWidgetSources.item(i);
        if (item->checkState() == Qt::Checked) {
            selectedSources << item->text();
        }
    }

    return selectedSources;
}

void SourcesManager::onItemChanged(QListWidgetItem *item)
{
    listWidgetSources.blockSignals(true);

    if (item->text() == SOURCE_ALL) {
        if (item->checkState() == Qt::Checked) {
            // Uncheck and disable all other items
            for (int i = 0; i < listWidgetSources.count(); ++i) {
                QListWidgetItem *otherItem = listWidgetSources.item(i);
                if (otherItem != item) {
                    otherItem->setCheckState(Qt::Unchecked);
                    otherItem->setFlags(otherItem->flags() & ~Qt::ItemIsEnabled);
                }
            }
        } else {
            // Enable all other items
            for (int i = 0; i < listWidgetSources.count(); ++i) {
                QListWidgetItem *otherItem = listWidgetSources.item(i);
                if (otherItem != item) {
                    otherItem->setFlags(otherItem->flags() | Qt::ItemIsEnabled);
                }
            }
        }
    } else {
        // If any other item is checked, uncheck 'all'
        if (item->checkState() == Qt::Checked) {
            QListWidgetItem *allItem = listWidgetSources.findItems(SOURCE_ALL,
                                                                    Qt::MatchExactly).value(0);
            if (allItem && allItem->checkState() == Qt::Checked) {
                allItem->setCheckState(Qt::Unchecked);
                allItem->setFlags(allItem->flags() | Qt::ItemIsEnabled);
            }
        }
    }

    listWidgetSources.blockSignals(false);
    listWidgetSources.update();

    emit sourcesChanged();
}

//=================================================================================================
