#ifndef BIT_WRAP_HELPER
#define BIT_WRAP_HELPER

typedef struct FileBuffer{
  unsigned char buffer;
  int buff_fill_count;
  unsigned char* string;
  unsigned long long int str_length; /* number of bits */
}FileBuffer;

FileBuffer append_to_file(FILE*, FileBuffer);
int shift_bits(char* , char* , int);

#endif /* BIT_WRAP_HELPER */
