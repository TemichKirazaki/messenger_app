#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>
#include "sqlite3.h"

struct User {
    int id;
    std::string full_name;
    std::string login;
    std::string phone;
};

struct Message {
    int id;
    std::string type;
    std::string text;
    std::string datetime;
    int user_id;
};

class Database {
private:
    sqlite3* db;

public:
    Database(const std::string& db_name);
    ~Database();

    bool init();

    bool insertUser(const User& user);
    bool insertMessage(const Message& msg);

    std::vector<Message> getMessages();
    std::vector<Message> searchMessages(const std::string& keyword);

    bool deleteMessage(int id);
    bool updateMessage(const Message& msg);
};

#endif
