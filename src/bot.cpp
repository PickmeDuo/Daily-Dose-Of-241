#include "DailyDoseOf241.hpp"
#include <iostream>

using namespace DailyDoseOf241;


DDO241Bot::DDO241Bot(const std::string& token) {
    bot = std::make_unique<TgBot::Bot>(token);
}


void DDO241Bot::start() {
    auto& b = *bot;


    // /start
    b.getEvents().onCommand("start", [this](TgBot::Message::Ptr message) {
        std::string username = message->from->username;
        if (addUser(username)) {
            bot->getApi().sendMessage(message->chat->id, "Добро пожаловать, @" + username + "!");
        } else {
            bot->getApi().sendMessage(message->chat->id, "Ошибка при добавлении пользователя.");
        }
    });


    // /addQuote 
    b.getEvents().onCommand("addQuote", [this](TgBot::Message::Ptr message) {
        if (!message->replyToMessage) {
            bot->getApi().sendMessage(message->chat->id, "Команду нужно использовать в ответ на сообщение.");
            return;
        }
        std::string quote = message->replyToMessage->text;
        std::string author = message->replyToMessage->from->username;
        if (addQuote(quote, author)) {
            bot->getApi().sendMessage(message->chat->id, "Цитата добавлена!");
        } else {
            bot->getApi().sendMessage(message->chat->id, "Ошибка при добавлении цитаты.");
        }
    });


    // /task
    b.getEvents().onCommand("task", [this](TgBot::Message::Ptr message) {
        std::string response = taskOfTheDay();
        bot->getApi().sendMessage(message->chat->id, response);
    });


    // /quote
    b.getEvents().onCommand("quote", [this](TgBot::Message::Ptr message) {
        std::string response = quoteOfTheDay();
        bot->getApi().sendMessage(message->chat->id, response);
    });


    // /pic
    b.getEvents().onCommand("pic", [this](TgBot::Message::Ptr message) {
        std::string path = picOfTheDay();
        if (path.find("Ошибка") == 0) {
            bot->getApi().sendMessage(message->chat->id, path);
            return;
        }

        try {
            TgBot::InputFile::Ptr photo = TgBot::InputFile::fromFile(path, "image/jpg");
            bot->getApi().sendPhoto(message->chat->id, photo);
        }
        catch (const std::exception& e) {
            bot->getApi().sendMessage(message->chat->id, "Ошибка при отправке картинки.");
            std::cerr << "Error sending photo: " << e.what() << std::endl;
        }
    });


    // /help
    b.getEvents().onCommand("help", [this](TgBot::Message::Ptr message) {
        std::string helpText = R"(Список команд:
        /start — регистрация пользователя
        /addQuote — добавить цитату (в ответ на сообщение)
        /quote — получить цитату дня
        /task — получить задание дня
        /pic — получить картинку дня)";

        bot->getApi().sendMessage(message->chat->id, helpText);
    });


    // /done
    b.getEvents().onCommand("done", [this](TgBot::Message::Ptr message) {
        std::string username = message->from->username;
        if (username == "eduardade" || username == "senyastrokov") {
            if (taskDone()) {
                bot->getApi().sendMessage(message->chat->id, "Детка, ты выполнила задание на 5+!");
            } else {
                bot->getApi().sendMessage(message->chat->id, "Ошибка при отметке задания.");
            }
        } else {
            bot->getApi().sendMessage(message->chat->id, "Ты не папочка Окси!");
        }
    });

    // Обработка любого сообщения 
    b.getEvents().onAnyMessage([this](TgBot::Message::Ptr message) {
        if (message->text.empty()) return;
        std::string username = message->from->username;
        addUser(username); 
    });

    try {
        std::cout << "Бот запущен. Ожидание сообщений..." << std::endl;
        TgBot::TgLongPoll longPoll(b);
        while (true) {
            longPoll.start();
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка в работе бота: " << e.what() << std::endl;
    }
}


int main() {
    try {
        std::string token = "8058398407:AAFt2kqAb_UqUJOxN2sSCMJtpyzDBFwbMQY";
        DailyDoseOf241::DDO241Bot bot(token);
        bot.start();
    } catch (const std::exception& e) {
        std::cerr << "Ошибка при запуске бота: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}