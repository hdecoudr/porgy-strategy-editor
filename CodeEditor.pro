#-------------------------------------------------
#
# Project created by QtCreator 2016-05-25T19:17:26
#
#-------------------------------------------------

QT       += core gui
QT       += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cpp\
           mainwindow.cpp \
           codeeditorplaintextview.cpp \
           codeeditorview.cpp \
           codeeditorlistview.cpp \
           sourcecode.cpp \
           sourcecodemodel.cpp \
           generalpurposesyntaxhighlighter.cpp \
           xmlsyntaxparser.cpp \
           generalpurposesyntaxhighlightingrules.cpp \
           highlightingrule.cpp

HEADERS  += mainwindow.h \
            codeeditorplaintextview.h \
            codeeditorview.h \
            codeeditorlistview.h \
            sourcecode.h \
            sourcecodemodel.h \
            generalpurposesyntaxhighlighter.h \
            xmlsyntaxparser.h \
            generalpurposesyntaxhighlightingrules.h \
            highlightingrule.h

FORMS    += mainwindow.ui \
            codeeditorview.ui

RESOURCES += \
    code-editor-icons.qrc \
    code-editor-styles.qrc \
    code-editor-syntax.qrc
