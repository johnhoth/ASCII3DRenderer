cmake_minimum_required(VERSION 3.10)
project(ASCII3DRenderer)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Поиск необходимых пакетов
find_package(Curses REQUIRED)
find_package(glm REQUIRED)

# Исходные файлы
set(SOURCES
    main.cpp
    3d_objects.cpp
    projection.cpp
    ascii_renderer.cpp
    menu.cpp
)

set(HEADERS
    3d_objects.hpp
    projection.hpp
    ascii_renderer.hpp
    menu.hpp
)

# Создание исполняемого файла
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Подключение библиотек и включение директорий
target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
    ${CURSES_LIBRARIES}
    glm::glm
)

target_include_directories(${PROJECT_NAME} 
    PRIVATE 
    ${CURSES_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Добавляем исходные файлы к существующему исполняемому файлу
target_sources(${PROJECT_NAME} 
    PRIVATE 
    ${SOURCES}
    ${HEADERS}
)
