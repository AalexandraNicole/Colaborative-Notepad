#include "note.h"
#include "ui_note.h"


Note::Note(QString content, int socketfd, const QString& documentName, QWidget* parent)
    : QDialog(parent),
    ui(new Ui::Note),
    socketfd(socketfd),
    documentName(documentName),
    content(content)
{

    ui->setupUi(this);
    this->setWindowTitle(documentName);
    int optval;

    socklen_t optlen = sizeof(optval);
    if (getsockopt(socketfd, SOL_SOCKET, SO_ERROR, &optval, &optlen) == -1) {
        perror("Error getting socket options");
        // Handle error, close socket, etc.
    }


    if (optval != 0) {
        qDebug() << "Socket error: " << strerror(optval);
        // Handle error, close socket, etc.
    }
    // Initialize the OTDocument with the initial content
    otDocument = new OTDocument(content);

    ui->textEdit->setText(content);


    // Connect signals and slots using standard BSD socket functions
    connect(this, &Note::finished, this, &Note::on_Note_finished);

    connect(ui->textEdit, &QTextEdit::textChanged, this, &Note::sendLocalOperationToServer);

    // Connect readyRead signal to receiveData slot
    connect(this, &Note::readyRead, this, &Note::receiveData);

    // Connect the connected signal
    connect(this, &Note::connected, this, &Note::onConnected);

    // Connect the disconnected signal
    connect(this, &Note::disconnected, this, &Note::onDisconnected);

    // Connect the errorOccurred signal
    connect(this, &Note::errorOccurred, this, &Note::onError);

    // Enable socket notifications in the event loop
    fcntl(socketfd, F_SETFL, O_NONBLOCK);
    socketNotifier = new QSocketNotifier(socketfd, QSocketNotifier::Read, this);
    connect(socketNotifier, &QSocketNotifier::activated, this, &Note::socketActivated);

    // Start the event loop
    exec();
}

Note::~Note()
{
    // Stop the event loop before deleting the object
    reject();

    delete ui;
    delete otDocument;
}

void Note::sendLocalOperationToServer()
{
    if(save){
        ui->textEdit->undo();
        QMessageBox::warning(this, "Wait...", "Cineva vrea sa salveze fisierul");
        return;
    }
    if(!remote){
        QTextCursor cursor(ui->textEdit->textCursor());

        QString newContent = ui->textEdit->toPlainText();
        qDebug() << "New content: " << newContent;
        qDebug() << "Content: " << content;
        QString localOperation;
        if (newContent.length() < content.length())
        {
            // Handle deletion
            int selectionStart = cursor.selectionStart();
            localOperation = "D" + QString::number(selectionStart) + ":1";
            qDebug() << "LocalOperation DELETE:" << localOperation;
        }
        else
        {
            // Handle insertion
            localOperation = "I" + QString::number(cursor.position()-1) + ":" + ui->textEdit->toPlainText();
            qDebug() << "LocalOperation INSERT: "<<localOperation;
        }

        // Apply the local operation to the OTDocument
        otDocument->applyLocalOperation(localOperation);

        //QString transformedOperation = otDocument->transform(localOperation);
        //QByteArray data = transformedOperation.toUtf8();

        QString modification = otDocument->extractModification(localOperation);
        QByteArray data = modification.toUtf8();

        // Use writeData function for non-blocking socket write
        writeData(data);
        content = newContent;
    }
}

void Note::writeData(const QByteArray &data)
{
    // Write data to the socket
    ssize_t bytesWritten = write(socketfd, data.constData(), data.size());

    if (bytesWritten == -1)
    {
        perror("Error writing data to server");
        ::close(socketfd);
        emit disconnected();
    }
}

void Note::receiveData()
{
    char buffer[1024];
    // Read data from the socket
    ssize_t bytesRead = read(socketfd, buffer, sizeof(buffer));

    if (bytesRead > 0)
    {
        QString remoteOperation = QString::fromUtf8(buffer, bytesRead);

        if(remoteOperation == "Save"){
            qDebug() << "Someone is saving...";
            save = true;
            return;
        }
        if(remoteOperation == "Done_Save"){
            qDebug() << "Done saving...";
            save = false;
            return;
        }
        //QString localOperation = otDocument->applyRemoteOperation(remoteOperation);
        QString localOperation = remoteOperation;
        qDebug() << "Remote Operation: " << localOperation;
        otDocument->applyLocalOperation(localOperation);
        // Update text in QTextEdit after applying remote operation
        remote = true;
        QChar opType = remoteOperation[0];
        int remotePosition = remoteOperation.mid(1, remoteOperation.indexOf(":") - 1).toInt();
        QString character = remoteOperation.mid(remoteOperation.indexOf(":") + 1);

        QTextCursor mainCursor = ui->textEdit->textCursor();
        QTextCharFormat format;
        format.setForeground(QColor(Qt::red));
        mainCursor.mergeCharFormat(format);
        ui->textEdit->setTextCursor(mainCursor);
        QTextCursor secondCursor(ui->textEdit->document());

        if (opType == 'I') {
            // Set the cursor position to the desired position
            secondCursor.setPosition(remotePosition);

            // Insert the character at the specified position
            secondCursor.insertText(character);

            secondCursor.setPosition(remotePosition + 1);

            // Set the updated cursor to the text edit
            ui->textEdit->setTextCursor(secondCursor);
        } else if (opType == 'D') {
            if(remotePosition - 1 == -1){
                ui->textEdit->clear();
            }else {
                // Set the cursor position to the desired position
                secondCursor.setPosition(remotePosition - 1);

                // Remove the character at the specified position
                secondCursor.movePosition(QTextCursor::NextCharacter);
                secondCursor.deleteChar();

                // Set the updated cursor to the text edit
                ui->textEdit->setTextCursor(secondCursor);
            }
        }
    }
    else if (bytesRead == 0)
    {
        // Connection closed by the server
        qDebug() << "Disconnected from server";
        ::close(socketfd);
        emit disconnected();
    }
    else
    {
        perror("Error receiving data from server");
        ::close(socketfd);
        emit errorOccurred();
    }
    content = ui->textEdit->toPlainText();
    remote = false;
}

void Note::socketActivated(int /*socket*/)
{
    // Emit the readyRead signal when data is available on the socket
    emit readyRead();
}

void Note::onConnected()
{
    qDebug() << "Connected to server";
    emit connected();
}

void Note::onDisconnected()
{
    qDebug() << "Disconnected from server";
    ::close(socketfd);
    emit disconnected();
}

void Note::onError()
{
    qDebug() << "Socket error";
    ::close(socketfd);
    emit errorOccurred();
}

void Note::on_Note_finished(int result)
{

    if (result == QDialog::Accepted) {
        qDebug() << "QDialog::Accepted";
        // Additional actions on dialog acceptance if needed
    }
    if (result == QDialog::Rejected){
        if (!exitSent) {
            qDebug() << "QDialog::Rejected";
            writeData("Exit$$");
            exitSent = true;  // Set flag to indicate that "Exit$$" has been sent
            close();
        }
    }
}


void Note::on_Save_clicked(bool checked)
{
    QString saveClicked = "Save";
    QByteArray data = saveClicked.toUtf8();
    save = true;

    // Use writeData function for non-blocking socket write
    writeData(data);

    QString documentName = QFileDialog::getSaveFileName(this, "Salveaza fisierul");
    QFile file(documentName);

    if (!file.open(QFile::WriteOnly | QFile::Text | QIODevice::Truncate | QIODevice::Text))
    {
        QMessageBox::warning(this, "Err", "Nu puteti salva fisierul");
        save = false;
        QString saveClickedDone = "Done_Save";
        QByteArray data2 = saveClickedDone.toUtf8();

        // Use writeData function for non-blocking socket write
        writeData(data2);
        return;
    }

    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.flush();
    file.close();
    save = false;
    QString saveClickedDone = "Done_Save";
    QByteArray data2 = saveClickedDone.toUtf8();

    // Use writeData function for non-blocking socket write
    writeData(data2);
}

