#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <string.h>
#include <sys/time.h>

void resonable_file_size(int pFile, unsigned int block_size){   
    long begin, end;
    struct timeval timecheck;   
    unsigned int block_count=0;
    size_t file_size=0,bytes_read;
    float time;
    int x=1;
    gettimeofday(&timecheck, NULL);
    begin = (long)timecheck.tv_sec * 1000 + (long)timecheck.tv_usec / 1000;
    
    while (1){     
        unsigned int* buffer=(unsigned int*) malloc(sizeof(unsigned int)*block_size);
        bytes_read=read (pFile,buffer,block_size*sizeof(unsigned int));
        if(bytes_read<1){ 
            free(buffer);
            break;
        }
        else{
            file_size += bytes_read;
            free(buffer);
            if (block_count==x){
                x=2*x;
                gettimeofday(&timecheck, NULL);
                end = (long)timecheck.tv_sec * 1000 + (long)timecheck.tv_usec / 1000;
                time = (float)(end - begin)/1000;
                if (time >= 5){
                    file_size=(unsigned long long int)block_size*block_count*4;
                    printf("Block size: %u\nBlock count: %u\nFile size in bytes: %zu\n", block_size, block_count,file_size);
                    printf("Total execution time in seconds: %f\n", time );
                    printf("Read speed in B/s: %.0f\n",(float)file_size/(time));
                    printf("Read speed in MiB/s: %.3f\n",(float)file_size/(1024*1024*time));
                    
                    close(pFile);
                    exit(0);
                }
            }
        }
        block_count++;
    }
    gettimeofday(&timecheck, NULL);
    end = (long)timecheck.tv_sec * 1000 + (long)timecheck.tv_usec / 1000;
    time = (float)(end - begin)/1000;
    file_size=(unsigned long long int)block_size*block_count*4;
    printf("Block size: %u\nBlock count: %u\nFile size in bytes: %zu\n", block_size, block_count,file_size);
    printf("Total execution time in seconds: %f\n", time );
    printf("Read speed in B/s: %.0f\n",(float)file_size/(time));
    printf("Read speed in MiB/s: %.3f\n",(float)file_size/(1024*1024*time));
}

int main(int argc,char *argv[]){
    int block_size, file_size, pFile;
    unsigned int block_count ;
    pFile=open(argv[1],O_RDONLY);

    if(pFile < 0){
        printf("cannot open file %s.\n",argv[1]);
        exit(1);
    }
    else if(argc!=3){
        printf("Invalid number of arguments.\n");
        exit(1);
    }     
    else{
        block_size=atoi(argv[2]);
        resonable_file_size(pFile, block_size);
        exit(0);
    }
    close(pFile);
    exit(0);
}    