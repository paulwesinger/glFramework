CONFIG -= qt

TEMPLATE = lib
DEFINES += GLFRAMEWORK_LIBRARY

CONFIG += c++17
VERSION = 1.0.0
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    glframework.cpp \
    vartypes.cpp

HEADERS += \
    GLFrameWork_global.h \
    glframework.h \
    vartypes.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../usr/lib64/release/ -lSDL2
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../usr/lib64/debug/ -lSDL2
else:unix:LIBS += -L$$PWD/../../../../usr/lib64/ -lSDL2
# else:unix: LIBS += -L/usr/lib64/ -lSDL2

INCLUDEPATH += /usr/include/SDL2
DEPENDPATH += /usr/include/SDL2

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../usr/lib64/release/ -lGL
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../usr/lib64/debug/ -lGL
else:unix: LIBS += -L$$PWD/../../../../usr/lib64/ -lGL

INCLUDEPATH += $$PWD/../../../../usr/include/GL
DEPENDPATH += $$PWD/../../../../usr/include/GL
