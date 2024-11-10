#include <stdio.h>
#include <openssl/md5.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <omp.h>

#define HEX_DIGEST_LENGTH MD5_DIGEST_LENGTH * 2
#define PASSWD_LENGTH 5 
#define ASCII_COUNT 95
//#define SIMPLE_DEBUG
//#define SLEEP

typedef unsigned long long int uint_64;

int brute_passwd(char*);
int md5_hash(char*, int, char*);

int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    puts("usage: ./md5 input-file");
    return -1;
  }

  
  char *filename = argv[1];

  FILE* input = fopen(filename, "rb");

  if(!input)
  {
    fprintf(stderr, "cannot open filename: %s", filename);
    return -1;
  }

  fseek(input, 0L, SEEK_END);
  size_t size = ftell(input);
  fseek(input, 0L, SEEK_SET);

  char* buf = (char*)malloc(sizeof(char) * size);
  fread(buf, sizeof(char), size, input);

  if(size != HEX_DIGEST_LENGTH)
  {
    printf("%s%d%s\n", "Passed file has a string which is not ", HEX_DIGEST_LENGTH, " long. Consider creating file with just cat > input.txt to avoid adding a trailing \\n");
    return -1;
  }

  puts(buf);

  brute_passwd(buf);

  free(buf);
  }

int md5_hash(char* str, int length, char* md5string)
{
  unsigned char digest[MD5_DIGEST_LENGTH];

  MD5_CTX context;
  MD5_Init(&context);
  MD5_Update(&context, str, length);
  MD5_Final(digest, &context);

  for(int i = 0; i < MD5_DIGEST_LENGTH; ++i)
  {
    sprintf(&md5string[i * 2], "%02x", (unsigned int)digest[i]);
  }
  return 1;
}

int brute_passwd(char* hash)
{
  char passwd[PASSWD_LENGTH];
  
  for(int i = 0; i < PASSWD_LENGTH; ++i)
  {
    passwd[i] = '\0';
  }

  for(int i = 1; i <= PASSWD_LENGTH; ++i)
  {
    printf("%s%d\n", "Str length: ", i);
    uint_64 iters = pow(95, i);
    uint_64 *powers_of_i = (uint_64*)malloc(sizeof(uint_64) * i);
    for(int p = 0; p < i; ++p)
    {
      powers_of_i[p] = pow(95, p); 
    }
  
    for(uint_64 j = 0; j < iters; ++j)
    {
      for(int p = i - 1; p >= 0; --p)
      {
        if(!(j % powers_of_i[p]))
        {
          passwd[p] = (j / powers_of_i[p]) % 94 + 33;
          break;
        }
      }
      char md5string[HEX_DIGEST_LENGTH];
      md5_hash(passwd, i, md5string);
#ifdef SIMPLE_DEBUG
      printf("%s%s%s%s\n", "Passwd: ", passwd, " MD5:", md5string);
#endif
#ifdef SLEEP
      sleep(1);
#endif
      if(!strcmp(md5string, hash))
      {
        printf("%s%s\n", "Password bruted: ", passwd);
        return 1;
      }
    }
    free(powers_of_i);
  }
  return 0;
}
