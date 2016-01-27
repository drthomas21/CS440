#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits>
#include <pthread.h>

struct arg_struct {
	int a;
	int b;
};

void *thread1(void*);
void *thread2(void*);
int summation(int, int);
int product(int, int);

int main() {
	int a, b;
	arg_struct args;
	pthread_t thread[2];
	do {
		std::cout << "Input 'A' and 'B' " << std::endl;
		std::cin >> a >> b;
		if (std::cin.fail()) {
			std::cout << "Invalid value" << std::endl;
			a = 0;
			b = 0;
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<int>::max(), '\n');
		}
		if (a >= b) {
			std::cout << "Invalid value" << std::endl;
		}
	} while (a >= b);

	args.a = a;
	args.b = b;
	pthread_create(&thread[0], NULL, &thread1, &args);
	pthread_create(&thread[1], NULL, &thread2, &args);

	pthread_join(thread[0], NULL);
	pthread_join(thread[1], NULL);
	return 0;
}

void *thread1(void* args) {
	struct arg_struct *_args = (struct arg_struct *)args;
	std::cout << "The summation of (" << _args->a << ", " << _args->b << "): " << summation(_args->a, _args->b) << std::endl;
	return NULL;
}

void *thread2(void* args) {
	struct arg_struct *_args = (struct arg_struct *)args;
	std::cout << "The product of (" << _args->a << ", " << _args->b << "): " << product(_args->a, _args->b) << std::endl;
	return NULL;
}

int summation(int a, int b) {
	if (a == b) {
		return b;
	}
	return a + summation(a + 1,b);
}

int product(int a, int b) {
	if (a == b) {
		return b;
	}
	return a * product(a + 1,b);
}
