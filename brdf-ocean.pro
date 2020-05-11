QT += core gui opengl

TARGET = brdf-ocean 
TEMPLATE = app

CONFIG += debug

QMAKE_CFLAGS    += -O3
QMAKE_CXXFLAGS  += -O3 

INCLUDEPATH += src src/ui src/util src/engine
DEPENDPATH  += src src/ui src/util src/engine

LIBS += -lm

RESOURCES += brdf-ocean.qrc

SOURCES += src/engine/engine.cpp \
           src/engine/oceanrenderer.cpp \
           src/engine/projectedgrid.cpp \
           src/main.cpp \
           src/ui/glwidget.cpp \
           src/ui/inputhandler.cpp \
           src/ui/mainwindow.cpp \
           src/util/camera.cpp \
           src/util/matrix.cpp

HEADERS += src/engine/engine.h \
           src/engine/oceanrenderer.h \
           src/engine/projectedgrid.h \
           src/ui/glwidget.h \
           src/ui/inputhandler.h \
           src/ui/mainwindow.h \
           src/util/camera.h \
           src/util/matrix.h \
           src/util/vector.h

DISTFILES += \
    src/shaders/ocean.frag \
    src/shaders/ocean.vert
