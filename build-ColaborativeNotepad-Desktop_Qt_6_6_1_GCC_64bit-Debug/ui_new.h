/********************************************************************************
** Form generated from reading UI file 'new.ui'
**
** Created by: Qt User Interface Compiler version 6.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NEW_H
#define UI_NEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QPlainTextEdit>

QT_BEGIN_NAMESPACE

class Ui_New
{
public:
    QDialogButtonBox *buttonBox;
    QPlainTextEdit *text;

    void setupUi(QDialog *New)
    {
        if (New->objectName().isEmpty())
            New->setObjectName("New");
        New->resize(204, 142);
        buttonBox = new QDialogButtonBox(New);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setGeometry(QRect(30, 80, 139, 24));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        text = new QPlainTextEdit(New);
        text->setObjectName("text");
        text->setGeometry(QRect(10, 30, 181, 41));
        QFont font;
        font.setFamilies({QString::fromUtf8("URW Bookman")});
        font.setPointSize(16);
        font.setBold(false);
        text->setFont(font);

        retranslateUi(New);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, New, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, New, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(New);
    } // setupUi

    void retranslateUi(QDialog *New)
    {
        New->setWindowTitle(QCoreApplication::translate("New", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class New: public Ui_New {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEW_H
