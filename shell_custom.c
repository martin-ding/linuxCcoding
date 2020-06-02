#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
extern char *optarg;
extern int optind, opterr, optopt;

int main(int argc, char *argv[]){
    int start = 1;
    int end = 10;
    int sum = 0, stop = 3;
    int opt, i;
    char *optstring = ":s:e::W;foo::";

    for(i=0; i< argc; i++){
        printf("argv %s\n", argv[i]);
    }



    //分析接收到的参数
    while((opt = getopt(argc, argv, optstring))!= -1){

        switch(opt){
            case 's': start = atoi(optarg); break;
            case 'e': end = atoi(optarg); break;
            case ':': puts("Missing parameter"); exit(1);
        }

        	 // printf("===%s\n", optarg);
        
    }
    
    //检测参数是否有效
    if(start<0 || end<=start){
        puts("Parameter error"); exit(2);
    }
   
    for(i=start; i<=end; i++){
    	if (i == stop) {break;}
        sum+=i;
    }
    printf("%d\n", sum);
    return 0;
}