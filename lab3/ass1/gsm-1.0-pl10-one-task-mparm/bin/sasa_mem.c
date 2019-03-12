#include <stdio.h>
#include "sasa_mem2.h"


int
main(int argc, char* argv[]) {
 int i, c;
 FILE *in;
 int k,j,jj;

/*
 for (j=0;j<16;j++) {
   for(jj=0;jj<160;jj++)
     printf("%c", memory[j*160+jj]);
 }
 for(jj=0;jj<32;jj++) printf("%c", memory[16*160+jj]);
*/
 for (j=0;j<2566;j++)
    printf("%c", memory[j]);
}

