#include "database.h"
#include <iostream>

Database::Database(const std::string& db_name) {
    sqlite3_open(db_name.c_str(), &db);
}

Database::~Database() {
    sqlite3_close(db);
}

bool Database::init() {
    const char* sql =
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "full_name TEXT,"
        "login TEXT UNIQUE,"
        "phone TEXT);"

        "CREATE TABLE IF NOT EXISTS messages ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "type TEXT,"
        "text TEXT,"
        "datetime TEXT,"
        "user_id INTEGER);";

    char* err;
    return sqlite3_exec(db, sql, 0, 0, &err) == SQLITE_OK;
}

bool Database::insertUser(const User& user) {
    std::string sql = "INSERT INTO users(full_name, login, phone) VALUES(?,?,?)";

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);

    sqlite3_bind_text(stmt, 1, user.full_name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, user.login.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, user.phone.c_str(), -1, SQLITE_STATIC);

    return sqlite3_step(stmt) == SQLITE_DONE;
}

bool Database::insertMessage(const Message& msg) {
    std::string sql = "INSERT INTO messages(type, text, datetime, user_id) VALUES(?,?,?,?)";

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);

    sqlite3_bind_text(stmt, 1, msg.type.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, msg.text.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, msg.datetime.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, msg.user_id);

    return sqlite3_step(stmt) == SQLITE_DONE;
}

std::vector<Message> Database::getMessages() {
    std::vector<Message> result;

    std::string sql = "SELECT * FROM messages";
    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Message msg;
        msg.id = sqlite3_column_int(stmt, 0);
        msg.type = (char*)sqlite3_column_text(stmt, 1);
        msg.text = (char*)sqlite3_column_text(stmt, 2);
        msg.datetime = (char*)sqlite3_column_text(stmt, 3);
        msg.user_id = sqlite3_column_int(stmt, 4);

        result.push_back(msg);
    }

    return result;
}

std::vector<Message> Database::searchMessages(const std::string& keyword) {
    std::vector<Message> result;

    std::string sql = "SELECT * FROM messages WHERE text LIKE ?";
    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);

    std::string pattern = "%" + keyword + "%";
    sqlite3_bind_text(stmt, 1, pattern.c_str(), -1, SQLITE_STATIC);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Message msg;
        msg.id = sqlite3_column_int(stmt, 0);
        msg.type = (char*)sqlite3_column_text(stmt, 1);
        msg.text = (char*)sqlite3_column_text(stmt, 2);
        msg.datetime = (char*)sqlite3_column_text(stmt, 3);
        msg.user_id = sqlite3_column_int(stmt, 4);

        result.push_back(msg);
    }

    return result;
}

bool Database::deleteMessage(int id) {
    std::string sql = "DELETE FROM messages WHERE id=?";
    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
    sqlite3_bind_int(stmt, 1, id);

    return sqlite3_step(stmt) == SQLITE_DONE;
}

bool Database::updateMessage(const Message& msg) {
    std::string sql = "UPDATE messages SET text=? WHERE id=?";
    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);

    sqlite3_bind_text(stmt, 1, msg.text.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, msg.id);

    return sqlite3_step(stmt) == SQLITE_DONE;
}