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

VALUE bm_add_num(VALUE self, VALUE file_str){
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

VALUE bm_dump_num(VALUE self, VALUE file_str){
  Bitmapper* map;
  FILE *fp;
  Data_Get_Struct(self, Bitmapper, map);
  fp = fopen(RSTRING_PTR(StringValue(file_str)), "w");
  dump_all_to_file(map, fp);
  fclose(fp);
  return self;
}

void Init_bitmapper(){
  rb_cBitmapper = rb_define_class("Bitmapper", rb_cObject);
  rb_define_alloc_func(rb_cBitmapper, bm_alloc);
  rb_define_method(rb_cBitmapper, "initialize", bm_init, 1);
  rb_define_method(rb_cBitmapper, "add_numbers", bm_add_num, 1);
  rb_define_method(rb_cBitmapper, "dump_numbers", bm_dump_num, 1);
  /* rb_define_method(rb_cBitmapper, "remove_numbers", bm_remove_num, 1); */
  /* rb_define_method(rb_cBitmapper, "inject_string", bm_inj_str, 1); */
  /* rb_define_method(rb_cBitmapper, "dump_string", bm_dump_str, 1); */

  /* rb_define_method(rb_cBitmapper, "status?", bm_num_status, 1); */
  /* rb_define_method(rb_cBitmapper, "set", bm_set, 1); */
  /* rb_define_method(rb_cBitmapper, "clear", bm_clear, 1); */

  /* rb_define_method(rb_cBitmapper, "foo", bm_foo, 0); */
  /* rb_define_method(rb_cBitmapper, "foo", bm_foo, 0); */
  /* rb_define_method(rb_cBitmapper, "foo", bm_foo, 0); */
}
