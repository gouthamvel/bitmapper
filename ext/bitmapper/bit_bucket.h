#ifndef BIT_BUCKET
#define BIT_BUCKET

/*
  this is the bitfield which takes 1 byte(8-bits) of space in memory
*/
typedef struct ByteBits{
  unsigned char  f0:1;
  unsigned char  f1:1;
  unsigned char  f2:1;
  unsigned char  f3:1;
  unsigned char  f4:1;
  unsigned char  f5:1;
  unsigned char  f6:1;
  unsigned char  f7:1;
} ByteBits;

/*
  wrapper for sequence of ByteBits number of Bytebits will be
  given_size(in bits)/8 + 1
*/
typedef struct BitBucket{
  ByteBits *bucket;
  unsigned long long int size;         /* size in bits as given */
} BitBucket;

void _bit_bucket_set_bit_to(BitBucket* ptr,unsigned long long int byte_part, int bit_part, unsigned val);
int bit_bucket_set_bit(BitBucket* ptr,unsigned long long int num);
int bit_bucket_clear_bit(BitBucket* ptr,unsigned long long int num);
unsigned bit_bucket_get_bit(BitBucket* ptr,unsigned long long int num);
void bit_bucket_reset(BitBucket *ptr);
BitBucket* bit_bucket_create(unsigned long long int size);
unsigned long long int bit_bucket_size(BitBucket *ptr);
void bit_bucket_string(BitBucket *ptr, unsigned char* raw);
int bit_bucket_load_byte(BitBucket* ptr,char *buf, unsigned long long int pos);
void bit_bucket_free(BitBucket *ptr);
#endif /* BIT_BUCKET */
