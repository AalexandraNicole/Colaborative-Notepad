#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <vector>
#include <map>
#include <mutex>
#include <shared_mutex>
#include <syncstream>
#include <string_view>
#include <memory>
#include <algorithm>
#include <thread>
#include <sqlite3.h>
#include "OTDocument.h"

constexpr int PORT = 12345;
constexpr int MAX_CLIENTS = 100;
constexpr int MAX_DOC_CLIENTS = 2;
constexpr char DATABASE_NAME[] = "file_database.db";

void loadDocumentsFromDatabase();

std::vector<int> clients;
std::map<std::string,std::vector<int>> conectedClients;
std::shared_mutex documentMutex;
std::shared_mutex conectedClientsMutex;
sqlite3 *db;
std::string document;  // Documentul partajat între clienți
std::map<std::string, std::string>documents; //documentele din baza de date
std::map<std::string, OTDocument> otDocuments;
bool saving = false;

void sendDocumentList(int clientSocket) {
    std::string message = "";
    loadDocumentsFromDatabase();
    // Logica pentru a interoga baza de date și a obține lista de documente
    sqlite3_stmt* statement;
    std::string query = "SELECT id, document_name FROM Docs";
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &statement, nullptr) == SQLITE_OK) {
        while (sqlite3_step(statement) == SQLITE_ROW) {
            const unsigned char* documentName = sqlite3_column_text(statement, 1);
            if(documentName){
                message += reinterpret_cast<const char*>(documentName) + std::string(",");
            }
        }
        sqlite3_finalize(statement);
    }else {
        std::cerr << "Error: " << sqlite3_errmsg(db)<<std::endl;
    }

    // Trimite lista de documente către client
    if (send(clientSocket, message.c_str(), message.size(), 0) == -1)
    {
        std::cerr << "Eroare la trimiterea datelor la client\n";
    }
    
}

void broadcastMessage(int senderSocket, const std::string& message, std::string docName) {
    std::shared_lock<std::shared_mutex> lock(documentMutex);
    std::string transformed_message = message;
    for (int client : conectedClients[docName]) {
        if (client != senderSocket) {
            send(client, transformed_message.c_str(), transformed_message.size(), 0);
        }
    }
}

void loadDocumentsFromDatabase() {
    std::shared_lock<std::shared_mutex> lock(documentMutex);
    sqlite3_stmt* stmt;
    std::string query = "SELECT id, document_name, document_content FROM Docs";
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            const unsigned char* loadedDocumentName = sqlite3_column_text(stmt, 1);
            const unsigned char* loadedDocumentContent = sqlite3_column_text(stmt, 2);
            int id = sqlite3_column_int(stmt, 0);

            if (loadedDocumentName && loadedDocumentContent) {
                std::cout <<"Name: " <<loadedDocumentName << " ID: " << id  << " CONTENT: " << loadedDocumentContent << std::endl;
                OTDocument newOTDocument(reinterpret_cast<const char*>(loadedDocumentContent));
                otDocuments[reinterpret_cast<const char*>(loadedDocumentName)] = newOTDocument;
                documents[reinterpret_cast<const char*>(loadedDocumentName)] = reinterpret_cast<const char*>(loadedDocumentContent);
                if (otDocuments[reinterpret_cast<const char*>(loadedDocumentName)].document == "")
                {
                    otDocuments[reinterpret_cast<const char*>(loadedDocumentName)].document = " ";
                }
                
                //std::cout << documents[reinterpret_cast<const char*>(loadedDocumentName)];
            }
        }
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Error: Nu s-a putut pregati stmt\n" << sqlite3_errmsg(db)<<std::endl;
    }
}

// Function to drop the Docs table
void dropTable() {
    const char* dropTableQuery = "DROP TABLE IF EXISTS Docs";

    // Execute the query
    if (sqlite3_exec(db, dropTableQuery, nullptr, nullptr, nullptr) != SQLITE_OK) {
        std::cerr << "Error: Couldn't drop table! " << sqlite3_errmsg(db) << std::endl;
    }
}

//Functie pentru a sterge un document
void deletStatement(int idToDelete){

    while(idToDelete >= 0){
        const char* deleteQuery = "DELETE FROM Docs WHERE id = ?";

        // Prepare the DELETE statement
        sqlite3_stmt* deleteStatement;
        if (sqlite3_prepare_v2(db, deleteQuery, -1, &deleteStatement, nullptr) == SQLITE_OK) {
            // Bind the parameter (ID value) to the statement
            sqlite3_bind_int(deleteStatement, 1, idToDelete);

            // Execute the DELETE statement
            if (sqlite3_step(deleteStatement) == SQLITE_DONE) {
                std::cout << "Row deleted successfully" << std::endl;
            } else {
                std::cerr << "Error deleting row: " << sqlite3_errmsg(db) << std::endl;
            }

            // Finalize the statement
            sqlite3_finalize(deleteStatement);
        } else {
            std::cerr << "Error preparing DELETE statement: " << sqlite3_errmsg(db) << std::endl;
        }
        idToDelete --;
    }
}

void saveDocumentToDatabase(std:: string documentName, std:: string document_content) {
    std::string query = "UPDATE Docs SET document_content = ? WHERE document_name = ?";  
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, document.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, documentName.c_str(), -1, SQLITE_STATIC);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }else {
        std::cerr << "Error: " << sqlite3_errmsg(db)<<std::endl;
    }

    if (sqlite3_exec(db, "BEGIN", nullptr, nullptr, nullptr) == SQLITE_OK) {
        sqlite3_stmt* statement;
        query = "UPDATE Docs SET document_content = ? WHERE document_name = ?";
        if (sqlite3_prepare_v2(db, query.c_str(), -1, &statement, nullptr) == SQLITE_OK) {
            sqlite3_bind_text(statement, 2, documentName.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(statement, 1, document_content.c_str(), -1, SQLITE_STATIC);

            if (sqlite3_step(statement) == SQLITE_DONE) {
                if (sqlite3_exec(db, "COMMIT", nullptr, nullptr, nullptr) != SQLITE_OK) {
                    std::cerr << "Error: Couldn't commit transaction! " << sqlite3_errmsg(db) << std::endl;
                }
            }else {
                // Rollback the transaction on statement execution failure
                std::cerr << "Error: Couldn't execute the statement! " << sqlite3_errmsg(db) << std::endl;
                if (sqlite3_exec(db, "ROLLBACK", nullptr, nullptr, nullptr) != SQLITE_OK) {
                    std::cerr << "Error: Couldn't rollback transaction! " << sqlite3_errmsg(db) << std::endl;
                }
            }
            sqlite3_finalize(statement);
        }
    }else {
        std::cerr <<"Error: Couldn't begin transaction! " << sqlite3_errmsg(db) << std::endl;
    }
}

std::string saveNewDocumentToDatabase(const std::string& documentName) {
    // Verific dacă există deja un document cu același nume în baza de date
    sqlite3_stmt* statement;
    std::string response;
    std::string query = "SELECT COUNT(*) FROM Docs WHERE document_name = ?";
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &statement, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(statement, 1, documentName.c_str(), -1, SQLITE_STATIC);
        if (sqlite3_step(statement) == SQLITE_ROW) {
            int documentCount = sqlite3_column_int(statement, 0);
            if (documentCount > 0) {
                // Documentul există deja
                response = "Document with the same name already exists";
                std::cout << "Error: Document with the same name already exists!" << std::endl;
                sqlite3_finalize(statement);
                return response;
            }
        }
        sqlite3_finalize(statement);
        
    }
    std::string doc_content = "New Doc\n";
    // Adăug noul document în baza de date
    if (sqlite3_exec(db, "BEGIN", nullptr, nullptr, nullptr) == SQLITE_OK) {
        sqlite3_stmt* statement;
        query = "INSERT INTO Docs (document_name) VALUES (?)";
        if (sqlite3_prepare_v2(db, query.c_str(), -1, &statement, nullptr) == SQLITE_OK) {
            sqlite3_bind_text(statement, 1, documentName.c_str(), -1, SQLITE_STATIC);
            if (sqlite3_step(statement) == SQLITE_DONE) {
                if (sqlite3_exec(db, "COMMIT", nullptr, nullptr, nullptr) != SQLITE_OK) {
                    std::cerr << "Error: Couldn't commit transaction! " << sqlite3_errmsg(db) << std::endl;
                    response = "Error: Couldn't commit transaction! ";
                    return response;
                }
            }else {
                // Rollback the transaction on statement execution failure
                std::cerr << "Error: Couldn't execute the statement! " << sqlite3_errmsg(db) << std::endl;
                if (sqlite3_exec(db, "ROLLBACK", nullptr, nullptr, nullptr) != SQLITE_OK) {
                    std::cerr << "Error: Couldn't rollback transaction! " << sqlite3_errmsg(db) << std::endl;
                    response = "Error: Couldn't rollback transaction! ";
                    return response;
                }
            }
            sqlite3_finalize(statement);
        }
    }else {
        std::cerr <<"Error: Couldn't begin transaction! " << sqlite3_errmsg(db) << std::endl;
        response = "Error: Couldn't begin transaction! ";
        return response;
    }
    if (sqlite3_exec(db, "BEGIN", nullptr, nullptr, nullptr) == SQLITE_OK) {
        sqlite3_stmt* statement;
        query = "UPDATE Docs SET document_content = ? WHERE document_name = ?";
        if (sqlite3_prepare_v2(db, query.c_str(), -1, &statement, nullptr) == SQLITE_OK) {
            sqlite3_bind_text(statement, 2, documentName.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(statement, 1, doc_content.c_str(), -1, SQLITE_STATIC);

            if (sqlite3_step(statement) == SQLITE_DONE) {
                if (sqlite3_exec(db, "COMMIT", nullptr, nullptr, nullptr) != SQLITE_OK) {
                    std::cerr << "Error: Couldn't commit transaction! " << sqlite3_errmsg(db) << std::endl;
                    response = "Error: Couldn't commit transaction! ";
                    return response;
                }
            }else {
                // Rollback the transaction on statement execution failure
                std::cerr << "Error: Couldn't execute the statement! " << sqlite3_errmsg(db) << std::endl;
                if (sqlite3_exec(db, "ROLLBACK", nullptr, nullptr, nullptr) != SQLITE_OK) {
                    std::cerr << "Error: Couldn't rollback transaction! " << sqlite3_errmsg(db) << std::endl;
                    response = "Error: Couldn't rollback transaction! ";
                    return response;
                }
            }
            sqlite3_finalize(statement);

        }
    }else {
        std::cerr <<"Error: Couldn't begin transaction! " << sqlite3_errmsg(db) << std::endl;
        response = "Error: Couldn't begin transaction! ";
        return response;
    }
    response = doc_content;
    return response;
}

void update_document(int clientSocket, const std::string& update, std::string documentName){
    std::unique_lock<std::shared_mutex> lock(documentMutex);
    documents[documentName] = update;
    auto& otDocument = otDocuments[documentName];
    otDocument.applyRemoteOperation(update);
    saveDocumentToDatabase(documentName, otDocument.document);
}

void handleClient(int clientSocket) {
    char buffer[1024] = {0};
    ssize_t bytesRead;
    std::string documentName;
    int exit = 0;

    while ((bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
        // Afișează datele primite
        std::cout << "Received data from client " << clientSocket << ": " << buffer << std::endl;
        
        if (strncmp(buffer,"GetDocumentList",15) == 0)
        {
            sendDocumentList(clientSocket);
        }else if (strncmp(buffer,"GetDocumentContent",18) == 0)
        {
            documentName = buffer + 18;
            std::shared_lock<std::shared_mutex> lock(documentMutex);
            auto& otDocument = otDocuments[documentName];
            document = otDocument.document;
            if (conectedClients[documentName].size() >= MAX_DOC_CLIENTS)
            {
                std::string errMsg("Maximum$$$");
                std::cerr << "Conection rejected: Maximum clients reached" << std::endl;
                send(clientSocket, errMsg.c_str(),errMsg.size(),0);
                continue;
            }
            conectedClients[documentName].push_back(clientSocket);
            std::cout << "NUMARUL DE CLIENTI CONECTATI: "<< conectedClients[documentName].size() << std::endl; 

            if(send(clientSocket, document.c_str(),document.size(),0) == -1){
                std::cerr << "Eroare la trimierea document content";
            }
            std::cout << "Document name: "<< documentName << std::endl;
            exit = 0;

        }else if (strncmp(buffer,"Exit$$", 6) == 0)
        {
            exit = 1;
            // Lock the mutex before modifying conectedClients
            std::shared_lock<std::shared_mutex> lock(conectedClientsMutex);

            auto& clientsForDocument = conectedClients[documentName];  // reference for brevity
            auto it = std::find(clientsForDocument.begin(), clientsForDocument.end(), clientSocket);
            
            if (it != clientsForDocument.end()) {
                //conectedClients[documentName].erase(it);
                clientsForDocument.erase(it);
                std::cout << "Client socket removed from conectedClients for document: " << documentName << std:: endl;
                for (int client : clientsForDocument) {
                    std::cout << "Client: " << client << std:: endl;
                }
            } else {
                std::cout << "Client socket not found in conectedClients for document: " << documentName << std::endl;
            }

        }else if (strncmp(buffer, "New Document:", 13) == 0){

            documentName = buffer + 13;
            auto& otDocument = otDocuments[documentName];
            document = otDocument.document;
            std::string response = saveNewDocumentToDatabase(documentName);
            if (response == "New Doc\n")
            {
                std::cout << "Creating New document: "<< documentName << std::endl; 
            }

            if(send(clientSocket, response.c_str(),response.size(),0) == -1){
                    std::cerr << "Eroare la trimierea response";
            }
            loadDocumentsFromDatabase();
            
            //sendDocumentList(clientSocket);

        }else if (strncmp(buffer, "Save", 4) == 0){
            std::cout << "CLIENT " << clientSocket << " is saving document " << documentName << std::endl;
            std::string response = "Save";
            broadcastMessage(clientSocket, buffer, documentName);
            saving = true;

        }else if (strncmp(buffer, "Done_Save", 9) == 0){
            std::cout << "DONE SAVE \n";
            std::string response = "Done_Save";
            broadcastMessage(clientSocket, buffer, documentName);
            saving = false;
        }else if(!saving){

            //Aclualizarea documentului
            {
                update_document(clientSocket, buffer, documentName);
            }
            broadcastMessage(clientSocket, buffer, documentName);

        }else if(saving){
            std::string response = "Save";
             if(send(clientSocket, response.c_str(),response.size(),0) == -1){
                    std::cerr << "Eroare la trimierea response";
            }
        }
        // Curătarea buffer-ului pentru a fi pregătit pentru următoarea citire
        memset(buffer, 0, sizeof(buffer));
    }

    std::unique_lock<std::shared_mutex> lock(documentMutex);

    // Când un client se deconectează, eliminăm descriptorul de socket din lista de clienți si din clientii conectati la document daca deconectarea sa facut fortat fata EXIT$$
    if (exit == 0)
    {
        auto itt = std::find(conectedClients[documentName].begin(), conectedClients[documentName].end(), clientSocket);
        if (itt != conectedClients[documentName].end()) {
            conectedClients[documentName].erase(itt);
        }    
    }

    auto it = std::find(clients.begin(), clients.end(), clientSocket);
    if (it != clients.end()) {
        clients.erase(it);
        close(clientSocket);
        std::cout << "Client " << clientSocket << " disconnected" << std::endl;
    }
}

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddress, clientAddress;
    socklen_t clientAddressLength = sizeof(clientAddress);
    char ipBuffer[INET_ADDRSTRLEN];
    const char* query;
    sqlite3_stmt* statement;

    // Deschide conexiunea la baza de date SQLite
    if (sqlite3_open(DATABASE_NAME, &db) != SQLITE_OK) {
        std::cerr << "Error: Couldn't open database!" << std::endl;
        return -1;
    }

    // Creează tabela pentru fișiere, dacă nu există deja
    std::string createTableQuery = "CREATE TABLE IF NOT EXISTS Docs (id INTEGER PRIMARY KEY, document_name TEXT, document_content TEXT)";
    if (sqlite3_exec(db, createTableQuery.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        std::cerr << "Error: Couldn't create table!" << std::endl;
        return -1;
    }


    //stregem documentele
    //deletStatement(4);

    //Introduc un document inital
    //saveNewDocumentToDatabase("Initial");

    // Încarcă documentele din baza de date la pornirea serverului
    loadDocumentsFromDatabase();

    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        std::cerr << "Socket creation failed" << std::endl;
        return -1;
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(PORT);

    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Binding failed" << std::endl;
        return -1;
    }

    if (listen(serverSocket, 3) < 0) {
        std::cerr << "Listening failed" << std::endl;
        return -1;
    }

    std::cout << "Server listening on port " << PORT << std::endl;
    std::vector<std::thread> threads;

    while (true) {
        if ((clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLength)) < 0) {
            std::cerr << "Acceptance failed" << std::endl;
            continue;
        }

        clients.push_back(clientSocket);

        std::cout << "Client " << clientSocket << " connected: " << inet_ntop(AF_INET, &(clientAddress.sin_addr), ipBuffer, INET_ADDRSTRLEN) << ":" << ntohs(clientAddress.sin_port) << std::endl;

        threads.push_back(std::thread(handleClient, clientSocket));
    }

    for (auto& thread : threads){
        thread.join();
    }
    // Închide conexiunea la baza de date SQLite înainte de închiderea programului
    sqlite3_close(db);

    return 0;
}
