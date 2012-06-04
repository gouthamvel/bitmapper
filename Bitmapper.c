#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "bit_bucket.c"

typedef struct Bitmapper{
    BitBucket** bkts;
    long long int bkt_count;
    long long int bkt_size;
    char fill_str[20];
} Bitmapper;

Bitmapper create_map(int index_count){
    long long int size = pow(10, index_count),i;
    Bitmapper smap;
    smap.bkts = (BitBucket**)malloc(size*sizeof(BitBucket*));
    for(i=0;i<size; i++){
        smap.bkts[i] = NULL;
    }
    smap.bkt_size = pow(10, 10 - index_count);
    smap.bkt_count = size;
    sprintf(smap.fill_str, "%%llu%%%illu\n",10-index_count);

    return smap;
}

void free_map(Bitmapper smap){
    long long int i;
    for(i=0;i<smap.bkt_count; i++){
        if(smap.bkts[i]!=NULL) bit_bucket_free(smap.bkts[i]);
    }
    free(smap.bkts);
}

BitBucket* create_bucket_for(BitBucket* bkt, long long int size){
    bkt = bit_bucket_create(size);
    bit_bucket_reset(bkt);
    return bkt;
}

int add_number(Bitmapper smap,long long int num, long long int index){

    if(smap.bkts[index] == NULL){
        printf("adding num %llu:%llu", index, num);puts("");
        smap.bkts[index] = (BitBucket*)create_bucket_for(smap.bkts[index], smap.bkt_size);
    }

    if(bit_bucket_set_bit(smap.bkts[index], num) != 0){
        puts("out of range");
        return 1;/* ERROR out of range */
    }
    return 0;
}
int remove_number(Bitmapper smap,long long int num, long long int index){
    if(smap.bkts[index] == NULL)
        return 0;
    if(bit_bucket_clear_bit(smap.bkts[index], num) != 0){
        puts("out of range");
        return 1;/* ERROR out of range */
    }
    return 0;
}

int add_numbers_in_file(Bitmapper map,FILE *in, long long int index_count){
    long long int num,i=0;
    char scan_str[20];
    char msisdn[15], index[index_count], rest_num[10-index_count];
    sprintf(scan_str, "%%%is%%%is\n",index_count, 10-index_count);
    while(fgets(msisdn, 15, in)!=NULL){
        sscanf(msisdn,scan_str, index, rest_num);
        add_number(map, atoll(rest_num), atoll(index) );
        i++;
    }
}
int remove_numbers_in_file(){

}
int dump_bucket_to_file(Bitmapper map, long long int bkt_index, FILE* fp){

    if(map.bkt_count<=bkt_index) return 1;
    long long int i;
    unsigned num;

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

int dump_all_to_file(Bitmapper map, FILE* fp){
    long long int i;
    for(i=0;i<map.bkt_count;i++){
        dump_bucket_to_file(map, i, fp);
    }
    return 0;
}

void init(){
    int index_count = 5;
    FILE *in, *out;

    in = fopen("test/data/1-add.txt","r");
    out = fopen("/tmp/ncpr/out.txt","w");
    Bitmapper map = create_map(index_count);
    add_numbers_in_file(map, in, index_count);
    dump_all_to_file(map, out);

    fclose(in);
    fclose(out);
    free_map(map);
}

void main(){
    init();
}
