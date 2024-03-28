/********************************************************************************
** Form generated from reading UI file 'note.ui'
**
** Created by: Qt User Interface Compiler version 6.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NOTE_H
#define UI_NOTE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolButton>

QT_BEGIN_NAMESPACE

class Ui_Note
{
public:
    QFormLayout *formLayout;
    QTextEdit *textEdit;
    QToolButton *Save;

    void setupUi(QDialog *Note)
    {
        if (Note->objectName().isEmpty())
            Note->setObjectName("Note");
        Note->resize(1161, 533);
        Note->setStyleSheet(QString::fromUtf8("border-top-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));"));
        formLayout = new QFormLayout(Note);
        formLayout->setObjectName("formLayout");
        textEdit = new QTextEdit(Note);
        textEdit->setObjectName("textEdit");
        textEdit->setStyleSheet(QString::fromUtf8("gridline-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(193, 128, 255, 255), stop:1 rgba(255, 255, 255, 255));\n"
"font: 14pt \"Ubuntu\";"));

        formLayout->setWidget(0, QFormLayout::SpanningRole, textEdit);

        Save = new QToolButton(Note);
        Save->setObjectName("Save");
        Save->setStyleSheet(QString::fromUtf8("selection-color: qlineargradient(spread:pad, x1:0.269444, y1:0.534, x2:1, y2:1, stop:0 rgba(225, 47, 215, 255), stop:1 rgba(255, 255, 255, 255));"));

        formLayout->setWidget(1, QFormLayout::LabelRole, Save);


        retranslateUi(Note);

        QMetaObject::connectSlotsByName(Note);
    } // setupUi

    void retranslateUi(QDialog *Note)
    {
        Note->setWindowTitle(QCoreApplication::translate("Note", "Notepad", nullptr));
        Save->setText(QCoreApplication::translate("Note", "Save", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Note: public Ui_Note {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NOTE_H
