#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../ext/bitmapper/bit_mapper_wrap.h"

#include "../ext/bitmapper/bit_mapper_wrap.c"
#include "../ext/bitmapper/bit_bucket.c"


void test(){
  int index_len = 6;
  FILE *in, *out, *del, *str_out, *str_in;
  Bitmapper* map = create_map();
  int *tmp;
  allocate_map(map, index_len);
  in = fopen("/tmp/ncpr/in.txt","r");
  add_numbers_in_file(map, in);
  /* in = fopen("test/data/1-add.txt","r"); */
  del = fopen("/tmp/ncpr/del.txt","r");
  out = fopen("/tmp/ncpr/out.txt","w");
  /* str_out = fopen("/tmp/ncpr/str_out.txt","w"); */
  /* add_numbers_in_file(map, in, index_len); */
  remove_numbers_in_file(map, del);
  /* dump_all_str_to_file( map, str_out); */
  /* dump_bucket_str_to_file(map, str_out,  934793); */
  /* fclose(str_out); */
  /* str_in = fopen("/tmp/ncpr/str_out.txt","r"); */
  /* load_str_file_to_bucket(map, str_in,  934793); */
  puts("dumping to file");
  /* dump_all_to_file(map, out); */


  fclose(in);
  fclose(del);
  fclose(out);
  /* fclose(str_in); */
  free_map(map);

  /* tmp = (int*)malloc(10000000*sizeof(int)); */
  /* free(tmp); */
  puts("DONE");
}

void main(){
  test();
}
