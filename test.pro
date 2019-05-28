QT       += core gui #testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = test
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11

SOURCES += \
    test/main.cpp \
    test/test_mainwindow.cpp \
    test/test_numerics.cpp \
    test/test_qlabeltracker.cpp \
    test/test_sbml.cpp \
    test/test_simulate.cpp \
    src/numerics.cpp \
    src/sbml.cpp \
    src/simulate.cpp

    # src/mainwindow.cpp \
    # src/qlabelmousetracker.cpp \

HEADERS += \
    inc/numerics.h \
    inc/sbml.h \
    inc/simulate.h \
    test/catch/catch.hpp \
    ext/exprtk/exprtk.hpp

    # inc/mainwindow.h \
    # inc/qlabelmousetracker.h \

INCLUDEPATH += inc ext/exprtk test/catch

QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage

# these static libraries are available from
# from https://github.com/lkeegan/libsbml-static
LIBS += $$PWD/libsbml/lib/libsbml-static.a $$PWD/libsbml/lib/libexpat.a
INCLUDEPATH += libsbml/include
