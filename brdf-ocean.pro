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

SOURCES += main.cpp \
           mainwindow.cpp \
           glwidget.cpp \
           camera.cpp \
           matrix.cpp \
           engine.cpp \
           inputhandler.cpp \
           projectedgrid.cpp \
           oceanrenderer.cpp

HEADERS += mainwindow.h \
           glwidget.h \
           camera.h \
           vector.h \
           matrix.h \
           engine.h \
           inputhandler.h \
           projectedgrid.h \
           oceanrenderer.h
