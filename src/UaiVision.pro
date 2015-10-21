#-------------------------------------------------
#
# Project created by QtCreator 2015-06-02T10:09:38
#
#-------------------------------------------------

QT      += core gui
QT      += opengl
CONFIG  += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = UaiVision
TEMPLATE = app

INCLUDEPATH += /usr/local/include/opencv
INCLUDEPATH += /usr/local/include/aruco


LIBS += -L/usr/local/lib
LIBS += -lopencv_calib3d -lopencv_contrib -lopencv_core -lopencv_features2d
LIBS += -lopencv_flann -lopencv_gpu -lopencv_highgui -lopencv_imgproc -lopencv_legacy
LIBS += -lopencv_ml -lopencv_nonfree -lopencv_objdetect -lopencv_ocl -lopencv_photo
LIBS += -lopencv_stitching -lopencv_superres -lopencv_ts -lopencv_video -lopencv_videostab
LIBS += -lrt -lpthread -lm -ldl

LIBS += -laruco
LIBS += -lglut -lGLU


SOURCES += main.cpp\
        mainwindow.cpp \
    mainqthread.cpp \
    camera.cpp \
    utils.cpp \
    configdao.cpp \
    dbscam.cpp \
    glwindow.cpp \
    myglwidget.cpp \
    calibpoint.cpp \
    showimage.cpp \
    selectthrashold.cpp \
    colorspace.cpp \
    selectborder.cpp \
    videolabel.cpp \
    robot.cpp

HEADERS  += mainwindow.h \
    mainqthread.h \
    camera.h \
    utils.h \
    configdao.h \
    dbscam.h \
    glwindow.h \
    myglwidget.h \
    calibpoint.h \
    showimage.h \
    selectthrashold.h \
    colorspace.h \
    selectborder.h \
    videolabel.h \
    robot.h

FORMS    += mainwindow.ui \
    glwindow.ui \
    showimage.ui \
    selectthrashold.ui \
    selectborder.ui

DISTFILES += \
    UAISoccer.png \
    config/UaiVision.json
