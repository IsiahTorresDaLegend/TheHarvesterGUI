//=================================================================================================

#include "collapsiblegroupbox.h"
#include <QScrollArea>

//=================================================================================================

CollapsibleGroupBox::CollapsibleGroupBox(QWidget *parent)
    : QWidget(parent)
{
    QString defaultTitle = "Advanced Options ▸";
    initialize(defaultTitle);
}

CollapsibleGroupBox::CollapsibleGroupBox(const QString &title, QWidget *parent)
    : QWidget(parent)
{
    initialize(title);
}

void CollapsibleGroupBox::initialize(const QString &title)
{
    // Intialize the toggle button
    toggleButton = new QPushButton(title);
    toggleButton->setCheckable(true);
    toggleButton->setChecked(false);


    // Style the toggle button
    toggleButton->setStyleSheet("QPushButton {"                             
                                "border: none;"
                                "text-align: left;"
                                "padding: 5px;"
                                "}"
                                "QPushButton:hover {"
                                "font-weight: bold;"
                                "background-color: #3E3E42;"
                                "}");

    // Initialze the content area
    contentArea = new QFrame;
    contentArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // Start with the content area collapsed
    contentArea->setMaximumHeight(0);

    // Create the animation for expanding collapsing
    animation = new QPropertyAnimation(contentArea, "maximumHeight");
    animation->setDuration(200);
    animation->setEasingCurve(QEasingCurve::InOutQuart);

    // Create the main layout
    mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(toggleButton);
    mainLayout->addWidget(contentArea);

    // Connect the toggle button signal
    connect(toggleButton, &QPushButton::toggled, this, &CollapsibleGroupBox::toggle);
}

void CollapsibleGroupBox::setContentLayout(QLayout *contentLayout)
{
    contentArea->setLayout(contentLayout);

    // Calculate the content height
    contentHeight = contentLayout->sizeHint().height();
}

void CollapsibleGroupBox::toggle(bool collapsed)
{
    // Adjust the arrow symbol on the toggle button
    if (collapsed) {
        toggleButton->setText(toggleButton->text().replace("▸", "▾"));
    } else {
        toggleButton->setText(toggleButton->text().replace("▾", "▸"));
    }

    // Set animation direction
    animation->setDirection(collapsed ? QPropertyAnimation::Forward : QPropertyAnimation::Backward);
    animation->setStartValue(0);
    animation->setEndValue(contentHeight);

    animation->start();
}
