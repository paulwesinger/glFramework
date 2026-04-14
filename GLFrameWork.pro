CONFIG -= qt

TEMPLATE = lib
DEFINES += GLFRAMEWORK_LIBRARY

CONFIG += c++17
VERSION = 1.0.0
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    errorlogs.cpp \
    fileutil.cpp \
    glframework.cpp \
    rendertext.cpp \
    shader.cpp \
    utils.cpp \
    vartypes.cpp

HEADERS += \
    GLFrameWork_global.h \
    errorlogs.h \
    fileutil.h \
    glframework.h \
    rendertext.h \
    shader.h \
    utils.h \
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

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../usr/lib64/release/ -lfreetype
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../usr/lib64/debug/ -lfreetype
else:unix: LIBS += -L$$PWD/../../../../usr/lib64/ -lfreetype

INCLUDEPATH += $$PWD/../../../../usr/include/freetype2
DEPENDPATH += $$PWD/../../../../usr/include/freetype2

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../usr/lib64/release/ -lSDL2_image
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../usr/lib64/debug/ -lSDL2_image
else:unix: LIBS += -L$$PWD/../../../../usr/lib64/ -lSDL2_image

INCLUDEPATH += $$PWD/../../../../usr/include/SDL2
DEPENDPATH += $$PWD/../../../../usr/include/SDL2

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../Logger/build/Desktop-Release/release/ -lLogger
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../Logger/build/Desktop-Release/debug/ -lLogger
else:unix: LIBS += -L$$PWD/../Logger/build/Desktop-Release/ -lLogger

INCLUDEPATH += $$PWD/../Logger
DEPENDPATH += $$PWD/../Logger/build

INCLUDEPATH += /usr/include/glm

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../usr/lib64/release/ -lGLEW
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../usr/lib64/debug/ -lGLEW
else:unix: LIBS += -L$$PWD/../../../../usr/lib64/ -lGLEW

INCLUDEPATH += $$PWD/../../../../usr/include/GL
DEPENDPATH += $$PWD/../../../../usr/include/GL
