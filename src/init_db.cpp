#include <SQLiteCpp/SQLiteCpp.h>
#include <iostream>

void initializeDatabase() {
    try {
        // Открываем/создаём базу данных
        SQLite::Database db("daily_dose.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        
        // SQL-запрос для создания таблиц
        const char* createTablesSQL = R"(
            CREATE TABLE IF NOT EXISTS daily_content (
                date TEXT PRIMARY KEY,
                quote_id INTEGER REFERENCES quotes(id),
                task_id INTEGER REFERENCES tasks(id),
                user_id INTEGER REFERENCES users(id),
                photo_id INTEGER REFERENCES photos(id)
            );

            CREATE TABLE IF NOT EXISTS users (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                username TEXT UNIQUE NOT NULL,
                tasks_completed INTEGER DEFAULT 0
            );

            CREATE TABLE IF NOT EXISTS photos (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                file_path TEXT NOT NULL
            );

            CREATE TABLE IF NOT EXISTS quotes (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                text TEXT NOT NULL,
                author TEXT NOT NULL
            );

            CREATE TABLE IF NOT EXISTS tasks (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                description TEXT NOT NULL
            );
        )";

        // Выполняем SQL
        db.exec(createTablesSQL);
        
        std::cout << "Database initialized successfully!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "SQLite error: " << e.what() << std::endl;
    }
}

int main() {
    initializeDatabase();
    return 0;
}