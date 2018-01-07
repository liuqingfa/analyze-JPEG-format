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
    dht_mark.numOfhuff = 0;

    char tmp[5];


    /////file open////
    FILE* jpg;
    fopen_s(&jpg, "test.jpg", "rb");
    if (jpg == NULL) {
        printf("error");
        return -1;
    }

    int check_error = TRUE;
    WORD tmpword;
   // while (1) {
        fread(&tmpword, sizeof(WORD), 1, jpg);
        switch (tmpword) {
        case SOI: check_error = readHeader(jpg, &header, &header_additional, tmpword); break;
       // case DQT: readDQT(jpg, dqt_mark, tmpword); break;
        }
        printf("tmpword : %x", tmpword);
        //if (check_error != TRUE) break;
        //if (tmpword == SOI) break;
    //}
    if (check_error != TRUE) return -1;

    fread(&tmpword, sizeof(WORD), 1, jpg);
    printf("next : %x\n", tmpword);
    /*
    

   
    ////////DQT : Define Quantization Tables/////////////
    WORD isDQT;
    
    while (fread(&isDQT, sizeof(WORD), 1, jpg)) {
        if (isDQT != DQT) break;
        WORD table_size;
        BYTE idx;
        fread(&table_size, sizeof(WORD), 1, jpg);
        fread(&idx, sizeof(BYTE), 1, jpg);
        dqt_mark[idx].data_len = table_size = to_big(table_size);
        dqt_mark[idx].DQT_tables = (BYTE*)malloc(sizeof(BYTE)*(table_size - 3));
        fread(dqt_mark[idx].DQT_tables , sizeof(BYTE)*(table_size - 3), 1, jpg);
        idx++;
    };

   fseek(jpg, -2L, SEEK_CUR);

   //////start of  frame////////
   fread(&isSOF, sizeof(WORD), 1, jpg);
   if (isSOF != SOF) {
       printf("%x\n", isSOF);
       printf("invalid format");
       return -1;
   }
   fread(&(sof_mark.SOF_len), sizeof(BYTE) * 2,1,jpg);
   sof_mark.SOF_len = to_big(sof_mark.SOF_len);
   sof_mark.SOF_data = (BYTE*)malloc(sizeof(BYTE)*(sof_mark.SOF_len - 2));
   fread(sof_mark.SOF_data, sizeof(BYTE)*(sof_mark.SOF_len - 2), 1, jpg);

   
   ////////DHT marker : huffman code table//////
   WORD isDHT;
   int idx = 0;
   while (1) {
       fread(&isDHT, sizeof(WORD), 1, jpg);
       if (isDHT != DHT) break;
       
       WORD hf_len;
       fread(&hf_len, sizeof(WORD), 1, jpg);
       hf_len = to_big(hf_len);
       dht_mark.huf_tables[idx].huff_len = hf_len-2;
       dht_mark.huf_tables[idx].huff_table = (BYTE*)malloc(sizeof(BYTE)*(hf_len - 2));
       fread(dht_mark.huf_tables[idx].huff_table, sizeof(BYTE)*(hf_len - 2), 1, jpg);
       idx++;   
   }

   dht_mark.numOfhuff = idx;
   fseek(jpg, -2L, SEEK_CUR);
   

   /////SOS : Start of Scan //////////
   WORD isSOS;
   fread(&isSOS, sizeof(WORD), 1, jpg);

   if (isSOS != SOS) {
       printf("invalid \n");
       return -1;
   }

   fread(&sos_mark, sizeof(SOS_MARK), 1, jpg);

    //// DRI : Define Restart interval/////
   fread(&dri_mark, sizeof(DRI_MARK), 1, jpg);
   if (dri_mark.DRI_mark != DRI) {
       printf("invalid\n");
       return -1;
   }

   
 
*/
   // if(header_additional.Thumbnail != NULL) free(header_additional.Thumbnail);
}







int readHeader(FILE* jpg, HEADER* header ,HEADER_ADDTIONAL* header_additional, WORD read) {
    fread(&header, sizeof(HEADER)-2, 1, jpg);
    header->soi_mark = read;
    if (header->soi_mark != SOI || header->app != APP0) {
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
    printf("tmp  : %s", tmp);
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
        printf("dqt_mark : %x\n", dqt_mark->DQT_mark);
    };

    fseek(jpg, -2L, SEEK_CUR);
}