#include <stdio.h>
#include <stdlib.h>

#include "helper.h"

/*
  Takes a string
  if buffer.len > 0
    * fills the buffer from string
    * shift's the string to left
    * write the buffer to file
  end
  * write the string to file with byte boundry
  * copy rest of string bits to buffer if present
 */
FileBuffer append_to_file(FILE* fp, FileBuffer buf){
  int shift, nxt_bits, final_bits;
  unsigned long long int str_byte_len, str_bytes_to_write, i;

  nxt_bits = buf.str_length % 8;
  /* no shift if string or buffer is zero */
  if(buf.buff_fill_count == 0 || buf.str_length == 0) shift = 0;
  else shift = (8 - buf.buff_fill_count);
  final_bits = (shift <= nxt_bits) ? nxt_bits - shift : 8 - (shift - nxt_bits);
  str_byte_len = buf.str_length / 8;
  str_bytes_to_write = str_byte_len - ((shift <= nxt_bits) ? 0 : 1);

  if(buf.buff_fill_count > 0){
    /* fills the buffer from string */
    shift_bits(&(buf.buffer), buf.string, shift);
    /* shift's the string to left */
    for(i=0;i< str_byte_len ; i++){
      shift_bits(&(buf.string[i]), &(buf.string[i+1]), shift);
    }
     /* write the buffer to file */
    fwrite(&(buf.buffer), 1, 1, fp);
  }
  /* write the string to file with byte boundry */
  if(str_bytes_to_write > 0) fwrite(buf.string, 1, str_bytes_to_write, fp);
  if(nxt_bits != 0){
    buf.buffer = buf.string[str_byte_len];
    buf.buff_fill_count = final_bits;
    printf("new buffer - %X - %d\n", buf.buffer, buf.buff_fill_count);
  }
  return buf;
}

/*
   copies 'num_bits' from src head to dest tail,
   and left shift src by 'num_bits'.
 */
int shift_bits(char* dest, char* src, int num_bits){
  char tmp;
  char mask = '\xFF';
  if(num_bits == 0) return 0;
  tmp = *src;
  *dest = ((*dest) & mask<<num_bits) | ((unsigned char)tmp)>>(8-num_bits);
  *src = (*src)<<num_bits;
  return 0;
}
