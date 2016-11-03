/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include <newpushbutton.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QPushButton *floatButton;
    QGroupBox *btnGroup;
    newPushButton *homeBtn;
    newPushButton *menuBtn;
    newPushButton *returnBtn;
    newPushButton *appBtn;
    QLabel *label;
    QLabel *picLabel;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1280, 720);
        QPalette palette;
        QBrush brush(QColor(0, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(235, 220, 206, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush1);
        QBrush brush2(QColor(255, 255, 255, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Light, brush2);
        QBrush brush3(QColor(245, 237, 230, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Midlight, brush3);
        QBrush brush4(QColor(117, 110, 103, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Dark, brush4);
        QBrush brush5(QColor(157, 147, 137, 255));
        brush5.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        palette.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Active, QPalette::AlternateBase, brush3);
        QBrush brush6(QColor(255, 255, 220, 255));
        brush6.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::Midlight, brush3);
        palette.setBrush(QPalette::Inactive, QPalette::Dark, brush4);
        palette.setBrush(QPalette::Inactive, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush3);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Midlight, brush3);
        palette.setBrush(QPalette::Disabled, QPalette::Dark, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        MainWindow->setPalette(palette);
        MainWindow->setIconSize(QSize(32, 32));
        MainWindow->setTabShape(QTabWidget::Rounded);
        MainWindow->setDockNestingEnabled(true);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        floatButton = new QPushButton(centralWidget);
        floatButton->setObjectName(QStringLiteral("floatButton"));
        floatButton->setGeometry(QRect(20, 30, 86, 74));
        floatButton->setFocusPolicy(Qt::NoFocus);
        floatButton->setIconSize(QSize(64, 64));
        floatButton->setFlat(true);
        btnGroup = new QGroupBox(centralWidget);
        btnGroup->setObjectName(QStringLiteral("btnGroup"));
        btnGroup->setGeometry(QRect(150, 480, 1080, 116));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(btnGroup->sizePolicy().hasHeightForWidth());
        btnGroup->setSizePolicy(sizePolicy);
        btnGroup->setAcceptDrops(false);
        btnGroup->setToolTipDuration(-1);
        btnGroup->setLayoutDirection(Qt::RightToLeft);
        btnGroup->setAutoFillBackground(false);
        btnGroup->setStyleSheet(QStringLiteral(""));
        btnGroup->setFlat(true);
        btnGroup->setCheckable(false);
        homeBtn = new newPushButton(btnGroup);
        homeBtn->setObjectName(QStringLiteral("homeBtn"));
        homeBtn->setGeometry(QRect(565, 10, 220, 100));
        QFont font;
        font.setPointSize(15);
        homeBtn->setFont(font);
        homeBtn->setFocusPolicy(Qt::NoFocus);
        homeBtn->setAutoFillBackground(false);
        homeBtn->setIconSize(QSize(36, 36));
        homeBtn->setFlat(true);
        menuBtn = new newPushButton(btnGroup);
        menuBtn->setObjectName(QStringLiteral("menuBtn"));
        menuBtn->setGeometry(QRect(820, 10, 220, 100));
        menuBtn->setFont(font);
        menuBtn->setFocusPolicy(Qt::NoFocus);
        menuBtn->setIconSize(QSize(36, 36));
        menuBtn->setFlat(true);
        returnBtn = new newPushButton(btnGroup);
        returnBtn->setObjectName(QStringLiteral("returnBtn"));
        returnBtn->setGeometry(QRect(295, 10, 220, 100));
        QFont font1;
        font1.setFamily(QStringLiteral("Ubuntu"));
        font1.setPointSize(15);
        returnBtn->setFont(font1);
        returnBtn->setFocusPolicy(Qt::NoFocus);
        returnBtn->setLocale(QLocale(QLocale::Chinese, QLocale::China));
        returnBtn->setIconSize(QSize(36, 36));
        returnBtn->setFlat(true);
        appBtn = new newPushButton(btnGroup);
        appBtn->setObjectName(QStringLiteral("appBtn"));
        appBtn->setGeometry(QRect(40, 10, 220, 100));
        QFont font2;
        font2.setPointSize(15);
        font2.setBold(false);
        font2.setWeight(50);
        appBtn->setFont(font2);
        appBtn->setFocusPolicy(Qt::NoFocus);
        appBtn->setIconSize(QSize(32, 32));
        appBtn->setFlat(true);
        label = new QLabel(btnGroup);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(0, 0, 1080, 116));
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setAutoFillBackground(false);
        label->setFrameShape(QFrame::NoFrame);
        label->setFrameShadow(QFrame::Plain);
        label->setLineWidth(0);
        label->raise();
        menuBtn->raise();
        appBtn->raise();
        homeBtn->raise();
        returnBtn->raise();
        picLabel = new QLabel(centralWidget);
        picLabel->setObjectName(QStringLiteral("picLabel"));
        picLabel->setGeometry(QRect(0, 0, 1280, 720));
        MainWindow->setCentralWidget(centralWidget);
        picLabel->raise();
        btnGroup->raise();
        floatButton->raise();

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        floatButton->setText(QString());
        btnGroup->setTitle(QString());
        homeBtn->setText(QString());
        menuBtn->setText(QString());
        returnBtn->setText(QString());
        appBtn->setText(QString());
        label->setText(QString());
        picLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
