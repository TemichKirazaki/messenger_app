#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QListWidget>

#include "database.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    Database db("messages.db");
    db.init();

    QWidget window;
    window.setWindowTitle("Messenger");

    QVBoxLayout *layout = new QVBoxLayout();

    QLineEdit *input = new QLineEdit();
    input->setPlaceholderText("Введите сообщение");

    QLineEdit *searchInput = new QLineEdit();
    searchInput->setPlaceholderText("Поиск...");

    QPushButton *addBtn = new QPushButton("Добавить");
    QPushButton *showBtn = new QPushButton("Показать все");
    QPushButton *searchBtn = new QPushButton("Поиск");
    QPushButton *deleteBtn = new QPushButton("Удалить по ID");

    QListWidget *list = new QListWidget();

    layout->addWidget(input);
    layout->addWidget(addBtn);
    layout->addWidget(showBtn);
    layout->addWidget(searchInput);
    layout->addWidget(searchBtn);
    layout->addWidget(deleteBtn);
    layout->addWidget(list);

    window.setLayout(layout);

    // ➕ Добавить
    QObject::connect(addBtn, &QPushButton::clicked, [&]() {
        Message msg{0, "in", input->text().toStdString(), "2026", 1};
        db.insertMessage(msg);
        input->clear();
    });

    // 📋 Показать
    QObject::connect(showBtn, &QPushButton::clicked, [&]() {
        list->clear();
        auto messages = db.getMessages();
        for (auto &m : messages) {
            list->addItem(QString::number(m.id) + ": " + QString::fromStdString(m.text));
        }
    });

    // 🔍 Поиск
    QObject::connect(searchBtn, &QPushButton::clicked, [&]() {
        list->clear();
        auto messages = db.searchMessages(searchInput->text().toStdString());
        for (auto &m : messages) {
            list->addItem(QString::number(m.id) + ": " + QString::fromStdString(m.text));
        }
    });

    // ❌ Удаление
    QObject::connect(deleteBtn, &QPushButton::clicked, [&]() {
        bool ok;
        int id = searchInput->text().toInt(&ok);
        if (ok) {
            db.deleteMessage(id);
        }
    });

    window.show();

    return app.exec();
}