#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <string.h>
#include <sys/time.h>
#include <arpa/inet.h>

void read_file(int pFile,unsigned int block_count,unsigned int block_size){
      
   long begin, end;
   struct timeval timecheck;   
   size_t file_size=0,bytes_read;

   float time;
   gettimeofday(&timecheck, NULL);
   begin = (long)timecheck.tv_sec * 1000 + (long)timecheck.tv_usec / 1000;

   for (int i=0 ; i < block_count ; i++){ 
      unsigned int* buffer=(unsigned int*) malloc(sizeof(unsigned int)*block_size);
      bytes_read=read (pFile,buffer,block_size*sizeof(unsigned int));
      if(bytes_read){
         file_size += bytes_read;
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

   printf("File size in bytes: %zu\n",file_size);
   printf("Total execution time in seconds: %f\n", time);
}


void write_file(int pFile,unsigned int block_size,unsigned int block_count){
      
   long begin, end;
   struct timeval timecheck;   
   float time;
   unsigned int* buffer=(unsigned int*) malloc(sizeof(unsigned int)*block_size);
   for (unsigned int i=0 ; i< block_size ; i++){
      *(buffer+i)=(i%255);
   }
   
   gettimeofday(&timecheck, NULL);
   begin = (long)timecheck.tv_sec * 1000 + (long)timecheck.tv_usec / 1000;
   
   for (unsigned int i=0 ; i < block_count ; i++){ 
      write (pFile,buffer,block_size*sizeof(unsigned int));
   }
   gettimeofday(&timecheck, NULL);
   end = (long)timecheck.tv_sec * 1000 + (long)timecheck.tv_usec / 1000;
   time=(float)(end - begin)/1000;

   printf("Total bytes written in the file: %llu\n",(unsigned long long)block_count*block_size*4);
   printf("Total execution time in seconds: %f\n", time);
}



int main(int argc,char *argv[]){
   int block_size, pFile;
   unsigned int block_count ;
   unsigned int file_size;
      if(strcmp(argv[2],"-r")==0){
      
      pFile=open(argv[1],O_RDONLY);
      if(pFile < 0){
         printf("cannot open file %s.\n",argv[1]);
         exit(1);
      }

      block_size=atoi(argv[3]);   
      block_count=atoi(argv[4]);
      
      if(argc != 5){
         printf("Invalid number of arguments.\n");
         exit(1);
      }

      read_file(pFile, block_count, block_size);
      close(pFile);
   }  
   else if (strcmp(argv[2],"-w")==0){

      pFile= open(argv[1], O_WRONLY|O_CREAT|O_APPEND, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH); 
      if(pFile < 0){
         printf("cannot open file %s.\n",argv[1]);
         exit(1);
      }
      
      if(argc != 5){
         printf("Invalid number of arguments.\n");
         exit(1);
      }
      block_size=atoi(argv[3]);   
      block_count=atoi(argv[4]);
      write_file(pFile,block_size,block_count);
   }
   else{
      printf("Invalid parameter.\n");
      exit(1);
   }
   exit(0);
}    