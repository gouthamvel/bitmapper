#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "bit_mapper_wrap.h"
/* #include "bit_bucket.c" */

/*
   Creates empty Bitmapper*. Without BitBucket pointers.
*/
Bitmapper* create_map(){
  Bitmapper* map;
  map =  (Bitmapper*)malloc(sizeof(Bitmapper));
  return map;
}

/*
  Buckets are not allocated space a this point. They will be created
  on the fly.
  @param int Number of digits from left(Most Significant Digits) to be used
  for indexing 10^(10-index) will be the size of the bucket in bits.
*/
void allocate_map(Bitmapper* map, int index_len){
  unsigned long long int size = pow(10, index_len),i;
  map->bkts = (BitBucket**)malloc(size*sizeof(BitBucket*));
  for(i=0;i<size; i++){
    map->bkts[i] = NULL;
  }
  map->bkt_size = pow(10, 10 - index_len);
  map->bkt_count = size;
  map->index_len = index_len;
  sprintf(map->fill_str, "%%llu%%0%illu\n",10-index_len);
}

/*
  Clears the memory allocated by create_map and create_bucket_for(if available)
  @param Bitmapper* the Bitmapper* object to clear, only the buckets in
  the bitmap obj and the buckets pointer will be freed
*/
void free_map(Bitmapper* map){
  unsigned long long int i;
  for(i=0;i<map->bkt_count; i++){
    if((map->bkts[i])!=NULL) bit_bucket_free(map->bkts[i]);
  }
  free(map->bkts);
  free(map);
}

/*
  Creates a Bitbucket with the given size and returns the pointer
  @param BitBucket* the pointer to the bucket
  @param unsigned long long int the size of bucket in bits
 */
BitBucket* create_bucket_for(BitBucket* bkt, unsigned long long int size){
  bkt = bit_bucket_create(size);
  return bkt;
}

/*
  Adds the given number to the bucket reperesented by the given index
  @param Bitmapper* the mapepr object to use
  @param unsigned long long int the offset to be added(if the offset is greater
  than bucket size it is rejected)
  @param unsigned long long int the index of the bucket that will hold the number
 */
int add_num_in_bkt(Bitmapper* map,unsigned long long int offset, unsigned long long int bkt_index){
  if((map->bkts[bkt_index]) == NULL)
    map->bkts[bkt_index] = (BitBucket*)create_bucket_for(map->bkts[bkt_index], map->bkt_size);
  if(bit_bucket_set_bit(map->bkts[bkt_index], offset) != 0){
    puts("out of range");
    return 1;/* ERROR out of range */
  }
  return 0;
}

/*
  Wrapper for add_num_in_bkt, splits the given number into index and
  bit location
  @param Bitmapper* the mapepr object to use
  @param unsigned long long int the number
 */
int add_num(Bitmapper* map,unsigned long long int full_num){
  unsigned long long int offset, bkt_index, div;
  div = pow(10, map->index_len);
  offset = full_num / div;
  bkt_index = full_num % div;
  return add_num_in_bkt(map, offset, bkt_index);
}

/*
  Removes the given number from the bucket reperesented by the given index
  @param Bitmapper* the mapepr object to use
  @param unsigned long long int the offset to be Removed(if the offset is greater
  than bucket size it is rejected)
  @param unsigned long long int the index of the bucket that would be hold the number
*/
int remove_num_in_bkt(Bitmapper* map,unsigned long long int offset, unsigned long long int bkt_index){
  if(map->bkts[bkt_index] == NULL)
    return 0;
  if(bit_bucket_clear_bit(map->bkts[bkt_index], offset) != 0){
    puts("out of range");
    return 1;/* ERROR out of range */
  }
  return 0;
}

/*
  Wrapper for remove_num_in_bkt, splits the given number into index and
  bit location
  @param Bitmapper* the mapepr object to use
  @param unsigned long long int the number
*/
int remove_num(Bitmapper* map,unsigned long long int full_num){
  unsigned long long int offset, bkt_index, div;
  div = pow(10, map->index_len);
  offset = full_num / div;
  bkt_index = full_num % div;
  return remove_num_in_bkt(map, offset, bkt_index);
}

/*
  returns the status of the given bit offset in the bucket reperesented by the given index
  @param Bitmapper* the mapepr object to use
  @param unsigned long long int the offset to be checked(if the number is greater
  than bucket size it is rejected)
  @param unsigned long long int the index of the bucket that would be hold the number
*/
int status_num_in_bkt(Bitmapper* map,unsigned long long int offset, unsigned long long int bkt_index){
  if(map->bkts[bkt_index] == NULL)
    return 0;
  return bit_bucket_get_bit(map->bkts[bkt_index], offset);
}

/*
  Wrapper for status_num_in_bkt, splits the given number into index and
  bit location
  @param Bitmapper* the mapepr object to use
  @param unsigned long long int the number
*/
int status_num(Bitmapper* map,unsigned long long int full_num){
  unsigned long long int offset, bkt_index, div;
  div = pow(10, 10-map->index_len);
  bkt_index = full_num / div;
  offset = full_num % div;
  return status_num_in_bkt(map, offset, bkt_index);
}

/*
  A wrapper method which take a file and loads all the numbers in that
  file into Bitmapper*. Works with DOS & unix files.
  @param Bitmapper* the mapepr object to use
  @param FILE* the file pointer to the opened file

  TODO: check for validity of number
 */
int add_numbers_in_file(Bitmapper* map,FILE *in){
  char scan_str[20];
  char msisdn[15], index[map->index_len], rest_num[10-map->index_len];
  sprintf(scan_str, "%%%is%%%is\n",map->index_len, 10-map->index_len);
  while(fgets(msisdn, 15, in)!=NULL){
    sscanf(msisdn,scan_str, index, rest_num);
    add_num_in_bkt(map, atoll(rest_num), atoll(index) );
  }
}

/*
  A wrapper method which take a file and removes all the numbers in that
  file from Bitmapper* if the number exist. Works with DOS & unix files.
  @param Bitmapper* the mapepr object to use
  @param FILE* the file pointer to the opened file

  TODO: check for validity of number
 */
int remove_numbers_in_file(Bitmapper* map,FILE *del){
  char scan_str[20];
  char msisdn[15], index[map->index_len], rest_num[10-map->index_len];
  sprintf(scan_str, "%%%is%%%is\n",map->index_len, 10-map->index_len);
  while(fgets(msisdn, 15, del)!=NULL){
    sscanf(msisdn,scan_str, index, rest_num);
    remove_num_in_bkt(map, atoll(rest_num), atoll(index) );
  }
}

/*
  Dumps the number in bucket(if available) into the given file
  @param Bitmapper* the mapepr object to use
  @param FILE* the file pointer to the opened file
  @param unsigned long long int index to the bucket to consider
 */
int dump_bucket_to_file(Bitmapper* map, FILE* fp, unsigned long long int bkt_index){
  unsigned long long int i;
  unsigned num;
  if(map->bkt_count<=bkt_index) return 1;

  if(map->bkts[bkt_index] == NULL)
    return 2;
  for(i=0;i<map->bkt_size;i++){
    num = bit_bucket_get_bit(map->bkts[bkt_index], i);
    if(num==1){
      fprintf(fp, map->fill_str, bkt_index, i);
    }
  }
  return 0;
}

/*
  Dumps all numbers set in Bitmapper* in into the given file
  @param Bitmapper* the mapepr object to use
  @param FILE* the file pointer to the opened file
  @param unsigned long long int the length of index
 */
int dump_all_to_file(Bitmapper* map, FILE* fp){
  unsigned long long int i;
  for(i=0;i<map->bkt_count;i++){
    dump_bucket_to_file(map, fp, i);
  }
  return 0;
}

/*
  Dumps the bucket's(if available) bit values into the given file
  @param Bitmapper* the mapepr object to use
  @param FILE* the file pointer to the opened file
  @param unsigned long long int index to the bucket to consider

  TODO: If the bucket doesn't exist, dump empty string equal to the
  size of the bucket
 */
int dump_bucket_str_to_file(Bitmapper* map, FILE* fp,unsigned long long int bkt_index){
  unsigned char* raw = (unsigned char*)malloc(map->bkt_size/8+1);
  BitBucket* tmp;
  if(map->bkts[bkt_index] == NULL)
    tmp = bit_bucket_create(map->bkt_size);
  else
    tmp = map->bkts[bkt_index];
  bit_bucket_string(tmp, raw);
  fwrite(raw, 1, bit_bucket_size(tmp), fp);

  if(map->bkts[bkt_index] == NULL)
    bit_bucket_free(tmp);
  free(raw);
  return 0;
}

/*
  Dumps all bucket's bit values into the given file
  @param Bitmapper* the mapepr object to use
  @param FILE* the file pointer to the opened file
  @param unsigned long long int the length of index
 */
int dump_all_str_to_file(Bitmapper* map, FILE* fp){
  unsigned long long int i;
  for(i=0;i<map->bkt_count;i++){
    dump_bucket_str_to_file(map, fp, i);
  }
  return 0;
}

/*
  Loads binary string from file to given bitmap bucket. Creates
  bucket if it doesn't exist
  @param Bitmapper* the mapepr object to use
  @param FILE* the file pointer to the opened file
  @param unsigned long long int the index of bucket to load
 */
int load_str_file_to_bucket(Bitmapper* map, FILE* fp,unsigned long long int bkt_index){
  unsigned long long int size,i;
  char buf[1];
  if(map->bkts[bkt_index] == NULL){
    puts("creating bucket");
    map->bkts[bkt_index] = (BitBucket*)create_bucket_for(map->bkts[bkt_index], map->bkt_size);
  }

  size = bit_bucket_size(map->bkts[bkt_index]);
  for(i=0;i<size;i++){
    fread(buf, 1, 1, fp);
    if(buf == NULL) break;
    bit_bucket_load_byte(map->bkts[bkt_index], buf, i);
  }
  return 0;
}

/*
  Loads binary string from file to given bitmap-> Creates
  bucket if it doesn't exist.
  * The load is one-one bit maping.
  * bits exceding Bitmapper* size(buckets count * size of each bucket
  in bits) will be ignored
  * the size of the bucket reperesented in file string must be equal
  to the size of bucket in Bitmapper*.
  @param Bitmapper* the mapepr object to use
  @param FILE* the file pointer to the opened file
  @param unsigned long long int the index of bucket to load
 */
int load_str_file(Bitmapper* map, FILE* fp){
  unsigned long long int size, i, bkt_no;
  char buf[1];
  for(bkt_no = 0;bkt_no < map->bkt_count; bkt_no++){
    if(map->bkts[bkt_no] == NULL){
      // TODO
      puts("creating bucket");
      map->bkts[bkt_no] = (BitBucket*)create_bucket_for(map->bkts[bkt_no], map->bkt_size);
    }
    size = bit_bucket_size(map->bkts[bkt_no]);
    for(i=0;i<size;i++){
      fread(buf, 1, 1, fp);
      if(buf == NULL) return 0;
      bit_bucket_load_byte(map->bkts[bkt_no], buf, i);
    }
    return 0;
  }
}

void test(){
  int index_len = 6;
  FILE *in, *out, *del, *str_out, *str_in;
  Bitmapper* map = create_map();
  allocate_map(map, index_len);
  in = fopen("/tmp/ncpr/out.txt","r");
  add_numbers_in_file(map, in);
  /* in = fopen("test/data/1-add.txt","r"); */
  /* del = fopen("test/data/1-del.txt","r"); */
  out = fopen("/tmp/ncpr/out2.txt","w");
  /* str_out = fopen("/tmp/ncpr/str_out.txt","w"); */
  /* add_numbers_in_file(map, in, index_len); */
  /* //   remove_numbers_in_file(map, del); */
  /* /\* dump_all_str_to_file( map, str_out); *\/ */
  /* dump_bucket_str_to_file(map, str_out,  934793); */
  /* fclose(str_out); */
  /* str_in = fopen("/tmp/ncpr/str_out.txt","r"); */
  /* load_str_file_to_bucket(map, str_in,  934793); */
  /* puts("dumping to file"); */
  dump_all_to_file(map, out);


  fclose(in);
  /* fclose(del); */
  fclose(out);
  /* fclose(str_in); */
  free_map(map);
}

void main(){
  test();
}
