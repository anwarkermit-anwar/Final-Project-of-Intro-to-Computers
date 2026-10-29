#include<stdio.h>
#include<stdlib.h>

int save(char* saving_file){
    FILE *file=fopen(saving_file,"w");
    if(file==NULL){
        printf("open file failed\n");
        return 1;
    }
    //
    //
    return 0;
}
int load(char* loading_file){

}
