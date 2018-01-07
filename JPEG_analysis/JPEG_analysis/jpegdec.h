#ifndef _JPEGDEC_H__
#define _JPEGDEC_H__

typedef struct {
    WORD soi;
    WORD app;
    WORD add_len;
    DWORD jfif;
    BYTE null;
    WORD version;
    BYTE numOfPix;
    WORD hori_resol;
    WORD perp_resol;
    WORD show;
    

}HEADER;


#define BYTE unsigned char 
#define WORD unsigned short int

#define DWORD unsigned int
#define SDWORD signed int

#define SBYTE signed char
#define SWORD signed short int


//marker
#define SOI 0xD8FF // Start of Image
#define EOI 0XD9 // End of Image
#define APP0 0xE0FF //application specific data
#define SOF 0xC0 //Start of Frame
#define DQT 0xDB //Define Quantization Tables
#define DHT 0xC4 //Deine Huffman Tables
#define SOS 0xDA //Start of Scan
#define DRI 0xDD //Define Restart interval
#define COM 0xFE //Commnet

#endif