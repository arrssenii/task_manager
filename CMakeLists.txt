cmake_minimum_required(VERSION 3.10)
project(TaskManager)

set(CMAKE_CXX_STANDARD 17)

# Указываем путь к заголовочным файлам
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories("C:/sqlite")  # Путь к заголовочному файлу sqlite3.h

# Указываем исходники
file(GLOB SOURCES "src/*.cpp")

# Добавляем исполнимый файл
add_executable(task_manager ${SOURCES})

# Линкуем с библиотекой SQLite
link_directories("C:/sqlite")  # Путь к библиотекам sqlite3.dll или sqlite3.lib
target_link_libraries(task_manager sqlite3)
