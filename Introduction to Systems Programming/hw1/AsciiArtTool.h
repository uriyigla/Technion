//
// Created by uriy4 on 26/04/2022.
//

#ifndef HW1_RLELIST_C_ASCIIARTTOOL_H
#define HW1_RLELIST_C_ASCIIARTTOOL_H
#include "RLEList.h"

RLEList asciiArtRead(FILE* in_stream);

RLEListResult asciiArtPrint(RLEList list, FILE *out_stream);

RLEListResult asciiArtPrintEncoded(RLEList list, FILE *out_stream);

#endif //HW1_RLELIST_C_ASCIIARTTOOL_H
