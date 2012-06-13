#include "ruby.h"
#include "bit_mapper_wrap.h"

static VALUE rb_cBitmapper;
static ID index_len;

VALUE bm_init(VALUE self, VALUE len){
  Bitmapper* map;
  Data_Get_Struct(self, Bitmapper, map);
  allocate_map(map, FIX2INT(len));
  rb_iv_set(self, "@index_len", len);
  return self;
}

/*
   TODO: check if void* to Bitmapper* is a safe cast
 */
void bm_free(void *p){
  free_map((Bitmapper*)p);
}

VALUE bm_alloc(VALUE self){
  Bitmapper* map;
  map = create_map();
  return Data_Wrap_Struct(self, 0, bm_free, map);
}

/*
   Generates current index and assign it to @indexes variable
 */
VALUE bm_referesh_indexes(VALUE self){
  Bitmapper* map;
  ID id_push;
  VALUE arr;
  FILE *fp;
  unsigned long long int i;
  arr = rb_ary_new();
  id_push = rb_intern("push");
  Data_Get_Struct(self, Bitmapper, map);

  rb_funcall(arr, rb_intern("clear"), 0);
  for(i=0;i<map->bkt_count;i++)
    if(map->bkts[i] != NULL)
      rb_funcall(arr, id_push, 1, LL2NUM(i));

  rb_iv_set(self, "@indexes", arr);
  return self;
}

/*
   Call to refresh indexs in @indexes
   and return the value
 */
VALUE bm_get_indexes(VALUE self){
  bm_referesh_indexes(self);
  return rb_iv_get(self, "@indexes");
}

VALUE bm_add_from_file(VALUE self, VALUE file_str){
  Bitmapper* map;
  VALUE index_len;
  FILE *fp;
  index_len = rb_iv_get(self, "@index_len");
  Data_Get_Struct(self, Bitmapper, map);
  fp = fopen(RSTRING_PTR(StringValue(file_str)), "r");
  add_numbers_in_file(map, fp, NUM2LL(index_len));
  fclose(fp);
  return self;
}

VALUE bm_remove_from_file(VALUE self, VALUE file_str){
  Bitmapper* map;
  VALUE index_len;
  FILE *fp;
  index_len = rb_iv_get(self, "@index_len");
  Data_Get_Struct(self, Bitmapper, map);
  fp = fopen(RSTRING_PTR(StringValue(file_str)), "r");
  remove_numbers_in_file(map, fp, NUM2LL(index_len));
  fclose(fp);
  return self;
}

VALUE bm_dump_to_file(VALUE self, VALUE file_str){
  Bitmapper* map;
  FILE *fp;
  Data_Get_Struct(self, Bitmapper, map);
  fp = fopen(RSTRING_PTR(StringValue(file_str)), "w");
  dump_all_to_file(map, fp);
  fclose(fp);
  return self;
}

VALUE bm_load_str_to_bkt(VALUE self, VALUE file_str, VALUE index){
  Bitmapper* map;
  FILE *fp;
  Data_Get_Struct(self, Bitmapper, map);
  fp = fopen(RSTRING_PTR(StringValue(file_str)), "r");
  load_str_file_to_bucket(map, fp, NUM2LL(index));
  fclose(fp);
  return self;
}

VALUE bm_dump_bkt_str(VALUE self, VALUE file_str, VALUE index){
  Bitmapper* map;
  FILE *fp;
  Data_Get_Struct(self, Bitmapper, map);
  fp = fopen(RSTRING_PTR(StringValue(file_str)), "w");
  dump_bucket_str_to_file(map, fp, NUM2LL(index));
  fclose(fp);
  return self;
}

/*
   returns true if the provided number is present in bitmap or false
 */
VALUE bm_num_status(VALUE self, VALUE num){

}

void Init_bitmapper(){
  rb_cBitmapper = rb_define_class("Bitmapper", rb_cObject);
  rb_define_alloc_func(rb_cBitmapper, bm_alloc);
  rb_define_method(rb_cBitmapper, "initialize", bm_init, 1);
  rb_define_method(rb_cBitmapper, "get_indexes", bm_get_indexes, 0);
  rb_define_method(rb_cBitmapper, "add_from_file", bm_add_from_file, 1);
  rb_define_method(rb_cBitmapper, "remove_from_file", bm_remove_from_file, 1);
  rb_define_method(rb_cBitmapper, "dump_to_file", bm_dump_to_file, 1);
  rb_define_method(rb_cBitmapper, "load_string_to_bucket", bm_load_str_to_bkt, 2);
  rb_define_method(rb_cBitmapper, "dump_bucket_string", bm_dump_bkt_str, 2);

  rb_define_method(rb_cBitmapper, "status?", bm_num_status, 1);
  /* rb_define_method(rb_cBitmapper, "set", bm_set, 1); */
  /* rb_define_method(rb_cBitmapper, "clear", bm_clear, 1); */

  /* rb_define_method(rb_cBitmapper, "foo", bm_foo, 0); */
  /* rb_define_method(rb_cBitmapper, "foo", bm_foo, 0); */
  /* rb_define_method(rb_cBitmapper, "foo", bm_foo, 0); */
}
