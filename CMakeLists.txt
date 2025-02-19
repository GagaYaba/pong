cmake_minimum_required(VERSION 3.19)
project(sans_titre LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(sans_titre
    WIN32 MACOSX_BUNDLE
    main.cpp
        src/mainwindow.cpp
        include/mainwindow.h
        ui/mainwindow.ui
        include/paddle.h src/paddle.cpp
        include/ball.h src/ball.cpp
        include/score.h src/score.cpp
        include/game.h src/game.cpp

)

target_link_libraries(sans_titre
    PRIVATE
        Qt::Core
        Qt::Widgets
)

include(GNUInstallDirs)

install(TARGETS sans_titre
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET sans_titre
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
