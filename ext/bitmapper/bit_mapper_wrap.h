#include "bit_bucket.h"

#ifndef BIT_MAPPER_WRAP
#define BIT_MAPPER_WRAP

/*
  Bitmapper Object holds the buckets pointer and other info
*/
typedef struct Bitmapper{
  BitBucket** bkts;         /* pointer to list of pointers(count=bkt_count) */
  unsigned long long int bkt_count;  /* number of buckets in Bitmapper */
  unsigned long long int bkt_size;   /* size of each bucket in bits */
  /* the format specifier used to dump number to file from buckets */
  char fill_str[20];
} Bitmapper;

Bitmapper create_map(int);
void free_map(Bitmapper);
BitBucket* create_bucket_for(BitBucket*, unsigned long long int);
int add_number(Bitmapper ,unsigned long long int, unsigned long long int);
int remove_number(Bitmapper ,unsigned long long int, unsigned long long int);
int add_numbers_in_file(Bitmapper ,FILE*, unsigned long long int);
int remove_numbers_in_file(Bitmapper ,FILE*, unsigned long long int );
int dump_bucket_to_file(Bitmapper, FILE* , unsigned long long int);
int dump_all_to_file(Bitmapper , FILE* );
int dump_bucket_str_to_file(Bitmapper , FILE* ,unsigned long long int );
int dump_all_str_to_file(Bitmapper , FILE*);
int load_str_file_to_bucket(Bitmapper , FILE* ,unsigned long long int);
int load_str_file(Bitmapper , FILE*);

#endif  /* BIT_MAPPER_WRAP */
