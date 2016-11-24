QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RayTracer
TEMPLATE = app

QMAKE_CXXFLAGS += -Wall
CONFIG += c++14

SOURCES += main.cpp \
    ray_tracer.cpp

HEADERS  += \
    geometry.h \
    ray_tracer.h
