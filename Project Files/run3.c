#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <string.h>
#include <sys/time.h>

void systemcall_time(int pFile){   
    long begin, end;
    struct timeval timecheck;   
    unsigned int counter=0;
    float time;
    int x=1;
    gettimeofday(&timecheck, NULL);
    begin = (long)timecheck.tv_sec * 1000 + (long)timecheck.tv_usec / 1000;
    
    while (lseek(pFile,1,SEEK_SET)>0){     
      if (counter==x){
         x=2*x;
         gettimeofday(&timecheck, NULL);
         end = (long)timecheck.tv_sec * 1000 + (long)timecheck.tv_usec / 1000;
         time = (float)(end - begin)/1000;
         if (time >= 5){
            printf("Systemcall(lseek) speed in systemcalls/s: %.0f\n", (float)counter/time);
            close(pFile);
            exit(0);
         }
      }
      counter++;
   }
    gettimeofday(&timecheck, NULL);
    end = (long)timecheck.tv_sec * 1000 + (long)timecheck.tv_usec / 1000;
    time = (float)(end - begin)/1000;
    printf("Systemcall(lseek) speed in systemcalls/s: %.0f\n", (float)counter/time);
}

int main(int argc,char *argv[]){
    int block_size, file_size, pFile;
    unsigned int block_count ;
    pFile=open(argv[1],O_RDONLY);

    if(pFile < 0){
        printf("cannot open file %s.\n",argv[1]);
        exit(1);
    }
    else if(argc!=2){
        printf("Invalid number of arguments.\n");
        exit(1);
    }     
    else{
        systemcall_time(pFile);
        exit(0);
    }
    close(pFile);
    exit(0);
}    