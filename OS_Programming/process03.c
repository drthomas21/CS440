#include <unistd.h>  
#include <stdio.h>   
int main ()   
{   
    pid_t fpid;  
    int i;
    for(i=0;i<3;i++){
    fpid=fork();   
    if (fpid < 0)   
        printf("error in fork!");   
    else if (fpid == 0)  
        printf("child process, my process id is %d\n",getpid());   
    
    else 
        printf("parent process, my process id is %d\n",getpid());   
	wait(NULL);
    }
    return 0;  
}  
