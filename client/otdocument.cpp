#include "otdocument.h"

OTDocument::OTDocument(const QString& document) : document(document) {}

QString OTDocument::transform(const QString& remoteOperation) {
    // Parse the remote operation
    QChar opType = remoteOperation[0];
    int remotePosition = remoteOperation.mid(1, remoteOperation.indexOf(":") - 1).toInt();
    qDebug() << remotePosition;
    QString remoteText = remoteOperation.mid(remoteOperation.indexOf(":") + 1);
    qDebug() << remoteText;
    // Transform the remote operation with the local operations
    for (const Operation& localOperation : operations) {
        // Apply transformation rules based on operation types
        if (opType == 'I' && localOperation.type == 'I') {
            if (remotePosition > localOperation.position ||
                (remotePosition == localOperation.position && opType == 'I')) {
                remotePosition += localOperation.text.length();
            }
        } else if (opType == 'D' && localOperation.type == 'I') {
            if (remotePosition >= localOperation.position) {
                remotePosition -= localOperation.text.length();
            }
        } else if (opType == 'I' && localOperation.type == 'D') {
            if (remotePosition > localOperation.position) {
                remotePosition += localOperation.text.length();
            }
        }
    }
    qDebug() << remotePosition;

    // Create and return the transformed remote operation
    return QString(opType) + QString::number(remotePosition) + ":" + remoteText;
}

void OTDocument::applyLocalOperation(const QString& localOperation) {
    // Parse the local operation
    QChar opType = localOperation[0];
    int localPosition = localOperation.mid(1, localOperation.indexOf(":") - 1).toInt();
    QString localText = localOperation.mid(localOperation.indexOf(":") + 1);

    // Apply the local operation to the local document
    Operation op;
    op.type = opType;
    op.position = localPosition;
    op.text = localText;

    if (opType == 'I') {
        applyInsert(op);
    } else if (opType == 'D') {
        applyDelete(op);
    }
}

QString OTDocument::applyRemoteOperation(const QString& remoteOperation) {
    // Parse the remote operation
    QChar opType = remoteOperation[0];
    int remotePosition = remoteOperation.mid(1, remoteOperation.indexOf(":") - 1).toInt();
    QString remoteText = remoteOperation.mid(remoteOperation.indexOf(":") + 1);

    // Transform the remote operation with the local operations
    for (const Operation& localOperation : operations) {
        // Apply transformation rules based on operation types
        if (opType == 'I' && localOperation.type == 'I') {
            if (remotePosition > localOperation.position ||
                (remotePosition == localOperation.position && opType == 'I')) {
                remotePosition += localOperation.text.length();
            }
        } else if (opType == 'D' && localOperation.type == 'I') {
            if (remotePosition >= localOperation.position) {
                remotePosition -= localOperation.text.length();
            }
        } else if (opType == 'I' && localOperation.type == 'D') {
            if (remotePosition > localOperation.position) {
                remotePosition += localOperation.text.length();
            }
        }
    }

    // Create and return the transformed remote operation
    return QString(opType) + QString::number(remotePosition) + ":" + remoteText;
}

void OTDocument::applyInsert(const Operation& op) {
    // Apply an insertion operation to the local document
    document.insert(op.position, op.text);

    // Adjust the positions of subsequent operations
    for (Operation& localOp : operations) {
        if (localOp.position >= op.position) {
            localOp.position += op.text.length();
        }
    }

    // Add the operation to the local operations list
    operations.append(op);
}

QString OTDocument::extractModification(const QString& localOperation) const
{
    // Parse the local operation
    QChar opType = localOperation[0];
    int position = localOperation.mid(1, localOperation.indexOf(":") - 1).toInt();
    QString const text = localOperation.mid(localOperation.indexOf(":") + 1);

    // Extract the modification part based on the operation type
    QString modification;
    if (opType == 'I')
    {
        modification = "I" + QString::number(position) + ":" + text.mid(position, 1);
    }
    else if (opType == 'D')
    {
        // If it's a deletion, extract the deleted part
        modification = localOperation;
    }
    qDebug() << "Modifications: "<<modification;
    return modification;
}

void OTDocument::applyDelete(const Operation& op) {
    // Apply a deletion operation to the local document
    document.remove(op.position, op.text.length());

    // Adjust the positions of subsequent operations
    for (Operation& localOp : operations) {
        if (localOp.position >= op.position) {
            localOp.position -= op.text.length();
        }
    }

    // Add the operation to the local operations list
    operations.append(op);
}

QString OTDocument::getDocument() const {
    return document;
}
