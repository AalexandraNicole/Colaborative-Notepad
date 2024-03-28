#ifndef OTDOCUMENT_H
#define OTDOCUMENT_H

#include <QString>
#include <QVector>
#include <QDebug>

class OTDocument {
public:
    OTDocument(const QString& document);

    QString transform(const QString& remoteOperation);
    QString applyRemoteOperation(const QString& remoteOperation);
    QString extractModification(const QString& localOperation) const;
    void applyLocalOperation(const QString& localOperation);
    QString getDocument() const;

private:
    QString document;

    struct Operation {
        QChar type;  // 'I' for insert, 'D' for delete
        int position;
        QString text;
    };

    QVector<Operation> operations;

    void applyInsert(const Operation& op);
    void applyDelete(const Operation& op);
};

#endif // OTDOCUMENT_H
