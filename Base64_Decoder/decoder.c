#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define B64_BUF 16384
#define FAIL -1
#define SKIP -2


int fromb64[] = {
FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL,  
FAIL, SKIP, SKIP, FAIL, FAIL, SKIP, FAIL, FAIL,
FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL,  
FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL,  

SKIP, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL,
FAIL, FAIL, FAIL, 0x3e, FAIL, FAIL, FAIL, 0x3f,
0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b,  
0x3c, 0x3d, FAIL, FAIL, FAIL, SKIP, FAIL, FAIL,

FAIL, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,  
0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e,
0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16,  
0x17, 0x18, 0x19, FAIL, FAIL, FAIL, FAIL, FAIL,

FAIL, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20,  
0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30,  
0x31, 0x32, 0x33, FAIL, FAIL, FAIL, FAIL, FAIL,

FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL,  
FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL,
FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL,  
FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL,
FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL,  
FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL,
FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL,  
FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL,
FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL,  
FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL,
FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL,  
FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL,
FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL,  
FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL,
FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL,  
FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL, FAIL
};


/* FIXME Possible buffer overflow on outputs larger than B64_BUF */
unsigned char* decode_base64(char* src,size_t ssize, size_t *ret_len)
{
  unsigned char* outbuf;
  unsigned char* retbuf;
  char* inb;
  int i;
  int l;
  int left;
  int pos;
  unsigned long triple;

  //error(235, "decode base64\n");
  /* Exit on empty input */
  if (!ssize||src==NULL)
    return NULL;

  
  /* Initialize working pointers */
  inb = src;
  outbuf = (unsigned char *)malloc(sizeof(unsigned char)*B64_BUF);

  l = 0;
  triple = 0;
  pos=0;
  left = ssize;
  /*
   * Process entire inbuf.
   */
  while (left != 0)
    {
      left--;
      i = fromb64[(unsigned char)*inb];
      switch(i)
	{
	case FAIL:
	  error(3, "decode_base64: Illegal character: %c\n", *inb);
	  error(230, "decode_base64: Illegal line:\n%s\n", src);
	  free(outbuf);
	  return NULL;
	  break;
	case SKIP:
	  break;
	default:
	  triple = triple<<6 | (0x3f & i);
	  l++;
	  break;
	}
      if (l == 4 || left == 0)
	{
	  switch(l)
	    {
	    case 2:
	      triple = triple>>4;
	      break;
	    case 3:
	      triple = triple>>2;
	      break;
	    default:
	      break;
	    }
	  for (l  -= 2; l >= 0; l--)
	    {
	      outbuf[pos]=( 0xff & (triple>>(l*8)));
	      pos++;
	    }
	  triple = 0;
	  l = 0;
	}
      inb++;
    }
  
  retbuf=(unsigned char*)malloc(sizeof(unsigned char)*(pos+1));
  memcpy(retbuf,outbuf,pos);
  retbuf[pos]='\0';
  
  free(outbuf);

  //I HAD TO COMMENT THIS LINE OR THE PROGRAM WOULD CRASH
  //if (ret_len) *ret_len = pos;
  
  return retbuf;
}

size_t length_base64(char* src,size_t ssize)
{
  char* inb;
  int i;
  int l;
  int left;
  size_t pos;
  unsigned long triple;

  //error(235, "decode base64\n");
  /* Exit on empty input */
  if (!ssize||src==NULL)
    return 0;


	
  /* Initialize working pointers */
  inb = src;

  l = 0;
  triple = 0;
  pos=0;
  left = ssize;
  /*
   * Process entire inbuf.
   */
  while (left != 0)
    {
      left--;
      i = fromb64[(unsigned char)*inb];
      switch(i)
	{
	case FAIL:
	  error(3, "length_base64: Illegal character: %c\n", *inb);
	  error(230, "length_base64: Illegal line:\n%s\n", src);
	  return 0; 
	  break;
	case SKIP:
	  break;
	default:
	  triple = triple<<6 | (0x3f & i);
	  l++;
	  break;
	}
      if (l == 4 || left == 0)
	{
	  switch(l)
	    {
	    case 2:
	      triple = triple>>4;
	      break;
	    case 3:
	      triple = triple>>2;
	      break;
	    default:
	      break;
	    }
	  for (l  -= 2; l >= 0; l--)
	    {
	      pos++;
	    }
	  triple = 0;
	  l = 0;
	}
      inb++;
    }
  printf("length_base64 was succesful.\n");
  return pos;
}

void print(char *t) {
   if (*t == '\0')
      return;
   printf("%c", *t);
   print(++t);
}

int main(int argc, char const *argv[])
{

	printf("Starting program. \n");
	FILE *f = fopen("test.txt", "r");
	printf("Opening file. \n");
    if(f==NULL) {
    	printf("Error opening file. Closing program.\n");
    	return -1;
    }
    printf("Input file is not null. \n");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    rewind(f);
    
    unsigned char* buffer = (unsigned char*)malloc(sizeof(unsigned char) * fsize);
    
    size_t result = fread (buffer, 1, fsize, f);
    if (result != fsize) {
		printf("Reading error. \n");
		return 0;
    }

    printf("Input String: \n");
    print(buffer);
    printf("\n");

    size_t size = 0;
    size = length_base64(buffer, result);
    printf("Dimension: %d\n", size);
    
    unsigned char* final = decode_base64(buffer, result, size);

    printf("Converted string: \n");
    print(final);
    printf("\n");
    
    fclose(f);
    free(buffer);

    printf("Program execution terminated. \n");
	return 0;
}
