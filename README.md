QtLZMA
======

LZMA library for Qt.

### Features

-Encode/Decode LZMA and LZMA2 data from a QByteArray.

### TODO

-Add convience functions for (en/de)-coding which append the size and header.
-Add proper threaded encoding and decoding support.
-Add support for (en/de)-coding via QIODevices.

### Usage

Encoding QByteArray with LZMA2:

```
QByteArray datain = "testdata";
QByteArray dataout;
int size = datain.size();
unsigned char prop;
int r = QLZMA::encode2(datain,dataout,&prop);

if (r != 0)
{
    std::cout << "Error: " << r << "\n";
    return;
}

//when storing the data, don't forget to also store the LZMA2 prop and datain size.
```

Decoding QByteArray with LZMA2:
```
QByteArray datain = <lzma2 data>;
QByteArray dataout;
int size = <uncompressed file size>;
unsigned char prop = <lzma2 prop>;

int r = QLZMA::decode2(datain,dataout,size,prop);

if (r != 0)
{
    std::cout << "Error: " << r << "\n";
    return;
}
```
