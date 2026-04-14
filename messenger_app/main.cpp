#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QListWidget>

#include "database.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    Database db("messages.db");
    db.init();

    QWidget window;
    window.setWindowTitle("Messenger");

    QVBoxLayout* layout = new QVBoxLayout();

    // === СООБЩЕНИЯ ===
    QLineEdit* input = new QLineEdit();
    input->setPlaceholderText("Введите сообщение");

    QLineEdit* userIdForMsg = new QLineEdit();
    userIdForMsg->setPlaceholderText("ID пользователя (для сообщения)");
    userIdForMsg->setText("1");

    QPushButton* addBtn = new QPushButton("Добавить сообщение");

    // === ПОЛЬЗОВАТЕЛИ ===
    QLineEdit* userNameInput = new QLineEdit();
    userNameInput->setPlaceholderText("ФИО пользователя");

    QLineEdit* userLoginInput = new QLineEdit();
    userLoginInput->setPlaceholderText("Логин");

    QLineEdit* userPhoneInput = new QLineEdit();
    userPhoneInput->setPlaceholderText("Телефон");

    QPushButton* addUserBtn = new QPushButton("Добавить пользователя");

    // === ПОИСК И УДАЛЕНИЕ ===
    QLineEdit* searchInput = new QLineEdit();
    searchInput->setPlaceholderText("Поиск...");

    QPushButton* showBtn = new QPushButton("Показать все");
    QPushButton* searchBtn = new QPushButton("Поиск");
    QPushButton* deleteBtn = new QPushButton("Удалить по ID");

    QLineEdit* userIdInput = new QLineEdit();
    userIdInput->setPlaceholderText("ID пользователя для удаления");

    QPushButton* deleteByUserBtn = new QPushButton("Удалить все сообщения пользователя");

    // === СПИСОК ===
    QListWidget* list = new QListWidget();

    // === ДОБАВЛЯЕМ В LAYOUT ===
    // Сообщения
    layout->addWidget(input);
    layout->addWidget(userIdForMsg);
    layout->addWidget(addBtn);
    
    // Пользователи
    layout->addWidget(userNameInput);
    layout->addWidget(userLoginInput);
    layout->addWidget(userPhoneInput);
    layout->addWidget(addUserBtn);
    
    // Поиск и удаление
    layout->addWidget(showBtn);
    layout->addWidget(searchInput);
    layout->addWidget(searchBtn);
    layout->addWidget(deleteBtn);
    layout->addWidget(userIdInput);
    layout->addWidget(deleteByUserBtn);
    
    // Список
    layout->addWidget(list);

    window.setLayout(layout);

    // === СИГНАЛЫ ===

    // Добавить сообщение
    QObject::connect(addBtn, &QPushButton::clicked, [&]() {
        bool ok;
        int userId = userIdForMsg->text().toInt(&ok);
        if (!ok || userId <= 0) userId = 1;
        
        Message msg{0, "in", input->text().toStdString(), "2026", userId};
        db.insertMessage(msg);
        input->clear();
    });

    // Добавить пользователя
    QObject::connect(addUserBtn, &QPushButton::clicked, [&]() {
        User u{0, 
            userNameInput->text().toStdString(),
            userLoginInput->text().toStdString(), 
            userPhoneInput->text().toStdString()
        };
        if (db.insertUser(u)) {
            userNameInput->clear();
            userLoginInput->clear();
            userPhoneInput->clear();
        }
    });

    // Показать все
    QObject::connect(showBtn, &QPushButton::clicked, [&]() {
        list->clear();
        auto messages = db.getMessages();
        for (auto& m : messages) {
            list->addItem(QString::number(m.id) + ": [User " + QString::number(m.user_id) + "] " + QString::fromStdString(m.text));
        }
    });

    // Поиск
    QObject::connect(searchBtn, &QPushButton::clicked, [&]() {
        list->clear();
        auto messages = db.searchMessages(searchInput->text().toStdString());
        for (auto& m : messages) {
            list->addItem(QString::number(m.id) + ": " + QString::fromStdString(m.text));
        }
    });

    // Удалить по ID
    QObject::connect(deleteBtn, &QPushButton::clicked, [&]() {
        bool ok;
        int id = searchInput->text().toInt(&ok);
        if (ok) db.deleteMessage(id);
    });

    // Удалить по пользователю
    QObject::connect(deleteByUserBtn, &QPushButton::clicked, [&]() {
        bool ok;
        int userId = userIdInput->text().toInt(&ok);
        if (ok) {
            db.deleteMessagesByUser(userId);
            list->clear();
            auto messages = db.getMessages();
            for (auto& m : messages) {
                list->addItem(QString::number(m.id) + ": " + QString::fromStdString(m.text));
            }
            userIdInput->clear();
        }
    });

    window.show();
    return app.exec();
}