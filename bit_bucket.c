#include <stdio.h>
#include <stdlib.h>

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

int bit_bucket_set_bit(BitBucket* ptr,long long int num){
    if(num > ptr->size) return 1;
    long long int byte_part=num/8, bit_part=num%8;
    _bit_bucket_set_bit_to(ptr, byte_part, bit_part, 1);
    return 0;
}

int bit_bucket_clear_bit(BitBucket* ptr,long long int num){
    if(num > ptr->size) return 1;
    long long int byte_part=num/8, bit_part=num%8;
    _bit_bucket_set_bit_to(ptr, byte_part, bit_part, 0);
    return 0;
}

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

void bit_bucket_reset(BitBucket *ptr){
    long long int count = ptr->size/8+1, i;
    for(i=0;i<count;i++){
        ptr->bucket[i] = (ByteBits){0,0,0,0,0,0,0,0};
    }
}

BitBucket* bit_bucket_create(long long int size){
    long long int count = size/8 + 1;
    BitBucket *ptr = (BitBucket*)malloc(sizeof(BitBucket));
    ptr->size = size;
    ptr->bucket = (ByteBits*)malloc(count*sizeof(ByteBits));
    return ptr;
}
long long int bit_bucket_size(BitBucket *ptr){
    return ptr->size/8+1;
}
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
 */
void bit_bucket_free(BitBucket *ptr){
    free(ptr->bucket);
    free(ptr);
}
