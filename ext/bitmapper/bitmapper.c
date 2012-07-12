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
  if(p != NULL) free(p);
}

/*
   Frees memory. The amount of memory returned to OS depends on
   compiler trip settings.
 */
VALUE bm_reset(VALUE self){
  Bitmapper* map;
  VALUE len;
  Data_Get_Struct(self, Bitmapper, map);
  free_map(map);
  len = rb_iv_get(self, "@index_len");
  allocate_map(map, FIX2INT(len));
  return self;
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
  FILE *fp;
  Data_Get_Struct(self, Bitmapper, map);
  fp = fopen(RSTRING_PTR(StringValue(file_str)), "r");
  if(fp == NULL) rb_raise(rb_eIOError, "file counld not be opened");
  add_numbers_in_file(map, fp);
  fclose(fp);
  return self;
}

VALUE bm_remove_from_file(VALUE self, VALUE file_str){
  Bitmapper* map;
  FILE *fp;
  Data_Get_Struct(self, Bitmapper, map);
  fp = fopen(RSTRING_PTR(StringValue(file_str)), "r");
  if(fp == NULL) rb_raise(rb_eIOError, "file counld not be opened");
  remove_numbers_in_file(map, fp);
  fclose(fp);
  return self;
}

VALUE bm_dump_to_file(VALUE self, VALUE file_str){
  Bitmapper* map;
  FILE *fp;
  Data_Get_Struct(self, Bitmapper, map);
  fp = fopen(RSTRING_PTR(StringValue(file_str)), "w");
  if(fp == NULL) rb_raise(rb_eIOError, "file counld not be opened");
  dump_all_to_file(map, fp);
  fclose(fp);
  return self;
}

VALUE bm_load_str_to_bkt(VALUE self, VALUE file_str, VALUE index){
  Bitmapper* map;
  FILE *fp;
  Data_Get_Struct(self, Bitmapper, map);
  fp = fopen(RSTRING_PTR(StringValue(file_str)), "r");
  if(fp == NULL) rb_raise(rb_eIOError, "file counld not be opened");
  load_str_file_to_bucket(map, fp, NUM2LL(index));
  fclose(fp);
  return self;
}

VALUE bm_dump_bkt_str(VALUE self, VALUE file_str, VALUE index){
  Bitmapper* map;
  FILE *fp;
  Data_Get_Struct(self, Bitmapper, map);
  fp = fopen(RSTRING_PTR(StringValue(file_str)), "w");
  if(fp == NULL) rb_raise(rb_eIOError, "file counld not be opened");
  dump_bucket_str_to_file(map, fp, NUM2LL(index));
  fclose(fp);
  return self;
}

/*
   helper method, works as to_i for various datatypes
 */
unsigned long long int bm_to_ll(VALUE num){
  VALUE tmp;
  switch(TYPE(num)){
  case T_STRING:
    tmp = rb_funcall(num, rb_intern("to_i"), 0); /* hack */
    return NUM2LL(tmp);
  case T_FIXNUM:
  case T_BIGNUM:
    return NUM2LL(num);
  }
  return 0;
}

/*
  adds the given number to bitmapper
*/
VALUE bm_set(VALUE self, VALUE num){
  int status;
  Bitmapper* map;

  Data_Get_Struct(self, Bitmapper, map);
  status = add_num(map, bm_to_ll(num));
  if(status==0)
    return (VALUE)Qtrue;
  else
    rb_raise(rb_eTypeError, "not valid number");
}

/*
  removes the given number from bitmapper
*/
VALUE bm_clear(VALUE self, VALUE num){
  int status;
  Bitmapper* map;

  Data_Get_Struct(self, Bitmapper, map);
  status = remove_num(map, bm_to_ll(num));
  if(status==0)
    return (VALUE)Qtrue;
  else
    rb_raise(rb_eTypeError, "not valid number");
}

/*
  returns true if the provided number is present in bitmap or returns
  false
*/
VALUE bm_num_status(VALUE self, VALUE num){
  int status;
  VALUE return_val;
  Bitmapper* map;

  Data_Get_Struct(self, Bitmapper, map);
  status = status_num(map, bm_to_ll(num));
  if(status==1)
    return (VALUE)Qtrue;
  else if(status == 0)
    return (VALUE)Qfalse;
  else
    rb_raise(rb_eTypeError, "not valid number");
  return (VALUE)Qfalse;
}


VALUE bm_set_filter(VALUE self, VALUE vCh){
  int status;
  char ch;
  Bitmapper* map;

  Data_Get_Struct(self, Bitmapper, map);
  vCh = rb_funcall(vCh, rb_intern("to_s"), 0); /* hack */
  ch = NUM2CHR(vCh);
  set_white_list_char(map, ch);
  return (VALUE)Qtrue;
}

VALUE bm_clear_filter(VALUE self){
  int status;
  VALUE return_val;
  Bitmapper* map;

  Data_Get_Struct(self, Bitmapper, map);
  clear_white_list_char(map);
  return (VALUE)Qtrue;
}

void Init_bitmapper(){
  rb_cBitmapper = rb_define_class("Bitmapper", rb_cObject);
  rb_define_alloc_func(rb_cBitmapper, bm_alloc);
  rb_define_method(rb_cBitmapper, "initialize", bm_init, 1);
  rb_define_method(rb_cBitmapper, "reset", bm_reset, 0);
  rb_define_method(rb_cBitmapper, "get_indexes", bm_get_indexes, 0);
  rb_define_method(rb_cBitmapper, "add", bm_add_from_file, 1);
  rb_define_method(rb_cBitmapper, "remove", bm_remove_from_file, 1);
  rb_define_method(rb_cBitmapper, "dump_to", bm_dump_to_file, 1);
  rb_define_method(rb_cBitmapper, "set_filter", bm_set_filter, 1);
  rb_define_method(rb_cBitmapper, "clear_filter", bm_clear_filter, 0);
  rb_define_method(rb_cBitmapper, "load_from_str", bm_load_str_to_bkt, 2);
  rb_define_method(rb_cBitmapper, "dump_to_str", bm_dump_bkt_str, 2);

  rb_define_method(rb_cBitmapper, "status?", bm_num_status, 1);
  rb_define_method(rb_cBitmapper, "set", bm_set, 1);
  rb_define_method(rb_cBitmapper, "clear", bm_clear, 1);
}
