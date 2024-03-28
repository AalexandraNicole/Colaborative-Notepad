#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , txtEdit(nullptr)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //Conection
    // Create a socket descriptor
    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd == -1) {
        perror("Error creating socket");
        exit(1);
    }

    // Set up server address
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to the server
    if (::connect(socketfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        QMessageBox::critical(this, "Error", " Server not responding!");
        perror("Error connecting to server");
        ::close(socketfd);
        exit(1);
    }

    qDebug() << "Connected to server";

    // Send "GetDocumentList" to the server
    const char* message = "GetDocumentList";
    if (::send(socketfd, message, strlen(message), 0) == -1) {
        perror("Error sending data");
        ::close(socketfd);
        exit(1);
    }

    char buffer[1024]; // Adjust the buffer size as needed

    // Receive document list from the server
    ssize_t bytesRead = ::recv(socketfd, buffer, sizeof(buffer), 0);
    if (bytesRead == -1) {
        perror("Error receiving data");
        ::close(socketfd);
        exit(1);
    }

    QString documentList = QString::fromUtf8(buffer, bytesRead);
    QStringList documents = documentList.split(',', Qt::SkipEmptyParts);

    // Display the received document list
    foreach (const QString& document, documents) {
        ui->listWidget->addItem(document);
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::handleNoteClose(){
    qDebug() << "NOTE CLOSED: " ;
    documentList();
    if (txtEdit) {
        delete txtEdit;
        txtEdit = nullptr;
    }
}

void MainWindow::receiveDocumentContent()
{
    // Send "GetDocumentContent" to the server
    QString msg = "GetDocumentContent" + documentName;
    QByteArray message = msg.toUtf8();
    if (::send(socketfd, message.constData(), message.size(), 0) == -1) {
        perror("Error sending data");
        ::close(socketfd);
        exit(1);
    }
    fflush(stdout);
    char buffer[1024];
    ssize_t bytesRead = 0;

    while (true) {
        bytesRead = ::recv(socketfd, buffer, sizeof(buffer), 0);

        if (bytesRead > 0) {
            // Read the document content from the server
            QString documentContent = QString::fromUtf8(buffer, bytesRead);
            // Process the document content...
            if (documentContent == "Maximum$$$") {
                QMessageBox::critical(this, "Error", "Maximum clients on the same document");
                return;
            }
            fcntl(socketfd, F_SETFL, O_NONBLOCK);
            // Pass the existing socket descriptor to the Note constructor
            this->hide();
            txtEdit = new Note(documentContent, socketfd, documentName, this);
            handleNoteClose();
            this->show();

            break;  // Break the loop since data is received
        } else if (bytesRead == 0) {
            // Connection closed by the peer
            break;
        } else {
            // Handle errors, including EAGAIN or EWOULDBLOCK
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                // Wait for data to become available (you can use select or sleep)
                continue;
            } else {
                // Handle other errors
                QMessageBox::critical(this, "Error", "Failed to receive document content from the server.");
                perror("Error receiving data");
                ::close(socketfd);
                exit(1);
            }
        }
    }
}

void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    qDebug() << "Double-clicked on item: " << item->text();
    documentName = item->text();
    receiveDocumentContent();
}

void MainWindow::documentList(){
    ui->listWidget->clear();
    // Send "GetDocumentList" to the server
    const char* message = "GetDocumentList";
    if (::send(socketfd, message, strlen(message), 0) == -1) {
        perror("Error sending data");
        ::close(socketfd);
        exit(1);
    }

    char buffer[1024];

    while (true) {
        ssize_t bytesRead = ::recv(socketfd, buffer, sizeof(buffer), 0);

        if (bytesRead > 0) {
            // Data received, process and break the loop
            QString documentList = QString::fromUtf8(buffer, bytesRead);
            QStringList documents = documentList.split(',', Qt::SkipEmptyParts);

            // Display the received document list
            foreach (const QString& document, documents) {
                ui->listWidget->addItem(document);
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

void MainWindow::on_actionNew_triggered()
{
    this->hide();
    New newDocument(socketfd);
    newDocument.exec();
    handleNoteClose();
    this->show();
}

