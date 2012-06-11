#include "ruby.h"
#include "bit_mapper_wrap.h"

VALUE rb_cBitmapper;

VALUE bm_init(VALUE self){
}

void Init_bitmapper(){
  rb_cBitmapper = rb_define_class("Bitmapper", rb_cObject);
  rb_define_method(rb_cBitmapper, "initialize", bm_init, 0);


}
