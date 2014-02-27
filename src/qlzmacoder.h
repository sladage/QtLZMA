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
#ifndef QLZMACODER_H
#define QLZMACODER_H

#include "qlzma.h"
#include "lzma/Types.h"
#include "lzma/LzmaDec.h"


class QLZMACoder : public QObject
{
    Q_OBJECT
public:
    QLZMACoder(QLZMA* p);

    void decode(QIODevice* in, QIODevice* out, quint64 uncompressed_size);

    void encode(QIODevice* in, QIODevice* out);

    SRes dodecode(CLzmaDec *state, QIODevice* instream, QIODevice* outstream, quint64 uncompressed_size);
    void updateProgress(int v);

public slots:

    void _decode(QIODevice* in, QIODevice* out, quint64 uncompressed_size);
    void _encode(QIODevice* in, QIODevice* out);

signals:

    void s_decode(QIODevice* in, QIODevice* out, quint64 uncompressed_size);
    void s_encode(QIODevice* in, QIODevice* out);

    void finished();
    void onError(QString error);
    void progress(int value);

private:
    QLZMA* m_pQLZMA;
};

#endif // QLZMACODER_H
