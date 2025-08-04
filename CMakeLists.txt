cmake_minimum_required(VERSION 3.10)
project(DailyDoseOf241 LANGUAGES CXX)

# Настройки компилятора
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Пути к зависимостям (замените на свои)
set(TGBOT_INCLUDE_DIR "C:/Code/tgbot-cpp/include")
set(TGBOT_LIBRARY "C:/Code/tgbot-cpp/build/libtgbot.a")
set(BOOST_ROOT "C:/Code/boost/")
set(SQLITE_INCLUDE_DIR "C:/Code/sqlite3/include")
set(SQLITECPP_INCLUDE_DIR "C:/Code/SQLiteCpp/include")
set(SQLITE_LIBRARY "C:/Code/sqlite3/lib/sqlite3.lib")
set(SQLITECPP_LIBRARY "C:/Code/SQLiteCpp/lib/SQLiteCpp.lib")

# Подключаем зависимости
find_package(Boost REQUIRED COMPONENTS)

include_directories(
    ${TGBOT_INCLUDE_DIR}
    ${Boost_INCLUDE_DIRS}
    ${SQLITE_INCLUDE_DIR}
    ${SQLITECPP_INCLUDE_DIR}
)

add_executable(${PROJECT_NAME}
    src/bot.cpp
    src/helpers.cpp
)

# Подключаем заголовки
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(DailyDoseOf241
    ${TGBOT_LIBRARY}
    ${Boost_LIBRARIES}
    ${SQLITECPP_LIBRARY}
    ${SQLITE_LIBRARY} 
    ssl crypto ws2_32 crypt32
)


#Инициализация БД
add_executable(init_db src/init_db.cpp)
target_link_libraries(init_db SQLiteCpp sqlite3)
