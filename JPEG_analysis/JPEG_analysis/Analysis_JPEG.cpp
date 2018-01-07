#include <stdio.h>
#include <stdlib.h>
//// it use little endian ////


#include <Windows.h>
#include "jpegdec.h"



int main() {
    WORD isSOF;
    HEADER header;
    HEADER_ADDTIONAL header_additional;
    SOF_MARK sof_mark;
    DQT_MARK dqt_mark[4];
    DHT_MARK dht_mark;
    SOS_MARK sos_mark;
    DRI_MARK dri_mark;
    BYTE* scandata = NULL;
    dht_mark.numOfhuff = 0;

    char tmp[5];


    /////file open////
    FILE* jpg;
    fopen_s(&jpg, "test.jpg", "rb");
    if (jpg == NULL) {
        printf("error");
        return -1;
    }

    fseek(jpg, 0, SEEK_END);
    long jpgsize = ftell(jpg);
    fseek(jpg, 0, SEEK_SET);

    int check_error = TRUE;
    WORD tmpword;

    while (!feof(jpg)) {

        fread(&tmpword, sizeof(WORD), 1, jpg);

        switch (tmpword) {
        
        case SOI: readHeader(jpg, &header, &header_additional, tmpword);
            break;
        case DQT: readDQT(jpg, dqt_mark, tmpword);
            break;
        case SOF: readSOF(jpg, &sof_mark, tmpword); break;
        case DHT: readDHT(jpg, &dht_mark, tmpword); break;
        case SOS: readSOS(jpg, &sos_mark, tmpword, jpgsize,scandata); break;
        case DRI: fread(&dri_mark, sizeof(DRI_MARK), 1, jpg); break;
            //To do : if restart  interval >0? RST
        default: check_error = FALSE;
        
        }

        if (check_error != TRUE) {
            printf("invalid file format\n");
            return -1;
        }
    }



}



int readHeader(FILE* jpg, HEADER* header ,HEADER_ADDTIONAL* header_additional, WORD read) {

    fread(header, sizeof(HEADER), 1, jpg);
    if (read != SOI || header->app != APP0) {
        printf("invalid Header\n");
        return FALSE;
    }
    
    header->data_len = to_big(header->data_len);
    header_additional->Thumbnail = (BYTE*)malloc(sizeof(BYTE)*(header_additional->Xthumbnail * header_additional->Ythumbnail));
    fread(header_additional, sizeof(BYTE)*(header->data_len - 2), 1, jpg);
    char tmp[5];
    memcpy(tmp, &(header_additional->identifier), 4);
    tmp[4] = 0x00;
    if (strcmp(tmp, "JFIF") != 0) {
        printf("NOT JFIF\n");
        return FALSE;
    }
    return TRUE;
}

void readDQT(FILE* jpg, DQT_MARK* dqt_mark, WORD read) {
    ////////DQT : Define Quantization Tables/////////////
    
    dqt_mark->DQT_mark = read;
    while (1) {
        if (dqt_mark->DQT_mark != DQT) break;
        WORD table_size;
        BYTE idx;
        fread(&table_size, sizeof(WORD), 1, jpg);
        fread(&idx, sizeof(BYTE), 1, jpg);
        dqt_mark[idx].data_len = table_size = to_big(table_size);
        dqt_mark[idx].DQT_tables = (BYTE*)malloc(sizeof(BYTE)*(table_size - 3));
        fread(dqt_mark[idx].DQT_tables, sizeof(BYTE)*(table_size - 3), 1, jpg);
        idx++;

        fread(&(dqt_mark->DQT_mark), sizeof(WORD), 1, jpg);
    };

    fseek(jpg, -2L, SEEK_CUR);
}

void readSOF(FILE* jpg, SOF_MARK* sof_mark, WORD read) {
    //////start of  frame////////
    fread(&(sof_mark->SOF_len), sizeof(BYTE) * 2, 1, jpg);
    sof_mark->SOF_len = to_big(sof_mark->SOF_len);
    sof_mark->SOF_data = (BYTE*)malloc(sizeof(BYTE)*(sof_mark->SOF_len - 2));
    fread(sof_mark->SOF_data, sizeof(BYTE)*(sof_mark->SOF_len - 2), 1, jpg);

}

void readDHT(FILE* jpg, DHT_MARK* dht_mark, WORD read) {
    ////////DHT marker : huffman code table//////
    
    int idx = 0;
    while (1) {
        if (read != DHT) break;

        WORD hf_len;
        fread(&hf_len, sizeof(WORD), 1, jpg);
        hf_len = to_big(hf_len);
        dht_mark->huf_tables[idx].huff_len = hf_len - 2;
        dht_mark->huf_tables[idx].huff_table = (BYTE*)malloc(sizeof(BYTE)*(hf_len - 2));
        fread(dht_mark->huf_tables[idx].huff_table, sizeof(BYTE)*(hf_len - 2), 1, jpg);
        idx++;
        fread(&read, sizeof(WORD), 1, jpg);
    }

    dht_mark->numOfhuff = idx;
    fseek(jpg, -2L, SEEK_CUR);

}

void readSOS(FILE* jpg, SOS_MARK* sos_mark, WORD read,long filesize, BYTE* scandata) {
    fread(sos_mark, sizeof(SOS_MARK), 1, jpg);
    long cur_location = ftell(jpg);
    
    scandata = (BYTE*)malloc(sizeof(BYTE)*(filesize - cur_location-2));
    fread(scandata, sizeof(BYTE)*(filesize - cur_location-2), 1, jpg);
    WORD fincheck;
    fread(&fincheck, sizeof(WORD), 1, jpg);
    printf("fincheck : %x\n\n", fincheck);
    if (fincheck != EOI)  printf("not finished\n");
    
}
