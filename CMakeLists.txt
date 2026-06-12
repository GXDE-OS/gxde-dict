cmake_minimum_required(VERSION 3.16)
project(gxde-dict VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network Multimedia DBus Concurrent)
find_package(Dtk6 REQUIRED Core Gui)
find_package(Dtk2widget REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(X11 REQUIRED x11 xext xtst)

add_executable(gxde-dict
    main.cpp
    mainwindow.cpp
    homepage.cpp
    dictpage.cpp
    dailypage.cpp
    loadpage.cpp
    transpage.cpp
    trayicon.cpp
    eventmonitor.cpp
    utils.cpp
    youdaoapi.cpp
    popup/popupcontent.cpp
    popup/popupwindow.cpp
    widgets/listdelegate.cpp
    widgets/queryedit.cpp
    widgets/scrollarea.cpp
    widgets/textedit.cpp
    widgets/toolbar.cpp
    gxde-dict.qrc
)

target_include_directories(gxde-dict PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets
    ${CMAKE_CURRENT_SOURCE_DIR}/popup
    ${X11_INCLUDE_DIRS}
)

target_link_libraries(gxde-dict PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
    Qt6::Multimedia
    Qt6::DBus
    Qt6::Concurrent
    Dtk6::Core
    Dtk6::Gui
    ${Dtk2widget_LIBRARIES}
    ${X11_LIBRARIES}
)

target_compile_definitions(gxde-dict PRIVATE
    ${X11_CFLAGS_OTHER}
)

# Install rules
include(GNUInstallDirs)

install(TARGETS gxde-dict DESTINATION ${CMAKE_INSTALL_BINDIR})

install(FILES images/gxde-dict.svg
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/scalable/apps
)

install(FILES gxde-dict.desktop
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/applications
)
