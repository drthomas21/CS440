#include <unistd.h>  
#include <stdio.h>  

int main() {  

    pid_t fpid;

    fpid = fork();  
    if (fpid < 0)  
        printf("error in fork!");  
    else if (fpid == 0)  
        printf("1: I am the child process, my process id is %d\n", getpid());  
    else { 
        printf("1: I am the parent process, my process id is %d\n", getpid());  
	wait(NULL);
    }

    fpid = fork();  
    if (fpid < 0)  
        printf("error in fork!");  
    else if (fpid == 0)  
        printf("2: I am the child process, my process id is %d\n", getpid());  
    else  {
        printf("2: I am the parent process, my process id is %d\n", getpid());   
	wait(NULL);
    }

    return 0;  
}  
