#include "OTDocument.h"

OTDocument::OTDocument(const std::string& initialDoc) : document(initialDoc) {}

void OTDocument::applyRemoteOperation(const std::string& remoteOperation) {
    // Transform the remote operation against local operations
    std::string transformedOperation = transform(remoteOperation);
    std::cout <<"APPLY REMOTE OPERATION: " << transformedOperation << std::endl;
    // Apply the transformed remote operation to the document
    document = applyOperation(document, transformedOperation);
    //document = transformedOperation;
    std::cout << "DocumentOT:  " << document <<std::endl;
}

std::string OTDocument::transform(const std::string& remoteOperation) {
    char opType = remoteOperation[0];

    if (opType == 'I') {
        return transformInsert(remoteOperation);
    } else if (opType == 'D') {
        return transformRemove(remoteOperation);
    }

    // Unsupported operation type
    return remoteOperation;
}

std::string OTDocument::transformInsert(const std::string& remoteOperation) {
    // the format "I[position]:text"
    std::cout << "AICI TRANSFORM INSERT: " << remoteOperation << std::endl;
    int remotePosition = std::stoi(remoteOperation.substr(1, remoteOperation.find(":") - 1));
    std::string remoteText = remoteOperation.substr(remoteOperation.find(":") + 1);

    std::cout << "Debug: Original remotePosition = " << remotePosition << ", remoteText = " << remoteText << std::endl;
    for (const auto& localOperation : operations) {
        char localOpType = localOperation[0];
        int localPosition = std::stoi(localOperation.substr(1, localOperation.find(":") - 1));

        // Conflict handling for insert operations
        if (localOpType == 'I' && remotePosition >= localPosition) {
            remotePosition += localOperation.substr(localOperation.find(":") + 1).length();
        }
    }
    std::cout << "Debug: Transformed remotePosition = " << remotePosition << std::endl;
    //Transform the remote insert operation with the adjusted position
    return "I" + std::to_string(remotePosition) + ":" + remoteText;
    //return remoteText;
}

std::string OTDocument::transformRemove(const std::string& remoteOperation) {
    //the format "D[position]:length"
        std::cout <<"Debug: Operation position: " <<remoteOperation << std::endl;
    int remotePosition = std::stoi(remoteOperation.substr(1, remoteOperation.find(":") - 1));
    int remoteLength = std::stoi(remoteOperation.substr(remoteOperation.find(":") + 1));

    for (const auto& localOperation : operations) {
        char localOpType = localOperation[0];
        int localPosition = std::stoi(localOperation.substr(1, localOperation.find(":") - 1));
        int localLength = std::stoi(localOperation.substr(localOperation.find(":") + 1));

        // Conflict handling for delete operations
        if (localOpType == 'D' && remotePosition >= localPosition) {
            if (remotePosition < localPosition + localLength) {
                // Remote delete operation overlaps with a local delete operation
                int overlapLength = remotePosition + remoteLength - localPosition;
                remotePosition -= overlapLength;
                remoteLength -= overlapLength;
            }
        } else if (localOpType == 'I' && remotePosition >= localPosition) {
            remotePosition -= localLength;
        }
    }

    // Transform the remote delete operation with the adjusted position and length
    return "D" + std::to_string(remotePosition) + ":" + std::to_string(remoteLength);
}


std::string OTDocument::applyOperation(std::string doc, const std::string& operation) {
    char opType = operation[0];
    int position = std::stoi(operation.substr(1, operation.find(":") - 1));
    std::string content = operation.substr(operation.find(":") + 1);

    if (opType == 'I') {
        if (doc.length() == position - 1)
        {
            doc.insert(position - 1, content);
        }else{
        
            doc.insert(position, content);
        }
    } else if (opType == 'D') {
        doc.erase(position, std::stoi(content));
    }

    return doc;
}
