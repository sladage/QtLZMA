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
#ifndef QLZMA_H
#define QLZMA_H

#include <QObject>
#include <QByteArray>

class QIODevice;
class QLZMACoder;
class QThread;

class QLZMA : public QObject
{
    Q_OBJECT
public:
    QLZMA(QObject* parent=0);
    ~QLZMA();

    static int decode(QByteArray &in, QByteArray &out, quint64 uncompressed_size);
    static int encode(QByteArray &in, QByteArray &out);

    void decode(QIODevice* in, QIODevice* out, quint64 uncompressed_size);
    void encode(QIODevice* in, QIODevice* out);

signals:
    void finished();
    void onError(QString error);
    void progress(int value);

private:
    QLZMACoder* m_pCoder;
    QThread* m_pThread;
};

#endif // QLZMA_H
