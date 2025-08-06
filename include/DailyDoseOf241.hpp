#ifndef DAILYDOSEOF241_HPP
#define DAILYDOSEOF241_HPP

#include <tgbot/tgbot.h>
#include <SQLiteCpp/SQLiteCpp.h>

#include <string>
#include <vector>
#include <map>
#include <memory>

namespace DailyDoseOf241 {
    // Возвращает текущую дату
    std::string getCurrentDate();

    // Создаёт ежедневный контент, если он ещё не создан.
    bool getDailyContent();

    // Возвращает случайный id объекта table из базы db.
    int getRandomId(SQLite::Database& db, const std::string& table);



    class DDO241Bot {
    public:
        explicit DDO241Bot(const std::string& token);

        // Копирование запрещено
        DDO241Bot(const DDO241Bot&) = delete;
        DDO241Bot& operator=(const DDO241Bot&) = delete;

        ~DDO241Bot() = default;

        // Запуск бота и всех обработчиков
        void start();

    private:
        std::unique_ptr<TgBot::Bot> bot;

        // Вызывается на каждое сообщение (onAnyMessage)
        // Проверяет, есть ли человек с ником username в базе по чату chat_id, если нет, добавляет.
        bool addUser(const std::string& username);

        // Вызывается по команде /addQuote (ответом на сообщение)
        // Добавляет цитату (сообщение, на которое ответили командой) в базу.
        bool addQuote(const std::string& quote, const std::string& author);

        // Вызывается по команде /task
        // Возвращает случайное задание для случайного пользователя.
        std::string taskOfTheDay();

        // Вызывается по команде /quote
        // Возвращает случайную цитату из добавленных.
        std::string quoteOfTheDay();

        // Вызывается по команде /pic
        // Возвращает путь к случайной картинке.
        std::string picOfTheDay();


        // Вызывается по команде /done только Сашей или Сеней.
        // Отмечает задание выполненным.
        bool taskDone();

        // Вызывается по команде /rating
        // Возвращает рейтинг в виде строки.
        std::string rating();

        // Можешь сделать это кнопками, или вообще как хочешь. Звони крч
    };
} 

#endif