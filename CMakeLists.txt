cmake_minimum_required(VERSION 3.19)
project(sans_titre LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Network)

qt_standard_project_setup()

qt_add_executable(sans_titre
    WIN32 MACOSX_BUNDLE
    main.cpp
        src/mainwindow.cpp include/mainwindow.h src/mainwindow.ui
        include/paddle.h src/paddle.cpp
        include/ball.h src/ball.cpp
        include/score.h src/score.cpp
        include/game.h src/game.cpp
        include/JoinDialog.h src/JoinDialog.cpp src/JoinDialog.ui
        include/CodeDialog.h src/CodeDialog.cpp src/CodeDialog.ui
        include/SelectDialog.h src/SelectDialog.cpp src/SelectDialog.ui
        include/utils.h src/utils.cpp
        include/menuwindow.h src/menuwindow.cpp src/menuwindow.ui
        client/GameClient/gameclient.h client/GameClient/gameclient.cpp
        server/GameServer/gameserver.h server/GameServer/gameserver.cpp
        client/GameClient/ClientEventHandler.h client/GameClient/ClientEventHandlerFactory.h
        config.h
)


target_link_libraries(sans_titre
    PRIVATE
        Qt6::Core
        Qt6::Widgets
        Qt6::Network
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
