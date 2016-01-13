#include <stdio.h>  
#include <string.h>  
#include <unistd.h>  
#include <stdlib.h>  
#include <sys/types.h>  
#include <sys/wait.h>  
  
int main(int argc, char *argv[])  
{  
    int fd_pipe[2] = {0};  
    pid_t pid;  
      
    char buf[1024] = {0};  
    memset(buf, 'a', sizeof(buf)); 
    int i = 0;  
      
    if( pipe(fd_pipe) < 0 ){
        perror("pipe");  
    }  
      
    pid = fork();  
    if( pid < 0 ){  
        perror("fork");  
        exit(-1);  
    }  
      
    if( pid == 0 ){ 
        while(1){  
            write(fd_pipe[1], buf, sizeof(buf));  
            i++;  
            printf("i ======== %d\n", i);  
        }  
          
        _exit(0);  
    }else if( pid > 0){
      
        wait(NULL);  
    }  
      
    return 0;  
}  