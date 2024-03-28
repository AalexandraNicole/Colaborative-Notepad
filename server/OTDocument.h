#ifndef OTDOCUMENT_H
#define OTDOCUMENT_H

#include <iostream>
#include <vector>

class OTDocument {
public:
    std::string document;
    std::vector<std::string> operations;

    // Constructor to initialize the document
    OTDocument(const std::string& initialDoc = "");

    // Apply a remote operation to the document
    void applyRemoteOperation(const std::string& remoteOperation);

    // Transform the remote operation against local operations
    std::string transform(const std::string& remoteOperation);
    std::string transformInsert(const std::string& remoteOperation);
    std::string transformRemove(const std::string& remoteOperation);

    // Apply the operation to the document
    std::string applyOperation(std::string doc, const std::string& operation);
};

#endif // OTDOCUMENT_H
