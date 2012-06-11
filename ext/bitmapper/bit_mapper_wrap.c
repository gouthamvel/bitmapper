#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "bit_mapper_wrap.h"

/*
  Initializes the Bitmapper. Buckets are not allocated space a this point.
  They will be created on the fly.
  @param int Number of digits from left(Most Significant Digits) to be used
  for indexing 10^(10-index) will be the size of the bucket in bits.
*/
Bitmapper create_map(int index_len){
  unsigned long long int size = pow(10, index_len),i;
  Bitmapper smap;
  smap.bkts = (BitBucket**)malloc(size*sizeof(BitBucket*));
  for(i=0;i<size; i++){
    smap.bkts[i] = NULL;
  }
  smap.bkt_size = pow(10, 10 - index_len);
  smap.bkt_count = size;
  sprintf(smap.fill_str, "%%llu%%0%illu\n",10-index_len);
  return smap;
}

/*
  Clears the memory allocated by create_map and create_bucket_for(if available)
  @param Bitmapper the Bitmapper object to clear, only the buckets in
  the bitmap obj and the buckets pointer will be freed
*/
void free_map(Bitmapper smap){
  unsigned long long int i;
  for(i=0;i<smap.bkt_count; i++){
    if(smap.bkts[i]!=NULL) bit_bucket_free(smap.bkts[i]);
  }
  free(smap.bkts);
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
  @param Bitmapper the mapepr object to use
  @param unsigned long long int the number to be added(if the number is greater
  than bucket size it is rejected)
  @param unsigned long long int the index of the bucket that will hold the number
 */
int add_number(Bitmapper smap,unsigned long long int num, unsigned long long int index){
  if(smap.bkts[index] == NULL)
    smap.bkts[index] = (BitBucket*)create_bucket_for(smap.bkts[index], smap.bkt_size);
  if(bit_bucket_set_bit(smap.bkts[index], num) != 0){
    puts("out of range");
    return 1;/* ERROR out of range */
  }
  return 0;
}

 /*
   Removes the given number from the bucket reperesented by the given index
   @param Bitmapper the mapepr object to use
   @param unsigned long long int the number to be Removed(if the number is greater
   than bucket size it is rejected)
   @param unsigned long long int the index of the bucket that would be hold the number
  */
 int remove_number(Bitmapper smap,unsigned long long int num, unsigned long long int index){
   if(smap.bkts[index] == NULL)
     return 0;
   if(bit_bucket_clear_bit(smap.bkts[index], num) != 0){
     puts("out of range");
     return 1;/* ERROR out of range */
   }
   return 0;
 }

/*
  A wrapper method which take a file and loads all the numbers in that
  file into Bitmapper. Works with DOS & unix files.
  @param Bitmapper the mapepr object to use
  @param FILE* the file pointer to the opened file
  @param unsigned long long int the length of index

  TODO: check for validity of number
 */
int add_numbers_in_file(Bitmapper map,FILE *in, unsigned long long int index_len){
  char scan_str[20];
  char msisdn[15], index[index_len], rest_num[10-index_len];
  sprintf(scan_str, "%%%is%%%is\n",index_len, 10-index_len);
  while(fgets(msisdn, 15, in)!=NULL){
    sscanf(msisdn,scan_str, index, rest_num);
    add_number(map, atoll(rest_num), atoll(index) );
  }
}

/*
  A wrapper method which take a file and removes all the numbers in that
  file from Bitmapper if the number exist. Works with DOS & unix files.
  @param Bitmapper the mapepr object to use
  @param FILE* the file pointer to the opened file
  @param unsigned long long int the length of index

  TODO: check for validity of number
 */
int remove_numbers_in_file(Bitmapper map,FILE *del, unsigned long long int index_len){
  char scan_str[20];
  char msisdn[15], index[index_len], rest_num[10-index_len];
  sprintf(scan_str, "%%%is%%%is\n",index_len, 10-index_len);
  while(fgets(msisdn, 15, del)!=NULL){
    sscanf(msisdn,scan_str, index, rest_num);
    remove_number(map, atoll(rest_num), atoll(index) );
  }
}

/*
  Dumps the number in bucket(if available) into the given file
  @param Bitmapper the mapepr object to use
  @param FILE* the file pointer to the opened file
  @param unsigned long long int index to the bucket to consider
 */
int dump_bucket_to_file(Bitmapper map, FILE* fp, unsigned long long int bkt_index){
  unsigned long long int i;
  unsigned num;
  if(map.bkt_count<=bkt_index) return 1;

  if(map.bkts[bkt_index] == NULL)
    return 2;
  for(i=0;i<map.bkt_size;i++){
    num = bit_bucket_get_bit(map.bkts[bkt_index], i);
    if(num==1){
      fprintf(fp, map.fill_str, bkt_index, i);
    }
  }
  return 0;
}

/*
  Dumps all numbers set in Bitmapper in into the given file
  @param Bitmapper the mapepr object to use
  @param FILE* the file pointer to the opened file
  @param unsigned long long int the length of index
 */
int dump_all_to_file(Bitmapper map, FILE* fp){
  unsigned long long int i;
  for(i=0;i<map.bkt_count;i++){
    dump_bucket_to_file(map, fp, i);
  }
  return 0;
}

/*
  Dumps the bucket's(if available) bit values into the given file
  @param Bitmapper the mapepr object to use
  @param FILE* the file pointer to the opened file
  @param unsigned long long int index to the bucket to consider

  TODO: If the bucket doesn't exist, dump empty string equal to the
  size of the bucket
 */
int dump_bucket_str_to_file(Bitmapper map, FILE* fp,unsigned long long int bkt_index){
  unsigned char* raw = (unsigned char*)malloc(map.bkt_size/8+1);
  BitBucket* tmp;
  if(map.bkts[bkt_index] == NULL)
    tmp = bit_bucket_create(map.bkt_size);
  else
    tmp = map.bkts[bkt_index];
  bit_bucket_string(tmp, raw);
  fwrite(raw, 1, bit_bucket_size(tmp), fp);

  if(map.bkts[bkt_index] == NULL)
    bit_bucket_free(tmp);
  free(raw);
  return 0;
}

/*
  Dumps all bucket's bit values into the given file
  @param Bitmapper the mapepr object to use
  @param FILE* the file pointer to the opened file
  @param unsigned long long int the length of index
 */
int dump_all_str_to_file(Bitmapper map, FILE* fp){
  unsigned long long int i;
  for(i=0;i<map.bkt_count;i++){
    dump_bucket_str_to_file(map, fp, i);
  }
  return 0;
}

/*
  Loads binary string from file to given bitmap bucket. Creates
  bucket if it doesn't exist
  @param Bitmapper the mapepr object to use
  @param FILE* the file pointer to the opened file
  @param unsigned long long int the index of bucket to load
 */
int load_str_file_to_bucket(Bitmapper map, FILE* fp,unsigned long long int bkt_index){
  unsigned long long int size,i;
  char buf[1];
  if(map.bkts[bkt_index] == NULL){
    puts("creating bucket");
    map.bkts[bkt_index] = (BitBucket*)create_bucket_for(map.bkts[bkt_index], map.bkt_size);
  }

  size = bit_bucket_size(map.bkts[bkt_index]);
  for(i=0;i<size;i++){
    fread(buf, 1, 1, fp);
    if(buf == NULL) break;
    bit_bucket_load_byte(map.bkts[bkt_index], buf, i);
  }
  return 0;
}

/*
  Loads binary string from file to given bitmap. Creates
  bucket if it doesn't exist.
  * The load is one-one bit maping.
  * bits exceding Bitmapper size(buckets count * size of each bucket
  in bits) will be ignored
  * the size of the bucket reperesented in file string must be equal
  to the size of bucket in Bitmapper.
  @param Bitmapper the mapepr object to use
  @param FILE* the file pointer to the opened file
  @param unsigned long long int the index of bucket to load
 */
int load_str_file(Bitmapper map, FILE* fp){
  unsigned long long int size, i, bkt_no;
  char buf[1];
  for(bkt_no = 0;bkt_no < map.bkt_count; bkt_no++){
    if(map.bkts[bkt_no] == NULL){
      // TODO
      puts("creating bucket");
      map.bkts[bkt_no] = (BitBucket*)create_bucket_for(map.bkts[bkt_no], map.bkt_size);
    }
    size = bit_bucket_size(map.bkts[bkt_no]);
    for(i=0;i<size;i++){
      fread(buf, 1, 1, fp);
      if(buf == NULL) return 0;
      bit_bucket_load_byte(map.bkts[bkt_no], buf, i);
    }
    return 0;
  }
}

void test(){
  int index_len = 6;
  FILE *in, *out, *del, *str_out, *str_in;
  Bitmapper map = create_map(index_len);
  in = fopen("test/data/1-add.txt","r");
  del = fopen("test/data/1-del.txt","r");
  out = fopen("/tmp/ncpr/out.txt","w");
  str_out = fopen("/tmp/ncpr/str_out.txt","w");
  add_numbers_in_file(map, in, index_len);
  //   remove_numbers_in_file(map, del, index_len);
  /* dump_all_str_to_file( map, str_out); */
  dump_bucket_str_to_file(map, str_out,  934793);
  fclose(str_out);
  str_in = fopen("/tmp/ncpr/str_out.txt","r");
  load_str_file_to_bucket(map, str_in,  934793);
  puts("dumping to file");
  dump_all_to_file(map, out);

  fclose(in);
  fclose(del);
  fclose(out);
  fclose(str_in);
  free_map(map);
}

void main(){
  test();
}
