#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <string.h>
#include <sys/time.h>

void read_file(int pFile,unsigned long long block_size){
      
   long begin, end;
   struct timeval timecheck;   
   size_t bytes_read,file_size=0;
   unsigned int result=0;
   float time;
   gettimeofday(&timecheck, NULL);
   begin = (long)timecheck.tv_sec * 1000 + (long)timecheck.tv_usec / 1000;

   while (1){ 
      unsigned int* buffer=NULL;
      buffer=(unsigned int*) malloc(sizeof(unsigned int)*block_size);
      bytes_read=read (pFile,buffer,block_size*sizeof(unsigned int));
      file_size+=bytes_read;
      if(bytes_read){
         for(unsigned int j=0;j<bytes_read/4;j++){
             result^=*(buffer+j);
         }
         free(buffer);
      }
      else{
         free(buffer);
         break;
      }      
   }
   gettimeofday(&timecheck, NULL);
   end = (long)timecheck.tv_sec * 1000 + (long)timecheck.tv_usec / 1000;
   time=(float)(end - begin)/1000;
   printf("XOR: %08x\n",result);
   printf("Block size: %llu\n",block_size);
   printf("Total execution time in seconds: %f\n", time);
   printf("Read speed in MiB/s: %.3f\n",(float)file_size/(1024*1024*time));
}

int main(int argc,char *argv[]){
   
   int pFile;
   pFile=open(argv[1],O_RDONLY);

    if(pFile < 0){
        printf("cannot open file %s.\n",argv[1]);
        exit(1);
    }
    unsigned long long block_size=1048576;     
    read_file(pFile,block_size);
    close(pFile);
    exit(0);
} 