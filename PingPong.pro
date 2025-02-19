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
    src/game.cpp \
    src/mainwindow.cpp \
    src/paddle.cpp \
    src/score.cpp


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

FORMS += \
    src/mainwindow.ui

HEADERS += \
    client/GameClient/gameclient.h \
    include/ball.h \
    include/boundary.h \
    include/collision.h \
    include/game.h \
    include/mainwindow.h \
    include/paddle.h \
    include/score.h \
    server/GameServer/gameserver.h \
    src/collision.h

DISTFILES += \
    .gitignore

