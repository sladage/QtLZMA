QT       -= gui

TARGET = qtlzma
TEMPLATE = lib
CONFIG += staticlib

DEFINES += _7ZIP_ST

SOURCES += \
    lzma/LzmaDec.c \
    lzma/LzmaEnc.c \
    lzma/Alloc.c \
    lzma/LzFind.c \
    lzma/Lzma2Dec.c \
    lzma/Lzma2Enc.c \
    lzma.cpp

HEADERS += \
    lzma/LzmaDec.h \
    lzma/LzmaEnc.h \
    lzma/Alloc.h \
    lzma/Types.h \
    lzma/LzFind.h \
    lzma/Lzma2Dec.h \
    lzma/Lzma2Enc.h \
    lzma/Types.h \
    lzma/LzHash.h \
    lzma.h
