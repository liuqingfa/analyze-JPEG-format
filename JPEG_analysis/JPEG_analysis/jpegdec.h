#ifndef _JPEGDEC_H__
#define _JPEGDEC_H__

#pragma pack(1)

#define TRUE 1
#define FALSE 0;

typedef struct {
    WORD soi_mark;
    WORD app;
    WORD data_len;
   
}HEADER;

typedef struct {

    DWORD identifier;
    BYTE null;
    BYTE major_v;
    BYTE minor_v;
    BYTE units;
    WORD X_density;
    WORD Y_density;
    BYTE Xthumbnail;
    BYTE Ythumbnail;
    BYTE* Thumbnail;
}HEADER_ADDTIONAL;


typedef struct _DQT_markers{
    WORD DQT_mark;
    BYTE* DQT_tables;
    WORD data_len;
}DQT_MARK;

typedef struct _SOF_markers {
    WORD SOF_len;
    BYTE* SOF_data;
}SOF_MARK;

typedef struct _HUFF_tables {
    BYTE huff_id;
    WORD huff_len;
    BYTE* huff_table;
}HUF_TABLE;

typedef struct _DHT_markers {
    int numOfhuff;
    HUF_TABLE huf_tables[4];
}DHT_MARK;

typedef struct _SOS_markers {
    WORD SOS_len;
    BYTE Comp_count;
    BYTE Comp_id_Y;
    BYTE huff_id;
    BYTE Comp_id_Cb;
    BYTE huff_id_Cb;
    BYTE Comp_id_Cr;
    BYTE huff_id_Cr;

    //dct//
    BYTE spec_start;
    BYTE spec_end;
    BYTE succ_approx;

    BYTE* SOS_data;
}SOS_MARK;

typedef struct _DRI_marker {
    WORD DRI_mark;
    WORD scan_len;
    WORD restart_interval;
}DRI_MARK;

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
#define SOF 0xC0FF //Start of Frame
#define DQT 0xDBFF //Define Quantization Tables
#define DHT 0xC4FF //Deine Huffman Tables
#define SOS 0xDAFF //Start of Scan
#define DRI 0xDDFF //Define Restart interval
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



int readHeader(FILE*, HEADER*,HEADER_ADDTIONAL*, WORD);
void readDQT(FILE* jpg, DQT_MARK* dqt_mark, WORD read);

#endif