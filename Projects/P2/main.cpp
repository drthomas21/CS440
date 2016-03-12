#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits>
#include <time.h>
#include <mutex>
#include <sys/types.h>
#include <sys/wait.h> 
#include <sys/mman.h>

const size_t charSize = 26, bufferSize = 10, loop = 5;


const char characters[charSize] = { 'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z' };
const int producers = 2, consumers = 2;
std::mutex procLock;
char *buffer;
size_t *in = 0, *out = 0;

void criticalSection(char&, bool);

int main() {	
	pid_t *pid = new pid_t[producers + consumers];
	size_t pidIter = 0;
	buffer = static_cast<char*>(mmap(NULL, (sizeof *buffer) * bufferSize, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0));
	in = static_cast<size_t*>(mmap(NULL, (sizeof *in), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0));
	out = static_cast<size_t*>(mmap(NULL, (sizeof *out), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0));
	srand(static_cast<unsigned int>(time(NULL)));

	//Fill Buffer
	for (size_t i = 0; i < bufferSize; i++) {
		buffer[i] = characters[rand() % charSize];
	}

	//Create Producers
	for (int i = 0; i < producers; i++) {
		pid_t _pid = fork();
		if (_pid < 0) {
			std::cout << "Epic Forking Error" << std::endl;
			return 1;
		} else if (_pid > 0) {
			pid[pidIter] = _pid;
			pidIter++;
		} else {
			std::cout << "created producer" << std::endl;
			srand(static_cast<unsigned int>(clock()));
			for (int i = 0; i < loop; i++) {
				sleep(rand() % 10);
				//Do Producer
				char item = characters[rand() % charSize];
				criticalSection(item, true);
			}
			std::cout << "producer finished" << std::endl;
			return 0;
		}
	}

	//Create Consumers
	for (int i = 0; i < consumers; i++) {
		pid_t _pid = fork();
		if (_pid < 0) {
			std::cout << "Epic Forking Error" << std::endl;
			return 1;
		}
		else if (_pid > 0) {
			pid[pidIter] = _pid;
			pidIter++;			
		}
		else {
			std::cout << "created consumer" << std::endl;
			srand(static_cast<unsigned int>(clock()));
			for (int i = 0; i < loop; i++) {
				sleep(rand() % 10);
				//Do Consumer
				char item;
				criticalSection(item, false);
			}
			std::cout << "consumer finished" << std::endl;
			return 0;
		}
	}

	//Wait on Procs
	std::cout << "waiting on child procs" << std::endl;
	for (int i = 0; i < producers + consumers; i++) {
		int status;
		while (waitpid(pid[i], &status, 0) == -1);
	}

	//Cleanup
	std::cout << "cleaning up shared memory" << std::endl;
	munmap(buffer, (sizeof *buffer) * bufferSize);
	delete pid;

	return 0;
}

void criticalSection(char &item, bool isSaving) {
	std::lock_guard<std::mutex> lock(procLock);
	
	//Buffer
	std::cout << "Buffer: {";
	for(size_t i = 0; i < bufferSize-1;i++) {
		std::cout << buffer[i] << ", ";
	}
	std::cout << buffer[bufferSize-1] << "}" << std::endl;
	
	if (isSaving) {
		std::cout << "inserting " << item << " at " << *in << std::endl;
		buffer[*in] = item;
		*in = (*in + 1) % bufferSize;
	} else {
		item = buffer[*out];
		std::cout << "retreiving " << item << " at " << *out << std::endl;
		*out = (*out + 1) % bufferSize;
	}
	
	std::cout << std::endl;
}