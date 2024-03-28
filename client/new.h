#ifndef NEW_H
#define NEW_H

#include <QDialog>
#include "note.h"

namespace Ui {
class New;
}

class New : public QDialog
{
    Q_OBJECT

public:
    explicit New(int socketfd, QWidget *parent = nullptr);
    ~New();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::New *ui;
    QString documentName;
    int socketfd;
    QString content;
    QString SuccOrFailed;
};

#endif // NEW_H
