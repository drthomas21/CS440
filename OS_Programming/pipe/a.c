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
        char buf[] = "This is Robert";  
        //  
        write(fd_pipe[1], buf, strlen(buf));  
          
        _exit(0);  
    }else if( pid > 0){//  
        wait(NULL);  
          
        char str[50] = {0};  
          
        //
        read(fd_pipe[0], str, sizeof(str));  
          
        printf("str=[%s]\n", str); 
    }  
      
    return 0;  
}  