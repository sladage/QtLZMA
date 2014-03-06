#-------------------------------------------------
#
# Project created by QtCreator 2014-02-26T15:24:06
#
#-------------------------------------------------

QT       -= gui

TARGET = QtLzma
TEMPLATE = lib
CONFIG += staticlib

include(QtLzma.pri)

unix {
    QMAKE_POST_LINK += cp libQtLzma.a lib/

    headers.path    = /usr/local/include/QtLZMA
    headers.files   += include/qlzma.h

    target.path = /usr/local/lib
    INSTALLS += target \
                headers
}
