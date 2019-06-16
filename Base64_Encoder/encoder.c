#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#define B64_BUF 16384

char tob64[] = 
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

char* encode_base64(unsigned char* src,size_t ssize)
{
  char* outbuf;
  char* retbuf;
  int pos;
  int i, l, left;
  unsigned long triple;
  unsigned char *inb;

  
  
  //error(235, "encode base64");
  /* Exit on empty input */
  if (!ssize||src==NULL){
    error(240,"\n");
    return NULL;
  }

  outbuf = (char *)malloc(sizeof(char)*B64_BUF);

  /* Initialize working pointers */
  inb = src;
  i = 0;
  triple = 0;
  pos = 0;
  left = ssize;
  //error(235, ", data length: %d\n", left);
  /*
   * Process entire inbuf.
   */
  while (left != 0)
    {
      i++;
      left--;
      
      triple = (triple <<8) | *inb;
      if (i == 3 || left == 0)
	{
	  switch (i) 
	    {
	    case 1:
	      triple = triple<<4;
	      break;
	    case 2:
	      triple = triple<<2;
	      break;
	    default:
	      break;
	    }
	  for (l = i; l >= 0; l--){
	    /* register */ 
	    int rr; 
	    rr = 0x3f & (triple>>(6*l)); 
	    assert (rr < 64); 
	    outbuf[pos]=tob64[rr];
	    pos++;
	      }
	  if (left == 0)
	    switch(i)
	      {
	      case 2:
		outbuf[pos]='=';
		pos++;
		break;
	      case 1:
		outbuf[pos]='=';
		pos++;
		outbuf[pos]='=';
		pos++;
		break;
	      default:
		break;
	      }
	  triple = 0;
	  i = 0;
	  }
      inb++;
  }
  
  /* outbuf is not completely used so we use retbuf */
  retbuf=(char*)malloc(sizeof(char)*(pos+1));
  memcpy(retbuf,outbuf,pos);
  retbuf[pos]='\0';
  free(outbuf);

  return retbuf;
}

void print(char *t) {
   if (*t == '\0')
      return;
   printf("%c", *t);
   print(++t);
}

int main(int argc, char const *argv[])
{
	printf("Inizio programma. \n");
	FILE *f = fopen("test.txt", "r");
	printf("Apertura file. \n");
    if(f==NULL) {
    	printf("Error opening file. Closing program.\n");
    	return -1;
    }
    printf("Il file non e' null. \n");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    rewind(f);
    
    unsigned char* buffer = (unsigned char*)malloc(sizeof(unsigned char) * fsize);
    
    size_t result = fread (buffer, 1, fsize, f);
    if (result != fsize) {
		printf("Reading error. \n");
		return 0;
    }

    printf("Stringa di input: \n");
    print(buffer);
    printf("\n");
    
    unsigned char* final = encode_base64(buffer, result);

    printf("Stringa convertita: \n");
    print(final);
    printf("\n");
    
    fclose(f);
    free(buffer);

    printf("Esecuzione finita. \n");

	return 0;
}
