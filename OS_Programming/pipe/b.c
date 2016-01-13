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
      
    if( pipe(fd_pipe) < 0 ){ 
        perror("pipe");  
    }  
      
    pid = fork(); 
    if( pid < 0 ){   
        perror("fork");  
        exit(-1);  
    }  
      
    if( pid == 0 ){  
          
        _exit(0);  
    }else if( pid > 0){  
      
        wait(NULL);  
          
        char str[50] = {0};  
          
        printf("before read\n");  
            
        read(fd_pipe[0], str, sizeof(str));  
          
        printf("after read\n");  
          
        printf("str=[%s]\n", str); 
    }  
      
    return 0;  
}  