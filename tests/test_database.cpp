#include "../messenger_app/database.h"
#include <iostream>
#include <cassert>

// Простые тесты без полной компиляции SQLite
int main() {
    std::cout << "=== DATABASE TESTS ===" << std::endl;
    
    // Тест 1: Создание объекта базы
    Database db("test.db");
    std::cout << "[OK] Database object created" << std::endl;
    
    // Тест 2: Инициализация
    assert(db.init());
    std::cout << "[OK] Database initialized" << std::endl;
    
    // Тест 3: INSERT
    Message msg{0, "in", "test message", "2026-04-13", 1};
    assert(db.insertMessage(msg));
    std::cout << "[OK] Insert message" << std::endl;
    
    // Тест 4: SELECT
    auto messages = db.getMessages();
    assert(!messages.empty());
    std::cout << "[OK] Select messages, count: " << messages.size() << std::endl;
    
    // Тест 5: SEARCH
    auto found = db.searchMessages("test");
    assert(!found.empty());
    std::cout << "[OK] Search messages" << std::endl;
    
    // Тест 6: UPDATE
    if (!messages.empty()) {
        Message updateMsg = messages[0];
        updateMsg.text = "updated text";
        assert(db.updateMessage(updateMsg));
        std::cout << "[OK] Update message" << std::endl;
    }
    
    // Тест 7: DELETE
    if (!messages.empty()) {
        assert(db.deleteMessage(messages[0].id));
        std::cout << "[OK] Delete message" << std::endl;
    }
    
    std::cout << "\n=== ALL TESTS PASSED ===" << std::endl;
    return 0;
}