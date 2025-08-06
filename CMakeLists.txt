cmake_minimum_required(VERSION 3.10)
project(UtrobisheBot LANGUAGES CXX)

# Настройки компилятора
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Пути к зависимостям (замените на свои)
set(TGBOT_INCLUDE_DIR "C:/Code/libs/tgbot-cpp/include")
set(TGBOT_LIBRARY "C:/Code/libs/tgbot-cpp/build/libtgbot.a")
set(BOOST_ROOT "C:/Code/libs/boost/")
set(SQLiteCpp_DIR "C:/Code/libs/SQLiteCpp-install/lib/cmake/SQLiteCpp")

# Подключаем зависимости
find_package(Boost REQUIRED COMPONENTS)

# Для SQLite - используем find_package
find_package(SQLite3 REQUIRED)

#find_package(SQLiteCpp REQUIRED)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/SQLiteCpp)



add_executable(${PROJECT_NAME}
    src/bot.cpp
    src/helpers.cpp
)

# Подключаем заголовки
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${TGBOT_INCLUDE_DIR}
    ${Boost_INCLUDE_DIRS}
    ${SQLite3_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/SQLiteCpp/include
)

# Для основного проекта
target_link_libraries(UtrobisheBot
    ${TGBOT_LIBRARY}
    ${Boost_LIBRARIES}
    SQLiteCpp
    SQLite::SQLite3 
    ssl crypto ws2_32 crypt32
)


#Инициализация БД
add_executable(init_db src/init_db.cpp)
target_link_libraries(init_db
    SQLiteCpp
    SQLite::SQLite3
)