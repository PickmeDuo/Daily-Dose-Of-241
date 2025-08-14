#include "DailyDoseOf241.hpp"





namespace DailyDoseOf241 {
    std::string getCurrentDate() {
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);
        
        std::stringstream ss;
        ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d");
        return ss.str();
    }

    bool getDailyContent() {
        std::string today = getCurrentDate();
        
        SQLite::Database db("daily_dose.db", SQLite::OPEN_READWRITE);

        // Пробуем получить контент за сегодня
        // Смотрим есть ли контент в базе, если есть и дата прошлая, удаляем и делаем новый.
        SQLite::Statement query(db,
            "SELECT date, quote_id, task_id, user_id, photo_id "
            " FROM daily_content");
        
        if (query.executeStep()) {
            std::string query_date = query.getColumn(0).getString();
            if (query_date == today) {
                // Контент на сегодня уже есть
                return true;
            } else {
                int tid = query.getColumn(2).getInt();
                int pid = query.getColumn(4).getInt();

                SQLite::Statement query1(db, "DELETE FROM tasks WHERE id = ?");
                query1.bind(1, tid);
                query1.exec();

                SQLite::Statement query2(db, "DELETE FROM photos WHERE id = ?");
                query2.bind(1, pid);
                query2.exec();

                db.exec("DELETE FROM daily_content");  // Очищаем таблицу daily_content
                db.exec("UPDATE sqlite_sequence SET seq = 0 WHERE name = 'daily_content'");
            }
        }

        
        // Если контента нет - создаём новый
        SQLite::Transaction transaction(db);
        
        try {
            // Получаем случайные ID для каждого типа контента
            int quote_id = getRandomId(db, "quotes");
            int task_id = getRandomId(db, "tasks");
            int user_id = getRandomId(db, "users");
            int photo_id = getRandomId(db, "photos");

            
            // Сохраняем в daily_content
            SQLite::Statement insert(db,
                "INSERT INTO daily_content (date, quote_id, task_id, user_id, photo_id) "
                "VALUES (?, ?, ?, ?, ?)");
            insert.bind(1, today);
            insert.bind(2, quote_id);
            insert.bind(3, task_id);
            insert.bind(4, user_id);
            insert.bind(5, photo_id);
            insert.exec();

            
            transaction.commit();
            return true;
        }
        catch (const std::exception& e) {
            transaction.rollback();
            return false;
        }
    }


    int getRandomId(SQLite::Database& db, const std::string& table) {
        SQLite::Statement query(db,
            "SELECT id FROM " + table + 
            " ORDER BY RANDOM() LIMIT 1");
        if (!query.executeStep()) {
            throw std::runtime_error("No available content in " + table);
        }
        return query.getColumn(0).getInt();
    }

    void delete_from(SQLite::Database& db, const std::string& table, const int& id) {

    }




    bool DDO241Bot::addUser(const std::string& username) {
        try {
            SQLite::Database db("daily_dose.db", SQLite::OPEN_READWRITE);
            SQLite::Statement insert(db, "INSERT OR IGNORE INTO users (username) VALUES (?)");
            insert.bind(1, username);
            insert.exec();
            return true;
        }
        catch (const std::exception& e) {
            std::cerr << "Error adding user: " << e.what() << "\n";
            return false;
        }
    }

    bool DDO241Bot::addQuote(const std::string& quote, const std::string& author) {
        try {
            SQLite::Database db("daily_dose.db", SQLite::OPEN_READWRITE);
            SQLite::Statement insert(db, "INSERT INTO quotes (text, author) VALUES (?, ?)");
            insert.bind(1, quote);
            insert.bind(2, author);
            insert.exec();
            return true;
        }
        catch (const std::exception& e) {
            std::cerr << "Error adding quote: " << e.what() << "\n";
            return false;
        }

    }



    std::string DDO241Bot::taskOfTheDay() {
        if (!getDailyContent()) {
            return "Ошибка при создании контента дня.";
        }
        std::string today = getCurrentDate();

        std::string username = "";

        try {
            SQLite::Database db("daily_dose.db", SQLite::OPEN_READONLY);
            // Берём id сегодняшнего пользователя
            SQLite::Statement id_query(db, 
                "SELECT user_id FROM daily_content"
                " WHERE date = ?");
            id_query.bind(1, today);
            id_query.executeStep();
            int id = id_query.getColumn(0).getInt();

            // Находим этого пользователя
            SQLite::Statement query(db,
                "SELECT username FROM users "
                "WHERE id = ?");
            query.bind(1, id);
            query.executeStep();
            username = query.getColumn(0).getString();
        }
        catch (const std::exception& e) {
            std::cerr << "Error selecting user: " << e.what() << std::endl;
            return "Ошибка при выборе задания дня.";
        }


        std::string task = "";

        try {
            SQLite::Database db("daily_dose.db", SQLite::OPEN_READONLY);
            // Берём id сегодняшнего задания
            SQLite::Statement id_query(db, 
                "SELECT task_id FROM daily_content"
                " WHERE date = ?");
            id_query.bind(1, today);
            id_query.executeStep();
            int id = id_query.getColumn(0).getInt();

            // Находим это задание
            SQLite::Statement query(db,
                "SELECT description FROM tasks "
                "WHERE id = ?");
            query.bind(1, id);
            query.executeStep();
            task = query.getColumn(0).getString();
        }
        catch (const std::exception& e) {
            std::cerr << "Error selecting task: " << e.what() << std::endl;
            return "Ошибка при выборе задания дня.";
        }

        return "@" + username + " Задание для вас:\n" + task;
    }



    std::string DDO241Bot::quoteOfTheDay() {
        if (!getDailyContent()) {
            return "Ошибка при создании контента дня.";
        }
        std::string today = getCurrentDate();

        try {
            SQLite::Database db("daily_dose.db", SQLite::OPEN_READONLY);
            // Берём id сегодняшней цитаты
            SQLite::Statement id_query(db, 
                "SELECT quote_id FROM daily_content"
                " WHERE date = ?");
            id_query.bind(1, today);
            id_query.executeStep();
            int id = id_query.getColumn(0).getInt();

            // Находим этой цитаты
            SQLite::Statement query(db,
                "SELECT text, author FROM quotes "
                "WHERE id = ?");
            query.bind(1, id);
            query.executeStep();
            return "'" + query.getColumn(0).getString() + "' - " + query.getColumn(1).getString();
        }
        catch (const std::exception& e) {
            std::cerr << "Error selecting quote: " << e.what() << std::endl;
            return "Ошибка при выборе цитаты дня.";
        }
    }



    std::string DDO241Bot::picOfTheDay() {
        if (!getDailyContent()) {
            return "Ошибка при создании контента дня.";
        }
        std::string today = getCurrentDate();
        try {
            SQLite::Database db("daily_dose.db", SQLite::OPEN_READONLY);
            // Берём id сегодняшней картинки
            SQLite::Statement id_query(db, 
                "SELECT photo_id FROM daily_content"
                " WHERE date = ?");
            id_query.bind(1, today);
            id_query.executeStep();
            int id = id_query.getColumn(0).getInt();

            // Находим эту картинку
            SQLite::Statement query(db,
                "SELECT file_path FROM photos "
                "WHERE id = ?");
            query.bind(1, id);
            query.executeStep();
            return query.getColumn(0).getString();
        }
        catch (const std::exception& e) {
            std::cerr << "Error selecting quote: " << e.what() << std::endl;
            return "Ошибка при выборе картинки дня.";
        }
    }

    bool DDO241Bot::taskDone() {
        if (!getDailyContent()) {
            return "Ошибка при создании контента дня.";
        }
        std::string today = getCurrentDate();

        SQLite::Database db("daily_dose.db", SQLite::OPEN_READWRITE);

        std::string username = "";
        try {
            // Берём id сегодняшнего пользователя
            SQLite::Statement id_query(db, 
                "SELECT user_id FROM daily_content"
                " WHERE date = ?");
            id_query.bind(1, today);
            id_query.executeStep();
            int id = id_query.getColumn(0).getInt();

            // Находим этого пользователя
            SQLite::Statement query(db,
                "SELECT username FROM users "
                "WHERE id = ?");
            query.bind(1, id);
            query.executeStep();
            username = query.getColumn(0).getString();
        }
        catch (const std::exception& e) {
            std::cerr << "Error getting username: " << e.what() << std::endl;
            return false;
        }
        
        try {
            SQLite::Statement updateUser(db,
                "UPDATE users "
                "SET tasks_completed = tasks_completed + 1 "
                "WHERE username = ?");
            updateUser.bind(1, username);
            updateUser.exec();
        }
        catch (const std::exception& e) {
            std::cerr << "Error updating rating: " << e.what() << std::endl;
            return false;
        }
        return true;
    }

    bool DDO241Bot::taskDoneSpecificUser(const std::string& username) {
        SQLite::Database db("daily_dose.db", SQLite::OPEN_READWRITE);
        try {
            SQLite::Statement updateUser(db,
                "UPDATE users "
                "SET tasks_completed = tasks_completed + 1 "
                "WHERE username = ?");
            updateUser.bind(1, username);
            updateUser.exec();
        }
        catch (const std::exception& e) {
            std::cerr << "Error updating rating: " << e.what() << std::endl;
            return false;
        }
        return true;
    }

    std::string DDO241Bot::rating() {
        try {
            SQLite::Database db("daily_dose.db", SQLite::OPEN_READONLY);
            SQLite::Statement query(db,
                "SELECT username, tasks_completed "
                "FROM users "
                "ORDER BY tasks_completed DESC, username ASC");
            std::string result = "Рейтинг по выполненным заданиям:\n";
            while (query.executeStep()) {
                std::string username = query.getColumn(0).getString();
                int tasks = query.getColumn(1).getInt();
                result += username + ": " + std::to_string(tasks) + "\n";
            }
            return result;
        }
        catch (const std::exception& e) {
            std::cerr << "Error getting rating: " << e.what() << std::endl;
            return "Ошибка при получении рейтинга.";
        }
    }
}