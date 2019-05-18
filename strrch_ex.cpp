/* strrchr example */
#include <stdio.h>
#include <string.h>
#include <libgen.h>
http://www.kscodes.com/java/add-watermark-pdf-using-apache-pdfbox/

int main ()
{
  char str[] = "/This/is/sample/string";
  char * pch;
  char str1[1024];
  memset(str1,0, sizeof(str1));
  pch=strrchr(str,'/');
  while(*pch != '\0') {
    printf("%c", *pch);
    
    // move the ptr pointer to the next memory location
    pch++;
  }
  printf("\n");
  strncpy ( str1, str, pch+2-str+2);
  //printf("\ndirname %s \n", dirname(str));
  printf("dirname %s\n",str1);
  
  return 0;
}
