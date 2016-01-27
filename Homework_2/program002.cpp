#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits>
#include <sys/types.h>
#include <sys/wait.h> 
#include <sys/mman.h>

const double AMOUNT = 10.00;
static double *account;

int main() {
	int clients = 0;
	account = static_cast<double*>(mmap(NULL, sizeof *account, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0));
	*account = 0;
	//std::mutex mtx;					// mutex for critical section
	//std::condition_variable cv;		// condition variable for critical section 
	
	while (clients == 0) {
		std::cout << "How many clients? " << std::endl;
		std::cin >> clients;
		if (std::cin.fail()) {
			std::cout << "Invalid value" << std::endl;
			clients = 0;
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<int>::max(), '\n');
		}
	}
	pid_t *pid = new pid_t[clients];
	for (int i = 0; i < clients; i++) {
		pid[i] = fork();
		
		if (pid[i] < 0) {
			std::cout << "Epic Error In Forking" << std::endl;
			return 1;
		} else if (pid[i] == 0) {
			*account += AMOUNT;
			return 0;
		}
	}

	for (int i = 0; i < clients; i++) {
		int status;
		while (waitpid(pid[i], &status, 0) == -1);
	}
	std::cout << "Savings Account Balance: $" << *account << std::endl;
	munmap(account, sizeof *account);
	delete pid;
	return 0;
}
