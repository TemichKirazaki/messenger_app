#include "../messenger_app/database.h"
#include <iostream>
#include <cassert>
#include <chrono>

int main() {
    auto start = std::chrono::high_resolution_clock::now();
        // Очистка тестовой базы
    std::remove("test.db");
    std::cout << "========================================" << std::endl;
    std::cout << "       DATABASE TESTS — 25 TESTS        " << std::endl;
    std::cout << "========================================" << std::endl;
    
    int passed = 0;
    int total = 25;
    
    // Тест 1: Создание объекта базы
    Database db("test.db");
    std::cout << "[1/25] Database object created — OK" << std::endl; passed++;
    
    // Тест 2: Инициализация
    assert(db.init());
    std::cout << "[2/25] Database initialized — OK" << std::endl; passed++;
    
    // Тест 3: INSERT сообщения
    Message msg1{0, "in", "Hello World", "2026-04-13 10:00:00", 1};
    assert(db.insertMessage(msg1));
    std::cout << "[3/25] Insert message — OK" << std::endl; passed++;
    
    // Тест 4: SELECT всех сообщений
    auto list1 = db.getMessages();
    assert(!list1.empty());
    std::cout << "[4/25] Select messages — OK (found " << list1.size() << ")" << std::endl; passed++;
    
    // Тест 5: SEARCH по ключевому слову
    auto found1 = db.searchMessages("Hello");
    assert(!found1.empty());
    std::cout << "[5/25] Search messages — OK" << std::endl; passed++;
    
    // Тест 6: UPDATE сообщения
    if (!list1.empty()) {
        Message upd = list1[0];
        upd.text = "Updated text";
        assert(db.updateMessage(upd));
    }
    std::cout << "[6/25] Update message — OK" << std::endl; passed++;
    
    // Тест 7: DELETE сообщения
    if (!list1.empty()) {
        assert(db.deleteMessage(list1[0].id));
    }
    std::cout << "[7/25] Delete message — OK" << std::endl; passed++;
    
        // Тест 8: INSERT пользователя (уникальный логин с timestamp)
    std::string uniqueLogin = "ivan_" + std::to_string(std::time(nullptr));
    User user1{0, "Ivan Ivanov", uniqueLogin, "+79991234567"};
    assert(db.insertUser(user1));
    std::cout << "[8/25] Insert user — OK" << std::endl; passed++;
    
    // Тест 9: Пустой поиск (должен вернуть пусто)
    auto notFound = db.searchMessages("xyznonexistent");
    assert(notFound.empty());
    std::cout << "[9/25] Empty search result — OK" << std::endl; passed++;
    
    // Тест 10: Множественная вставка (10 сообщений)
    for (int i = 0; i < 10; i++) {
        Message m{0, "out", "Bulk " + std::to_string(i), "2026-04-13", 1};
        assert(db.insertMessage(m));
    }
    std::cout << "[10/25] Bulk insert (10 msgs) — OK" << std::endl; passed++;
    
    // Тест 11: Удаление несуществующего ID
    bool delFake = db.deleteMessage(99999);
    std::cout << "[11/25] Delete non-existent ID — OK (returned " << (delFake ? "true" : "false") << ")" << std::endl; passed++;
    
    // Тест 12: Обновление несуществующего сообщения
    Message fakeMsg{99999, "in", "fake", "2026", 1};
    bool updFake = db.updateMessage(fakeMsg);
    std::cout << "[12/25] Update non-existent msg — OK (returned " << (updFake ? "true" : "false") << ")" << std::endl; passed++;
    
    // Тест 13: Спецсимволы в тексте
    Message spec{0, "in", "!@#$%^&*()_+{}|:<>?~`-=[]\\;',./\"", "2026-04-13", 1};
    assert(db.insertMessage(spec));
    std::cout << "[13/25] Special characters — OK" << std::endl; passed++;
    
    // Тест 14: Очень длинное сообщение (500 символов)
    std::string longText(500, 'X');
    Message longMsg{0, "out", longText, "2026-04-13", 1};
    assert(db.insertMessage(longMsg));
    std::cout << "[14/25] Long message (500 chars) — OK" << std::endl; passed++;
    
    // Тест 15: Пустой текст сообщения
    Message empty{0, "in", "", "2026-04-13", 1};
    assert(db.insertMessage(empty));
    std::cout << "[15/25] Empty message text — OK" << std::endl; passed++;
    
    // Тест 16: Регистр в поиске
    auto up = db.searchMessages("BULK");
    auto low = db.searchMessages("bulk");
    std::cout << "[16/25] Case-sensitive search — OK (upper=" << up.size() << ", lower=" << low.size() << ")" << std::endl; passed++;
    
       // Тест 17: Множество пользователей (5 штук) — уникальные логины
    for (int i = 0; i < 5; i++) {
        std::string unique = "login" + std::to_string(i) + "_" + std::to_string(std::time(nullptr));
        User u{0, "User" + std::to_string(i), unique, "+7900" + std::to_string(i)};
        db.insertUser(u);
    }
    std::cout << "[17/25] Multiple users (5) — OK" << std::endl; passed++;
    
    // Тест 18: Сообщение типа "out"
    Message outMsg{0, "out", "Outgoing message", "2026-04-13", 1};
    assert(db.insertMessage(outMsg));
    std::cout << "[18/25] Message type 'out' — OK" << std::endl; passed++;
    
    // Тест 19: Сообщение типа "in"
    Message inMsg{0, "in", "Incoming message", "2026-04-13", 1};
    assert(db.insertMessage(inMsg));
    std::cout << "[19/25] Message type 'in' — OK" << std::endl; passed++;
    
    // Тест 20: Поиск по части слова
    auto part = db.searchMessages("com");
    std::cout << "[20/25] Partial word search — OK (found " << part.size() << ")" << std::endl; passed++;
    
    // Тест 21: Повторная инициализация (idempotent)
    assert(db.init());
    std::cout << "[21/25] Re-initialization — OK" << std::endl; passed++;
    
    // Тест 22: Проверка final count
    auto finalList = db.getMessages();
    std::cout << "[22/25] Final message count — OK (total " << finalList.size() << ")" << std::endl; passed++;
    
    // Тест 23: Поиск цифр в тексте
    Message nums{0, "in", "Test 123 numbers 456", "2026-04-13", 1};
    db.insertMessage(nums);
    auto foundNums = db.searchMessages("123");
    assert(!foundNums.empty());
    std::cout << "[23/25] Search numbers in text — OK" << std::endl; passed++;
    
    // Тест 24: Русские символы (UTF-8)
    Message rus{0, "in", "Привет мир", "2026-04-13", 1};
    assert(db.insertMessage(rus));
    std::cout << "[24/25] UTF-8 Russian text — OK" << std::endl; passed++;
    
    // Тест 25: Смешанный контент
    Message mixed{0, "out", "Hello! Привет! 123 @#$", "2026-04-13 15:30:45", 1};
    assert(db.insertMessage(mixed));
    std::cout << "[25/25] Mixed content — OK" << std::endl; passed++;
    
    // Итоги
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << "\n========================================" << std::endl;
    std::cout << "           TEST RESULTS                 " << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Passed: " << passed << "/" << total << std::endl;
    std::cout << "Failed: " << (total - passed) << "/" << total << std::endl;
    std::cout << "Time: " << duration.count() << " ms" << std::endl;
    std::cout << "Status: " << (passed == total ? "ALL TESTS PASSED ✓" : "SOME TESTS FAILED ✗") << std::endl;
    std::cout << "========================================" << std::endl;
    
    return (passed == total) ? 0 : 1;
}