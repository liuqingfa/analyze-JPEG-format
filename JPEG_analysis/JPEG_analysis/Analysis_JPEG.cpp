#include <stdio.h>
#include <stdlib.h>
//// it use little endian ////


#include <Windows.h>
#include "jpegdec.h"



int main() {

    FILE* jpg;
    fopen_s(&jpg, "test.jpg", "rb");
    if (jpg == NULL) {
        printf("error");
        return -1;
    }
    HEADER header;
    fread(&header, 20, 1, jpg);
    if (header.soi != SOI) {
        printf("different SOI");
        return -1;
    }
    if (header.app != APP0) {
        printf("different APP0");
        return -1;
    }
    printf("soi : %x\n", header.soi);
    //additional data length
    printf("additional data length: %d\n", header.add_len);



}