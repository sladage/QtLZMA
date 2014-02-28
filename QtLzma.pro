#-------------------------------------------------
#
# Project created by QtCreator 2014-02-26T15:24:06
#
#-------------------------------------------------

QT       -= gui

TARGET = QtLzma
TEMPLATE = lib
CONFIG += staticlib

INCLUDEPATH += include \
               src

DEFINES += _7ZIP_ST

SOURCES += src/qlzma.cpp \
    src/lzma/LzmaDec.c \
    src/lzma/LzmaEnc.c \
    src/lzma/Alloc.c \
    src/lzma/LzFind.c \
    src/lzma/Lzma2Dec.c \
    src/lzma/Lzma2Enc.c

HEADERS += include/qlzma.h \
    src/lzma/LzmaDec.h \
    src/lzma/LzmaEnc.h \
    src/lzma/Alloc.h \
    src/lzma/Types.h \
    src/lzma/LzFind.h \
    src/lzma/Lzma2Dec.h \
    src/lzma/Lzma2Enc.h \
    src/lzma/Types.h \
    src/qlzmacoder.h \
    src/lzma/LzHash.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
