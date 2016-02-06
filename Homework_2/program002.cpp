#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h> 
#include <sys/mman.h>

const double AMOUNT = 10.00;

class Account {
	private:
		double savings = 0;
		double checking = 0;
public:
	Account(): savings(0), checking(0) { };
	Account(double savings, double checking) :savings(savings), checking(checking) { }
	bool transferFunds(double amount) {
		if (checking >= amount) {
			checking -= amount;
			savings += amount;
			return true;
		}

		return false;
	}
	void showAccounts() {
		std::cout << "\n\tSavings: $" << savings
			<< "\n\tChecking: $" << checking
			<< std::endl;
	}
};

int main() {
	srand(static_cast<int>(time(NULL)));
	int clients = 0;
	
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

	Account account;
	Account *pAccounts = &account;
	

	pAccounts = static_cast<Account*>(mmap(NULL, (sizeof *pAccounts) * clients, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0));
	Account *_pAccounts = pAccounts;
	for (int i = 0; i < clients; i++) {
		pAccount = _pAccounts + sizeOf(account) * i;
		*pAccounts = new Account(rand() % 100, rand() % 100 + 10);
		std::cout << "Client " << (i + 1) << ": ";
		accounts[i]->showAccounts();
	}

	std::cout << "[Press 'ENTER' to continue]" << std::endl << std::endl;
	std::cin.ignore();

	pid_t *pid = new pid_t[clients];
	for (int i = 0; i < clients; i++) {
		pid[i] = fork();
		
		if (pid[i] < 0) {
			std::cout << "Epic Error In Forking" << std::endl;
			return 1;
		} else if (pid[i] == 0) {
			pAccount = _pAccounts + sizeOf(account) * i;
			(*pAccounts)->transferFunds(AMOUNT);
			return 0;
		}
	}

	for (int i = 0; i < clients; i++) {
		int status;
		while (waitpid(pid[i], &status, 0) == -1);
	}
	
	for (int i = 0; i < clients; i++) {
		pAccount = _pAccounts + sizeOf(account) * i;
		*pAccounts = new Account(rand() % 100, rand() % 100 + 10);
		std::cout << "Client " << (i + 1) << ": ";
		accounts[i]->showAccounts();
	}

	munmap(pAccounts, sizeof *account);
	delete pid;
	delete _pAccounts;
	return 0;
}
