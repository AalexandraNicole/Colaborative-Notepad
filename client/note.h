#ifndef NOTE_H
#define NOTE_H

#include <QDialog>
#include <QTextEdit>
#include <QSocketNotifier>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QHostAddress>
#include <fcntl.h>
#include "note.h"
#include <QDialog>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QHostAddress>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "otdocument.h"

namespace Ui {
class Note;
}

class Note : public QDialog
{
    Q_OBJECT

public:
    explicit Note(QString content, int socketfd, const QString& documentName, QWidget* parent = nullptr);

    ~Note();

signals:
    void socketClosed();
    void readyRead();
    void connected();
    void disconnected();
    void errorOccurred();

private slots:
    void sendLocalOperationToServer();
    void receiveData();
    void onConnected();
    void onDisconnected();
    void onError();
    void socketActivated(int socket);

    void on_Note_finished(int result);

    void on_Save_clicked(bool checked);

private:
    Ui::Note* ui;
    int socketfd;
    bool save = false;
    bool exitSent = false;
    bool remote = false;
    QSocketNotifier* socketNotifier;
    OTDocument* otDocument;
    QString documentName;
    QString content;

    void writeData(const QByteArray& data);
};

#endif // NOTE_H
