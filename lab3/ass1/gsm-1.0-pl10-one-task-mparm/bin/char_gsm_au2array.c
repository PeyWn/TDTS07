#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>



int 
main(int argc, char* argv[]) {
 int i, c;
 FILE *in;
 int k,j,jj;
 char memory[100*160];
 in = fopen(argv[1], "r");
 if (in==NULL) {
	printf("Input file error\n"); exit(1);
 }
 k=0;j=0;
 while(!(feof(in))) {
//   for (i = 0; i < 160 && (c = fgetc(in)) != EOF; i++) {
        //printf("%c",c);
        c = fgetc(in);
	memory[k++]=c;
	printf("%d %c %d\n",c, memory[k-1], memory[k-1] ); 
 }

 exit(1);
 printf("char memory[]={\n");
 for(jj=0;jj<2566;jj++) printf("%d, ", memory[jj]);
 printf("\n};\n");

 return 1;
}
