#include <stdio.h>
#include <stdlib.h>

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
    long long int size;         /* size in bits as given */
} BitBucket;

void bit_bucket_free(BitBucket*);
BitBucket* bit_bucket_create(long long int);
int bit_bucket_set_bit(BitBucket*,long long int);
int bit_bucket_clear_bit(BitBucket*,long long int);
unsigned bit_bucket_get_bit(BitBucket*,long long int);
void bit_bucket_reset(BitBucket*);
unsigned char* bit_bucket_string(BitBucket*);
long long int bit_bucket_size(BitBucket*);
int bit_bucket_load_byte(BitBucket* ,char *, long long int );

void _bit_bucket_set_bit_to(BitBucket* ptr,long long int byte_part, int bit_part, unsigned val){
    val = (val == 0) ? 0 : 1;
    switch(bit_part){
    case 0: ptr->bucket[byte_part].f0 = val;break;
    case 1: ptr->bucket[byte_part].f1 = val;break;
    case 2: ptr->bucket[byte_part].f2 = val;break;
    case 3: ptr->bucket[byte_part].f3 = val;break;
    case 4: ptr->bucket[byte_part].f4 = val;break;
    case 5: ptr->bucket[byte_part].f5 = val;break;
    case 6: ptr->bucket[byte_part].f6 = val;break;
    case 7: ptr->bucket[byte_part].f7 = val;break;
    };
}

/*
  sets the of given bit location to 1
  @param BitBucket* pointer to the bit bucket to use, users
  responsablity to ensure pointer is valid
  @param num the bit location
*/
int bit_bucket_set_bit(BitBucket* ptr,long long int num){
    if(num > ptr->size) return 1;
    long long int byte_part=num/8, bit_part=num%8;
    _bit_bucket_set_bit_to(ptr, byte_part, bit_part, 1);
    return 0;
}

/*
  clears the state of given bit location to 0
  @param BitBucket* pointer to the bit bucket to use, users
  responsablity to ensure pointer is valid
  @param num the bit location
*/
int bit_bucket_clear_bit(BitBucket* ptr,long long int num){
    if(num > ptr->size) return 1;
    long long int byte_part=num/8, bit_part=num%8;
    _bit_bucket_set_bit_to(ptr, byte_part, bit_part, 0);
    return 0;
}

/*
  returns the current state of given bit location
  @param BitBucket* pointer to the bit bucket to use, users
  responsablity to ensure pointer is valid
  @param num the bit location
 */
unsigned bit_bucket_get_bit(BitBucket* ptr,long long int num){
    if(num > ptr->size) return 2;
    long long int byte_part=num/8, bit_part=num%8;
    switch(bit_part){
    case 0: return ptr->bucket[byte_part].f0;
    case 1: return ptr->bucket[byte_part].f1;
    case 2: return ptr->bucket[byte_part].f2;
    case 3: return ptr->bucket[byte_part].f3;
    case 4: return ptr->bucket[byte_part].f4;
    case 5: return ptr->bucket[byte_part].f5;
    case 6: return ptr->bucket[byte_part].f6;
    case 7: return ptr->bucket[byte_part].f7;
    };
}

/*
  Resets the given BitBucket*
  @param BitBucket* pointer to the bit bucket to use, users
  responsablity to ensure pointer is valid
 */
void bit_bucket_reset(BitBucket *ptr){
    long long int count = ptr->size/8+1, i;
    for(i=0;i<count;i++){
        ptr->bucket[i] = (ByteBits){0,0,0,0,0,0,0,0};
    }
}

/*
   Creates BitBucket with the given size and allocates the requried
   memory. A reset on all bits is done after creating the memory.
   @param long long size Number of bits you want to allocate.
 */
BitBucket* bit_bucket_create(long long int size){
    long long int count = size/8 + 1;
    BitBucket *ptr = (BitBucket*)malloc(sizeof(BitBucket));
    ptr->size = size;
    ptr->bucket = (ByteBits*)malloc(count*sizeof(ByteBits));
    bit_bucket_reset(ptr);
    return ptr;
}

/*
   gives the size of the BitBucket in bytes
   @param BitBucket* pointer to the bit bucket to use, users
   responsablity to ensure pointer is valid
 */
long long int bit_bucket_size(BitBucket *ptr){
    return ptr->size/8+1;
}

/*
  gives the string representation of BitBucket
  @param BitBucket* pointer to the bit bucket to use, users
  responsablity to ensure pointer is valid
 */
unsigned char* bit_bucket_string(BitBucket *ptr){
    if(ptr == NULL){
        printf("######### ERROR: ptr is returning null");
        return (unsigned char*)"null";
    }
    long long int count = ptr->size/8+1, i;
    unsigned char* raw = (unsigned char*)malloc(count);
    memcpy(raw,ptr->bucket, count);
    return raw;
}

/*
  Copies 1 Byte of given string to BitBucket* memory at given position
  @param BitBucket* pointer to the bit bucket to use, users
  responsablity to ensure pointer is valid
  @param char* pointer to location where the 1 Byte is located
  @pos long long int the offset byte location in BitBucket to be modified
 */
int bit_bucket_load_byte(BitBucket* ptr,char *buf, long long int pos){
    if(ptr == NULL){
        printf("####### ERROR: ptr is returning null");
        return 2;
    }
    long long int count = ptr->size/8+1, i;
    if(count < pos) return 2;
    memcpy(ptr->bucket+pos, buf, 1);
    return 1;
}

/*
  Frees all the memory allocated by bit_bucket_create
  Should be called before dereferencing BitBucket pointer
  returned by bit_bucket_create
  @param BitBucket* the pointer to be cleared
 */
void bit_bucket_free(BitBucket *ptr){
    free(ptr->bucket);
    free(ptr);
}
