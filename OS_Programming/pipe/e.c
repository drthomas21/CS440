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
        //close(fd_pipe[0]);  
          
        _exit(0);  
    }else if( pid > 0 ){
      
        wait(NULL);   
          
        close(fd_pipe[0]); 
          
        char buf[50] = "12345";  
            
        // write() receives SIGPIPE  
        write(fd_pipe[1], buf, strlen(buf));  
          
        while(1);    
    }  
      
    return 0;  
}  