QT = core widgets network

CONFIG += c++17 cmdline

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ball.cpp \
    client/GameClient/gameclient.cpp \
    game.cpp \
    main.cpp \
    mainwindow.cpp \
    paddle.cpp \
    score.cpp \
    server/GameServer/gameserver.cpp


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

FORMS += \
    mainwindow.ui

HEADERS += \
    ball.h \
    client/GameClient/gameclient.h \
    game.h \
    mainwindow.h \
    paddle.h \
    score.h \
    server/GameServer/gameserver.h

DISTFILES += \
    .gitignore

