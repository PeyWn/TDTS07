#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void main() {
 FILE *fp;
 int outfd;

/* if ((outfd = open("sasa.gsm", O_CREAT, 0666)) >= 0) {*/
   if ((outfd = open("sasa\0", O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU )) >= 0) {
   printf("OK\n");
   fp = fdopen();
   printf("%d \n\n", write(outfd, "sasa is smart\n", 14));
 }

 close(outfd);
 /*
 FILE *fp;

 fp = fopen("f1", "w");
 fprintf(fp, "sasa\n"); fflush(stdout);
 fclose(fp);
 */

}
