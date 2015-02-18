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

#ifndef QTLZMA_H
#define QTLZMA_H

#include <QObject>
#include <QByteArray>
#include <QMutex>

class QIODevice;

namespace LZMA
{
/*!
     * \brief Decode LZMA1 data from a QByteArray.
     * \param in
     * \param out
     * \param uncompressed_size
     * \param header
     * \return
     */
    int decode(const QByteArray &in, QByteArray *out, quint64 uncompressed_size, QByteArray header);
    /*!
     * \brief Encode LZMA1 data from a QByteArray.
     * \param in
     * \param out
     * \param header
     * \param compression_level
     * \return
     */
    int encode(const QByteArray &in, QByteArray *out, QByteArray &header, int compression_level = 5);

    /*!
     * \brief Decode LZMA2 data from a QByteArray.
     * \param in
     * \param out
     * \param uncompressed_size
     * \param lzmaprop
     * \return
     */
    int decode2(const QByteArray &in, QByteArray *out, quint64 uncompressed_size, unsigned char lzmaprop);
    /*!
     * \brief Encode LZMA2 data from a QByteArray.
     * \param in
     * \param out
     * \param lzmaprop
     * \param compression_level
     * \return
     */
    int encode2(const QByteArray &in, QByteArray *out, unsigned char *lzmaprop, int compression_level = 5);

    /*!
     * \brief The LZMACoder class is used for en/de-coding QIODevice streams.
     */
    class LZMACoder : public QObject
    {
        Q_OBJECT
    public:

        LZMACoder(QObject *parent=0);

        /*!
         * \brief Set the input/output.
         * \param input
         * \param output
         */
        void setIO(QIODevice *input ,QIODevice *output);
        /*!
         * \brief Set the LZMA property to be used when decoding.
         * \param lzmaprop
         */
        void setLzmaProp(unsigned char lzmaprop);
        /*!
         * \brief Set the uncompressed size to be used when decoding.
         * \param uncompressed_size
         */
        void setUncompressedSize(quint64 uncompressed_size);

    public slots:

        /*!
         * \brief Decode input to output using LZMA2.
         */
        void decode2();
        /*!
         * \brief Encode input to output using LZMA2.
         * \param compression_level
         */
        void encode2(int compression_level = 5);

    signals:

        /*!
         * \brief This signal is emitted when LZMACoder is done en/de-coding.
         * \param lzmaprop Contains the lzma property data when done encoding. Can be ignored when decoding.
         */
        void done(unsigned char lzmaprop);
        /*!
         * \brief Progres of the current en/de-coding in progress.
         * \param i Value range 0-100
         */
        void progress(int i);
        /*!
         * \brief Signal is emitted when an error occurred.
         * \param errcode LZMA error code. Lookup the meaning in the LZMA documentation.
         */
        void error(int errcode);

    private:
        QMutex m_CoderMutex;
        quint64 m_iUncompressedSize;
        unsigned char m_ucLzmaProp;
        QIODevice *m_pInput;
        QIODevice *m_pOutput;
    };

}

#endif // QTLZMA_H
