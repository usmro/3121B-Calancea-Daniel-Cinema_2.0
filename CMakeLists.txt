cmake_minimum_required(VERSION 3.16)
project(CinemaSistem VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_QT_UI "Compileaza cu interfata Qt" OFF)

set(COMMON_SOURCES
    src/Film.cpp
    src/Sala.cpp
    src/Rezervare.cpp
    src/RezervareOnline.cpp
    src/Cinematograf.cpp
    ui_terminal/UITerminal.cpp
    ui_terminal/UIAdmin.cpp
    ui_terminal/UIAngajat.cpp
)

if(BUILD_QT_UI)
    find_package(Qt6 REQUIRED COMPONENTS Widgets PrintSupport)
    qt6_standard_project_setup()

    add_executable(cinema
        ${COMMON_SOURCES}
        src/main.cpp
        ui_qt/FilmCard.cpp
        ui_qt/SalaWidget.cpp
        ui_qt/RezervareDialog.cpp
        ui_qt/MainWindow.cpp
    )

    target_compile_definitions(cinema PRIVATE BUILD_QT_UI)
    target_include_directories(cinema PRIVATE include)
    target_link_libraries(cinema PRIVATE Qt6::Widgets Qt6::PrintSupport)
    set_target_properties(cinema PROPERTIES
        WIN32_EXECUTABLE TRUE
        MACOSX_BUNDLE TRUE
    )
else()
    add_executable(cinema
        ${COMMON_SOURCES}
        src/main.cpp
    )
    target_include_directories(cinema PRIVATE include)
endif()

add_executable(cinema_tests
    ${COMMON_SOURCES}
    tests/test_rezervari.cpp
)
target_include_directories(cinema_tests PRIVATE include)

enable_testing()
add_test(NAME TestRezervari COMMAND cinema_tests)
