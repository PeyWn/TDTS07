#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int 
main(int argc, char* argv[]) {
 int i, c;
 FILE *in;
 int k,j,jj;
 short memory[100][160];
 in = fopen(argv[1], "r");
 if (in==NULL) {
	printf("Input file error\n"); exit(1);
 }
 k=0;j=0;
 while(1) {
//   for (i = 0; i < 160 && (c = fgetc(in)) != EOF; i++) {
     for (i = 0; i < 160; i++) {
	//printf("%c",c);
	c = fgetc(in);
	memory[k][i]=c;
   }
   k++;
   if (c == EOF ) break;
 }

 /*
 for (j=0;j<k-1;j++) {
   for(jj=0;jj<160;jj++)
     printf("%c", memory[j][jj]);
 }
 for(jj=0;jj<i;jj++) printf("%c", memory[k-1][jj]);
 */

 printf("char memory[]={\n");
 for (j=0;j<k-1;j++) {
   for(jj=0;jj<160;jj++)
     printf("%d, ", memory[j][jj]);
   printf("\n");
 }
 for(jj=0;jj<i-1;jj++) printf("%d, ", memory[k-1][jj]);
 printf("%d", memory[k-1][i-1]);

 printf("\n};\n");
 return 1;
}
