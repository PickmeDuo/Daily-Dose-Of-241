#ifndef DAILYDOSEOF241_HPP
#define DAILYDOSEOF241_HPP

#include "tasks.hpp"

#include <tgbot/tgbot.h>
#include <nlohmann/json.hpp>

#include <string>
#include <vector>
#include <map>
#include <memory>

namespace DailyDoseOf241 {
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
        void addPerson(const std::string& username, const int64_t chat_id);

        // Вызывается по команде /task
        // Возвращает случайное задание для случайного пользователя.
        std::string taskOfTheDay();

        // Вызывается по команде /quote
        // Возвращает случайную цитату из добавленных.
        std::string quoteOfTheDay();

        // Вызывается по команде /addQuote (ответом на сообщение)
        // Добавляет цитату (сообщение, на которое ответили командой) в базу.
        void addQuote(const std::string& quote);

        // Вызывается по команде /pic
        // Возвращает путь к случайной картинке.
        std::string picOfTheDay();


        // Можешь сделать это кнопками, или вообще как хочешь. Звони крч
    };

} 

#endif