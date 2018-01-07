#ifndef _JPEGDEC_H__
#define _JPEGDEC_H__

#pragma pack(1)
typedef struct {
    WORD soi;
    WORD app;
    WORD data_len;
    /*
    DWORD identifier;
    BYTE null;
    BYTE major_v;
    BYTE minor_v;
    BYTE units;
    WORD X_density;
    WORD Y_density;
    BYTE Xthumbnail;
    BYTE Ythumbnail;
    */

}HEADER;

typedef struct _DQT_markers{
    WORD DQT_mark;
    WORD data_len;
}DQT;

#define BYTE unsigned char 
#define WORD unsigned short

#define DWORD unsigned int
#define SDWORD signed int

#define SBYTE signed char
#define SWORD signed short int


//marker
#define SOI 0xD8FF // Start of Image
#define EOI 0XD9 // End of Image
#define APP0 0xE0FF //application specific data
#define SOF 0xC0 //Start of Frame
#define DQT 0xDBFF //Define Quantization Tables
#define DHT 0xC4 //Deine Huffman Tables
#define SOS 0xDA //Start of Scan
#define DRI 0xDD //Define Restart interval
#define COM 0xFE //Commnet



WORD to_big(WORD a) {
    unsigned char bytes[2];
    bytes[0] = (unsigned char)((a >> 0) & 0xff);
    bytes[1] = (unsigned char)((a >> 8) & 0xff);

    WORD result;
    result = ((WORD)bytes[0] << 8) |
        ((WORD)bytes[1] << 0);
    return result;
}

#endif