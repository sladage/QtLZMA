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
#include "lzma.h"
#include "lzma/Alloc.h"
#include "lzma/LzmaDec.h"
#include "lzma/LzmaEnc.h"
#include "lzma/Lzma2Dec.h"
#include "lzma/Lzma2Enc.h"
#include <QDataStream>
#include <QIODevice>

#define IN_BUF_SIZE (1 << 16)
#define OUT_BUF_SIZE (1 << 16)

static void *SzAlloc(void *p, size_t size) { p = p; return MyAlloc(size); }
static void SzFree(void *p, void *address) { p = p; MyFree(address); }
static ISzAlloc g_Alloc = { SzAlloc, SzFree };

typedef struct
{
    ISeqInStream s;
    QIODevice* io;
} QIODevInStream;

typedef struct
{
    ISeqOutStream s;
    QIODevice* io;
} QIODevOutStream;

static SRes qioread(void *p, void* buf, size_t* size)
{
    QIODevInStream* io = (QIODevInStream*)p;
    int r = io->io->read((char*)buf,*size);
    if (r >= 0)
    {
        *size = r;
        return SZ_OK;
    }
    return SZ_ERROR_READ;
}

static size_t qiowrite(void *p, const void* buf, size_t size)
{
    QIODevOutStream* io = (QIODevOutStream*)p;
    return io->io->write((const char*)buf,size);
}

int LZMA::encode(const QByteArray &in, QByteArray *out, QByteArray &headerdata, int compression_level)
{
    CLzmaEncHandle enc;
    QDataStream instream(in);
    QDataStream outstream(out,QIODevice::WriteOnly);
    SRes res;
    CLzmaEncProps props;

    QIODevInStream inStream;
    QIODevOutStream outStream;

    inStream.s.Read = &qioread;
    inStream.io = instream.device();

    outStream.s.Write = &qiowrite;
    outStream.io = outstream.device();

    enc = LzmaEnc_Create(&g_Alloc);
    if (enc == 0)
      return SZ_ERROR_MEM;

    LzmaEncProps_Init(&props);
    props.level = compression_level;
    res = LzmaEnc_SetProps(enc, &props);

    if (res == SZ_OK)
    {
        Byte header[LZMA_PROPS_SIZE];
        size_t headerSize = LZMA_PROPS_SIZE;

        res = LzmaEnc_WriteProperties(enc, header, &headerSize);
        headerdata = QByteArray::fromRawData((char*)(&header),headerSize);

        if (res == SZ_OK)
        {
            //res = Encode2(enc,&instream,&outstream);
            res = LzmaEnc_Encode(enc, &outStream.s, &inStream.s, NULL, &g_Alloc, &g_Alloc);
        }

    }
    LzmaEnc_Destroy(enc, &g_Alloc, &g_Alloc);

    return res;
}

static SRes lzmadecode(CLzmaDec *state, QDataStream* instream, QDataStream* outstream, quint64 uncompressed_size)
{
    int thereIsSize = (uncompressed_size != (UInt64)(Int64)-1);
    Byte inBuf[IN_BUF_SIZE];
    Byte outBuf[OUT_BUF_SIZE];
    size_t inPos = 0, inSize = 0, outPos = 0;
    LzmaDec_Init(state);
    for (;;)
    {
      if (inPos == inSize)
      {
        inSize = IN_BUF_SIZE;
        //RINOK(instream->readRawData((char*)(&inBuf), inSize));
        inSize = instream->readRawData((char*)(&inBuf), inSize);
        if (inSize == -1)
            return SZ_ERROR_READ;
        inPos = 0;
      }
      {
        SRes res;
        SizeT inProcessed = inSize - inPos;
        SizeT outProcessed = OUT_BUF_SIZE - outPos;
        ELzmaFinishMode finishMode = LZMA_FINISH_ANY;
        ELzmaStatus status;
        if (thereIsSize && outProcessed > uncompressed_size)
        {
          outProcessed = (SizeT)uncompressed_size;
          finishMode = LZMA_FINISH_END;
        }

        res = LzmaDec_DecodeToBuf(state, outBuf + outPos, &outProcessed,
          inBuf + inPos, &inProcessed, finishMode, &status);
        inPos += inProcessed;
        outPos += outProcessed;
        uncompressed_size -= outProcessed;


        if (outstream->writeRawData((char*)(&outBuf), outPos) != outPos)
            return SZ_ERROR_WRITE;

        outPos = 0;

        if (res != SZ_OK || thereIsSize && uncompressed_size == 0)
          return res;

        if (inProcessed == 0 && outProcessed == 0)
        {
          if (thereIsSize || status != LZMA_STATUS_FINISHED_WITH_MARK)
            return SZ_ERROR_DATA;
          return res;
        }
      }
    }
}

int LZMA::decode(const QByteArray &in, QByteArray *out, quint64 uncompressed_size, QByteArray headerdata)
{
    CLzmaDec state;
    QDataStream outstream(out,QIODevice::WriteOnly);
    QDataStream instream(in);

    /* header: 5 bytes of LZMA properties */
    unsigned char header[LZMA_PROPS_SIZE];

    /* Read and parse header */
    memcpy(&header,headerdata.constData(),LZMA_PROPS_SIZE);

    LzmaDec_Construct(&state);
    RINOK(LzmaDec_Allocate(&state, header, LZMA_PROPS_SIZE, &g_Alloc));

    SRes res = lzmadecode(&state, &instream, &outstream, uncompressed_size);

    LzmaDec_Free(&state, &g_Alloc);
    return res;
}

int LZMA::encode2(const QByteArray &in, QByteArray *out, unsigned char* lzmaprop, int compression_level)
{
    CLzma2EncHandle enc;
    QDataStream instream(in);
    QDataStream outstream(out,QIODevice::WriteOnly);
    SRes res;
    CLzma2EncProps props;

    QIODevInStream inStream;
    QIODevOutStream outStream;

    inStream.s.Read = &qioread;
    inStream.io = instream.device();

    outStream.s.Write = &qiowrite;
    outStream.io = outstream.device();

    enc = Lzma2Enc_Create(&g_Alloc,&g_Alloc);
    if (enc == 0)
      return SZ_ERROR_MEM;

    Lzma2EncProps_Init(&props);

    props.lzmaProps.level = compression_level;

    res = Lzma2Enc_SetProps(enc, &props);

    if (res == SZ_OK)
    {
        //Byte header;

        *lzmaprop = Lzma2Enc_WriteProperties(enc);
        //outstream << header;

        res = Lzma2Enc_Encode(enc, &outStream.s, &inStream.s, NULL);

    }
    Lzma2Enc_Destroy(enc);

    return res;
}

static SRes lzmadecode2(CLzma2Dec *state, QDataStream* instream, QDataStream* outstream, quint64 uncompressed_size)
{
    int thereIsSize = (uncompressed_size != (UInt64)(Int64)-1);
    Byte inBuf[IN_BUF_SIZE];
    Byte outBuf[OUT_BUF_SIZE];
    size_t inPos = 0, inSize = 0, outPos = 0;
    Lzma2Dec_Init(state);
    for (;;)
    {
      if (inPos == inSize)
      {
        inSize = IN_BUF_SIZE;
        //RINOK(instream->readRawData((char*)(&inBuf), inSize));
        inSize = instream->readRawData((char*)(&inBuf), inSize);
        if (inSize == -1)
            return SZ_ERROR_READ;
        inPos = 0;
      }
      {
        SRes res;
        SizeT inProcessed = inSize - inPos;
        SizeT outProcessed = OUT_BUF_SIZE - outPos;
        ELzmaFinishMode finishMode = LZMA_FINISH_ANY;
        ELzmaStatus status;
        if (thereIsSize && outProcessed > uncompressed_size)
        {
          outProcessed = (SizeT)uncompressed_size;
          finishMode = LZMA_FINISH_END;
        }

        res = Lzma2Dec_DecodeToBuf(state, outBuf + outPos, &outProcessed,
          inBuf + inPos, &inProcessed, finishMode, &status);
        inPos += inProcessed;
        outPos += outProcessed;
        uncompressed_size -= outProcessed;


        if (outstream->writeRawData((char*)(&outBuf), outPos) != outPos)
            return SZ_ERROR_WRITE;

        outPos = 0;

        if (res != SZ_OK || thereIsSize && uncompressed_size == 0)
          return res;

        if (inProcessed == 0 && outProcessed == 0)
        {
          if (thereIsSize || status != LZMA_STATUS_FINISHED_WITH_MARK)
            return SZ_ERROR_DATA;
          return res;
        }
      }
    }
}

int LZMA::decode2(const QByteArray &in, QByteArray *out, quint64 uncompressed_size, unsigned char lzmaprop)
{
    CLzma2Dec state;
    QDataStream outstream(out,QIODevice::WriteOnly);

    QDataStream instream(in);

    Lzma2Dec_Construct(&state);
    RINOK(Lzma2Dec_Allocate(&state, lzmaprop, &g_Alloc));

    SRes res = lzmadecode2(&state, &instream, &outstream, uncompressed_size);

    Lzma2Dec_Free(&state, &g_Alloc);
    return res;
}
