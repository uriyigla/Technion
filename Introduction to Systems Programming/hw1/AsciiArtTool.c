
#include "AsciiArtTool.h"
#include "RLEList.h"
#include <stdio.h>
#include <stdlib.h>


#define BUFFER_ASSIST 2


RLEList asciiArtRead(FILE* in_stream){
    RLEList list=RLEListCreate();
    if ( list==NULL ){
        return NULL;
    }
    char buffer [BUFFER_ASSIST];
    while(fgets(buffer,BUFFER_ASSIST,in_stream)!=NULL){
        RLEListAppend(list,buffer[0]);
    }
    return list;
}

RLEListResult asciiArtPrint(RLEList list, FILE *out_stream){
    if (list==NULL){
        return  RLE_LIST_NULL_ARGUMENT;
    }
    int len=RLEListSize(list);
    for(int i=0;i<len;i++){
        fprintf(out_stream,"%c",RLEListGet(list,i,NULL));
    }
    return RLE_LIST_SUCCESS;
}

RLEListResult asciiArtPrintEncoded(RLEList list, FILE *out_stream){
    if (list==NULL){
        return  RLE_LIST_NULL_ARGUMENT;
    }
    fprintf(out_stream,"%s",RLEListExportToString(list,NULL));
    return RLE_LIST_SUCCESS;
}





