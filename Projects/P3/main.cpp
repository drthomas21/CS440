#include <string>
#include <iostream>
#include <cstdio>
#include <memory>
#include <regex>
#include <unistd.h>
#include <stdio.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/wait.h> 
#include <sys/mman.h>

const size_t commandSize = 9;
std::string currentDir = "";
const std::string validCommands[commandSize] = {"login","dir","mkdir","cd","create","delete","read","write","logout"};

std::string exec(const char*);
int setCurrentDirectory();
void doCommand(size_t, std::string);
void doLogin(std::string);
void doDir();
void doMkDir(std::string);
void doCd(std::string);
void doCreate(std::string);
void doDelete(std::string);
void doRead(std::string);
void doWrite(std::string);

int main() {	
	std::string line;
	
	do {
		pid_t pid = fork();
		if(pid < 0) {
			std::cout << "Epic Error In Fork" << std::endl;
			return 1;
		} else if(pid > 0) {
			wait(NULL);
		} else {		
			std::string username;
			std::cout << "username:";
			getline(std::cin,username);
			line = "login ";
			line.append(username);
			doLogin(line);
			
			do {
				std::cout << "Command: ";
				getline(std::cin,line);
				
				std::regex search("^([A-Za-z]+).*");
				std::smatch matches;
				
				if(std::regex_match(line,matches,search)) {
					std::string command = matches[1].str();
					bool foundCommand = false;
					for(size_t i = 0; i < commandSize; i++) {
						if(command.compare(validCommands[i]) == 0) {
							foundCommand = true;
							doCommand(i,line);
						}
					}
					
					if(!foundCommand) {
						std::cout << "Invalid command" << std::endl;
					}
				}
			} while(true);
		}
	} while(true);
		
}

int setCurrentDirectory() {
	char path[255];
	getcwd(path,255);
	currentDir = path;
	return 0;
}

std::string exec(const char *cmd) {
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while (!feof(pipe.get())) {
        if (fgets(buffer, 128, pipe.get()) != NULL)
            result += buffer;
    }
    return result;
}

void doCommand(size_t idx, std::string line) {
	switch(idx) {
		case (0):
			doLogin(line);
			break;
		case (1):
			doDir();
			break;
		case (2):
			doMkDir(line);
			break;
		case (3):
			doCd(line);
			break;
		case (4):
			doCreate(line);
			break;
		case (5):
			doDelete(line);
			break;
		case (6):
			doRead(line);
			break;
		case (7):
			doWrite(line);
			break;
		case (8):
			exit(0);
			break;
		default:
			std::cout << "Command not found" << std::endl;
			break;
	}
}

void doLogin(std::string line) {
	std::regex search("^login ([A-Za-z0-9\\-\\_]+).*");
	std::smatch matches;
	std::string username;
	std::string password;
	
	if(std::regex_match(line, matches, search)) {
		username = matches[1].str();
	}
	
	if(username.empty()) {
		std::cout << "invalid username" << std::endl;
	} else {
		struct passwd *_passwd;
		
		if((_passwd = getpwnam(username.c_str())) == NULL) {
			std::cout << "user does not exists" << std::endl;
		} else {
			if(setuid(_passwd->pw_uid) == -1) {
				std::cout << "cannot switch to account" << std::endl;
			}
			//std::string command = "su -p -c ./main-ubuntu_14_04_amd64 -l ";
			//command.append(username);
			//std::cout << execInput(command.c_str(),password.c_str()) << std::endl;
			//std::cout << exec(command.c_str()) << std::endl;
		}		
	}
}

void doDir() {
	std::cout << exec("ls") << std::endl;
}

void doMkDir(std::string line) {
	std::regex search("^mkdir ([A-Za-z0-9\\-\\_\\.\\(\\)~]+|\"[A-Za-z0-9\\-\\_\\.\\(\\)~ ]+\").*");
	std::smatch matches;
	std::string foldername;
	
	if(std::regex_match(line, matches, search)) {
		foldername = matches[1].str();
	}
	
	if(foldername.empty()) {
		std::cout << "invalid foldername" << std::endl;
	} else {
		std::string command = "mkdir ";
		command.append(foldername);
		std::cout << exec(command.c_str()) << std::endl;
	}	
}

void doCd(std::string line) {
	std::regex search("^cd ([A-Za-z0-9\\-\\_\\./\\(\\)~]+|\"[A-Za-z0-9\\-\\_\\./\\(\\)~ ]+\").*");
	std::smatch matches;
	std::string pathname;
	
	if(std::regex_match(line, matches, search)) {
		pathname = matches[1].str();
	}
	
	if(pathname.empty()) {
		std::cout << "invalid path" << std::endl;
	} else {
		if( (pathname[0] != '.' && pathname[0] != '/') || (pathname[0] == '.' && pathname[1] == '/') ) {
			setCurrentDirectory();
			std::string _pathname = pathname;
			pathname = currentDir;
			pathname.append("/");
			pathname.append(_pathname);
		}
		std::string command = "cd ";
		command.append(pathname);
		chdir(pathname.c_str());
		std::cout << exec(command.c_str()) << std::endl;
	}
}

void doCreate(std::string line) {
	std::regex search("^create ([A-Za-z0-9\\-\\_\\.\\(\\)~]+|\"[A-Za-z0-9\\-\\_\\.\\(\\)~ ]+\").*");
	std::smatch matches;
	std::string filename;
	
	if(std::regex_match(line, matches, search)) {
		filename = matches[1].str();
	}
	
	if(filename.empty()) {
		std::cout << "invalid filename" << std::endl;
	} else {
		std::string command = "touch ";
		command.append(filename);
		std::cout << exec(command.c_str()) << std::endl;
	}
}

void doDelete(std::string line) {
	std::regex search("^delete ([A-Za-z0-9\\-\\_\\.\\(\\)~]+|\"[A-Za-z0-9\\-\\_\\.\\(\\)~ ]+\").*");
	std::smatch matches;
	std::string filename;
	
	if(std::regex_match(line, matches, search)) {
		filename = matches[1].str();
	}
	
	if(filename.empty()) {
		std::cout << "invalid filename" << std::endl;
	} else {
		std::string command = "rm ";
		command.append(filename);
		std::cout << exec(command.c_str()) << std::endl;
	}
}

void doRead(std::string line) {
	std::regex search("^read ([A-Za-z0-9\\-\\_\\.\\(\\)~]+|\"[A-Za-z0-9\\-\\_\\.\\(\\)~ ]+\").*");
	std::smatch matches;
	std::string filename;
	
	if(std::regex_match(line, matches, search)) {
		filename = matches[1].str();
	}
	
	if(filename.empty()) {
		std::cout << "invalid filename" << std::endl;
	} else {
		std::string command = "cat ";
		command.append(filename);
		std::cout << exec(command.c_str()) << std::endl;
	}
}

void doWrite(std::string line) {
	std::regex search("^write ([A-Za-z0-9\\-\\_\\.\\(\\)~]+|\"[A-Za-z0-9\\-\\_\\.\\(\\)~ ]+\").*");
	std::smatch matches;
	std::string filename;
	std::string content;
	
	if(std::regex_match(line, matches, search)) {
		filename = matches[1].str();
	}
	
	if(filename.empty()) {
		std::cout << "invalid filename" << std::endl;
	} else {
		std::cout << "What do you want to write: ";
		getline(std::cin,content);
		
		std::string command = "echo \"";
		command.append(content);
		command.append("\" | tee ");
		command.append(filename);
		std::cout << exec(command.c_str()) << std::endl;
	}
}
