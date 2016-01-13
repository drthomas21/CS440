#include <unistd.h>  
#include <stdio.h>   
int main ()   
{   
    pid_t fpid;  
    int count=0;  
    fpid=fork();   
    if (fpid < 0)   
        printf("error in fork!");   
    else if (fpid == 0) {  
        printf("i am the child process, my process id is %d\n",getpid());   
        printf("This is the son! \n");
        count++;  
    }  
    else {  
        printf("i am the parent process, my process id is %d\n",getpid());   
        printf("This is the father! \n");  
        count++;  
	wait(NULL);
    }  
    printf("Result: %d\n",count);  
    return 0;  
}  
