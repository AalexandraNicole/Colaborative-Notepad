#include "new.h"
#include "ui_new.h"

New::New(int socketfd, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::New)
    , socketfd(socketfd)
{
    ui->setupUi(this);
    this->setWindowTitle("Denumeste fisier");
}

New::~New()
{
    delete ui;
}

void New::on_buttonBox_accepted()
{
    documentName = ui->text->toPlainText();
    if(documentName.isEmpty()){
        QMessageBox::about(this,"Warning","Denumiti fisierul");
    }else{
        const char* docName = documentName.toUtf8().constData();
        std::string newDocumentCommand = std::string("New Document:") + docName;
        // Send "New Document" command
        ssize_t sentBytesCommand = ::send(socketfd, newDocumentCommand.c_str(), newDocumentCommand.size(), 0);
        if (sentBytesCommand == -1) {
            perror("Error sending 'New Document' command");
            ::close(socketfd);
            exit(1);
        }
        fflush(stdout);
        char buffer[1024];

        while (true) {
            ssize_t bytesRead = ::recv(socketfd, buffer, sizeof(buffer), 0);

            if (bytesRead > 0) {
                // Read the document content from the server
                QString documentContent = QString::fromUtf8(buffer, bytesRead);

                fcntl(socketfd, F_SETFL, O_NONBLOCK);
                this->hide();
                // Pass the existing socket descriptor to the Note constructor
                if(documentContent  == "New Doc\n"){
                    Note txtEdit(documentContent, socketfd, documentName, this);
                }
                else{
                    QMessageBox::critical(this, "Error: ", documentContent);
                    close();
                }
                break;
            } else if (bytesRead == 0) {
                // Connection closed by the peer
                break;
            } else {
                // Handle errors, including EAGAIN or EWOULDBLOCK
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    // Wait for data to become available
                    continue;
                } else {
                    perror("Error receiving data from server");
                    ::close(socketfd);
                    exit(1);
                }
            }
        }

    }
}

