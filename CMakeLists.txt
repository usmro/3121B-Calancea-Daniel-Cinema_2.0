cmake_minimum_required(VERSION 3.16)
project(CinemaSistem VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_QT_UI "Compileaza cu UI grafic Qt6 (necesita Qt6)" OFF)

include_directories(include)

# ── Surse backend (comune ambelor target-uri) ──────────────────────────────
set(BACKEND_SOURCES
    src/Film.cpp
    src/Sala.cpp
    src/Rezervare.cpp
    src/RezervareOnline.cpp
    src/Cinematograf.cpp
)

# ── Surse UI terminal ───────────────────────────────────────────────────────
set(TERMINAL_SOURCES
    ui_terminal/UITerminal.cpp
    ui_terminal/UIAdmin.cpp
    ui_terminal/UIAngajat.cpp
)

# ── Executabil principal ────────────────────────────────────────────────────
if(BUILD_QT_UI)
    find_package(Qt6 REQUIRED COMPONENTS Widgets PrintSupport)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_AUTORCC ON)

    set(QT_SOURCES
        ui_qt/MainWindow.cpp
        ui_qt/FilmCard.cpp
        ui_qt/SalaWidget.cpp
        ui_qt/RezervareDialog.cpp
    )

    add_executable(cinema
        src/main.cpp
        ${BACKEND_SOURCES}
        ${TERMINAL_SOURCES}
        ${QT_SOURCES}
    )
    target_link_libraries(cinema PRIVATE Qt6::Widgets Qt6::PrintSupport)
    target_compile_definitions(cinema PRIVATE BUILD_QT_UI)
else()
    add_executable(cinema
        src/main.cpp
        ${BACKEND_SOURCES}
        ${TERMINAL_SOURCES}
    )
endif()

# ── Teste unitare ───────────────────────────────────────────────────────────
add_executable(cinema_tests
    tests/test_rezervari.cpp
    ${BACKEND_SOURCES}
)

enable_testing()
add_test(NAME UnitTests COMMAND cinema_tests)
