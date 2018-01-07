#include <stdio.h>
#include <stdlib.h>
//// it use little endian ////


#include <Windows.h>
#include "jpegdec.h"



int main() {

    char tmp[5];
    FILE* jpg;
    fopen_s(&jpg, "test.jpg", "rb");
    if (jpg == NULL) {
        printf("error");
        return -1;
    }

    
    HEADER header;
    
    fread(&header, sizeof(HEADER), 1, jpg);
    if (header.soi != SOI) {
        printf("different SOI");
        return -1;
    }
    if (header.app != APP0) {
        printf("different APP0");
        return -1;
    }

    header.data_len = to_big(header.data_len);
    
    BYTE* additional_data = (BYTE*)malloc(sizeof(BYTE)*(header.data_len - 2));
    fread(additional_data, sizeof(BYTE)*(header.data_len-2), 1, jpg);
    
    //To Do: check additional data  --> Read Commnet, too ( FFEE )_

   

    ////////DQT : Define Quantization Tables/////////////
    WORD isDQT;
    BYTE* DQT_table[4];
    

    while (fread(&isDQT, sizeof(WORD), 1, jpg)) {
        if (isDQT != DQT) break;
        WORD table_size;
        BYTE idx;
        fread(&table_size, sizeof(WORD), 1, jpg);
        fread(&idx, sizeof(BYTE), 1, jpg);
        table_size = to_big(table_size);
        DQT_table[idx] = (BYTE*)malloc(sizeof(BYTE)*(table_size - 3));
        fread(DQT_table[idx], sizeof(BYTE)*(table_size - 3), 1, jpg);
        idx++;
    };

   fseek(jpg, -2L, SEEK_CUR);
    
}

