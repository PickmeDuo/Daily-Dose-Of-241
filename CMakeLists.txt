cmake_minimum_required(VERSION 3.10)
project(PsychotypeBot LANGUAGES CXX)

# Настройки компилятора
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Пути к TgBot и Boost (замените на свои)
set(TGBOT_INCLUDE_DIR "C:/Code/tgbot-cpp/include")
set(TGBOT_LIBRARY "C:/Code/tgbot-cpp/build/libtgbot.a")
set(BOOST_ROOT "C:/Code/boost/")

# Подключаем зависимости
find_package(Boost REQUIRED COMPONENTS)

include_directories(
    ${TGBOT_INCLUDE_DIR}
    ${Boost_INCLUDE_DIRS}
)


add_executable(${PROJECT_NAME}
    src/bot.cpp
    src/helpers.cpp
)

# Подключаем заголовки
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(PsychotypeBot
    ${TGBOT_LIBRARY}
    ${Boost_LIBRARIES}
    ssl crypto ws2_32 crypt32
)
# endif()