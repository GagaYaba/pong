QT = core widgets network

CONFIG += c++17 cmdline

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    client/GameClient/gameclient.cpp \
    main.cpp \
    server/GameServer/gameserver.cpp \
    src/ball.cpp \
    src/boundary.cpp \
    src/collision.cpp \
    src/control.cpp \
    src/game.cpp \
    src/inputKeybord.cpp \
    src/inputNetwork.cpp \
    src/globals.cpp \
    src/mainwindow.cpp \
    src/menuwindow.cpp \
    src/paddle.cpp \
    src/score.cpp \
    src/JoinDialog.cpp \
    src/SelectDialog.cpp \
    src/CodeDialog.cpp \
    src/utils.cpp \
    src/player.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

FORMS += \
    src/mainwindow.ui

HEADERS += \
    client/GameClient/ClientEventHandlerFactory.h \
    client/GameClient/ClientEventHandler.h \
    client/GameClient/gameclient.h \
    include/SelectDialog.h \
    include/ball.h \
    include/boundary.h \
    include/collision.h \
    include/control.h \
    include/game.h \
    include/globals.h \
    include/inputKeybord.h \
    include/inputNetwork.h \
    include/mainwindow.h \
    include/menuwindow.h \
    include/paddle.h \
    include/player.h \
    include/score.h \
    server/GameServer/gameserver.h \
    include/utils.h \
    include/CodeDialog.h \
    include/JoinDialog.h \
    config.h

FORMS += \
    src/mainwindow.ui \
    src/menuwindow.ui \
    src/CodeDialog.ui \
    src/SelectDialog.ui \
    src/JoinDialog.ui


MOC_DIR = build/moc
OBJECTS_DIR = build/obj
UI_DIR = build/ui
RCC_DIR = build/rcc

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

UI_HEADERS += \
    build/ui/ui_mainwindow.h \
    build/ui/ui_menuwindow.h \
    build/ui/ui_CodeDialog.h \
    build/ui/ui_SelectDialog.h \
    build/ui/ui_JoinDialog.h

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
    server/GameServer/gameserver.h \

DISTFILES += \
    .gitignore

