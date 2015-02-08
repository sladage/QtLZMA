/****************************************************************************
**
** LZMA library for Qt
**
** Copyright (C) 2014 Stefan Ladage <sladage@gmail.com>
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published
** by the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#pragma once

#include <QByteArray>

class LZMA {
public:
    static int decode(QByteArray &in, QByteArray &out, quint64 uncompressed_size, QByteArray header);
    static int encode(QByteArray &in, QByteArray &out, QByteArray &header, int compression_level = 5);

    static int decode2(QByteArray &in, QByteArray &out, quint64 uncompressed_size, unsigned char lzmaprop);
    static int encode2(QByteArray &in, QByteArray &out, unsigned char *lzmaprop, int compression_level = 5);
};
