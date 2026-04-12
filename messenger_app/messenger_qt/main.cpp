#include <iostream>
#include "database.h"

int main() {
    Database db("messages.db");

    if (!db.init()) {
        std::cout << "DB error\n";
        return 1;
    }

    User user{ 0, "Ivan Ivanov", "ivan", "+123" };
    db.insertUser(user);

    Message msg{ 0, "in", "Hello world", "2026-04-11", 1 };
    db.insertMessage(msg);

    auto list = db.getMessages();

    for (auto& m : list) {
        std::cout << m.id << " " << m.text << std::endl;
    }

    return 0;
}