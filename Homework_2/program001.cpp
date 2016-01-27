#include <iostream>
#include <time.h>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h> 
#include <cstring>
#include <sys/types.h>
#include <sys/wait.h> 

enum PIPING{ READING_END = 0, WRITING_END = 1,BUFFER_SIZE=2};
int factorial(int);	
int main() {
	srand(static_cast<unsigned int>(time(NULL)));

	int _pipe[2];
	if (pipe(_pipe) < 0) {
		std::cout << "Epic Error In Piping" << std::endl;
		return 1;
	}

	pid_t pid = fork();		
	if(pid < 0) {
		std::cout << "Epic Error In Forking" << std::endl;
		return 1;
	} else if(pid == 0) {
		//Child
		close(_pipe[READING_END]);
		int n = (rand() % 10) + 1;
		char const *message = (std::to_string(n)).c_str();

		std::cout << "Writing " << message << " to pipe" << std::endl;
		write(_pipe[WRITING_END], message, BUFFER_SIZE);

		close(_pipe[WRITING_END]);
		return 0;
	} else {
		//Parent
		close(_pipe[WRITING_END]);
		wait(NULL);

		int n;
		char message[BUFFER_SIZE];
		read(_pipe[READING_END], message, BUFFER_SIZE);
		n = atoi(message);

		close(_pipe[WRITING_END]);
		std::cout << "Recieved " << message << " from child. Factorial = " << factorial(n) << std::endl;
	}		

	return 0;
}

int factorial(int n) {
	if(n < 1) {
		return 0;
	} else if(n == 1) {
		return 1;
	} else {
		return n * factorial(n-1);
	}
}
