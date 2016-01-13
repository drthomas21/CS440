#include <unistd.h>  
#include <stdio.h>  

static int a = 100; /* Allocated in data segment */
int main(int argc, char *argv[])
{
    int b = 10; /* Allocated in stack segment */
    pid_t childPid;
    switch (childPid = fork()) {
    case -1:
        printf("error in fork!");  
    break;
    case 0:
        //idata *= 3;
        //istack *= 3;
        printf("PID=%ld %s", (long) getpid(), 
        (childPid == 0) ? "(child) " : "(parent)");
        printf("a*b=%d*%d=%d\n", a,b,a*b);
    break;
    default:
        sleep(3); /* Give child a chance to execute */
        printf("PID=%ld %s", (long) getpid(), 
        (childPid == 0) ? "(child) " : "(parent)");
        printf("a-b=%d-%d=%d\n", a,b,a-b);
    break;
    }
    /* Both parent and child come here */
    /*
    printf("PID=%ld %s idata=%d istack=%d\n", (long) getpid(), 
    	(childPid == 0) ? "(child) " : "(parent)", idata, istack);*/
    return 0;
}