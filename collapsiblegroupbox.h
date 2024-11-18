#ifndef COLLAPSIBLEGROUPBOX_H
#define COLLAPSIBLEGROUPBOX_H

//=================================================================================================

#include <QWidget>
#include <QPushButton>
#include <QFrame>
#include <QVBoxLayout>
#include <QPropertyAnimation>

//=================================================================================================

class CollapsibleGroupBox : public QWidget
{
    Q_OBJECT
public:
    explicit CollapsibleGroupBox(QWidget *parent = nullptr);
    explicit CollapsibleGroupBox(const QString &title, QWidget *parent = nullptr);
    
    void setContentLayout(QLayout *contentLayout);
    
private slots:
    void toggle(bool collapsed);

private:
    void initialize(const QString &title);

    QPushButton *toggleButton;
    QFrame *contentArea;
    QVBoxLayout *mainLayout;
    QPropertyAnimation *animation;
    
    // int collapsedHeight;
    int contentHeight;
};

//=================================================================================================
#endif // COLLAPSIBLEGROUPBOX_H
