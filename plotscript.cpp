#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <thread>
#include <atomic> 
#include <csignal>
#include <cstdlib>
#include <startup_config.hpp>

#include "interpreter.hpp"
#include "semantic_error.hpp"
#include "multiThread.hpp"

volatile sig_atomic_t global_status_flag = 0;

#if defined(_WIN64) || defined(_WIN32)
#include <windows.h>

// this function is called when a signal is sent to the process
BOOL WINAPI interrupt_handler(DWORD fdwCtrlType) {

	switch (fdwCtrlType) {
	case CTRL_C_EVENT: // handle Cnrtl-C
					   // if not reset since last call, exit
		if (global_status_flag > 0) {
			exit(EXIT_FAILURE);
		}
		++global_status_flag;
		return TRUE;

	default:
		return FALSE;
	}
}

// install the signal handler
inline void install_handler() { SetConsoleCtrlHandler(interrupt_handler, TRUE); }
// *****************************************************************************

// *****************************************************************************
// install a signal handler for Cntl-C on Unix/Posix
// *****************************************************************************
#elif defined(__APPLE__) || defined(__linux) || defined(__unix) ||             \
    defined(__posix)
#include <unistd.h>

// this function is called when a signal is sent to the process
void interrupt_handler(int signal_num) {

	if (signal_num == SIGINT) { // handle Cnrtl-C
								// if not reset since last call, exit
		if (global_status_flag > 0) {
			exit(EXIT_FAILURE);
		}
		++global_status_flag;
	}
}

// install the signal handler
inline void install_handler() {

	struct sigaction sigIntHandler;

	sigIntHandler.sa_handler = interrupt_handler;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;

	sigaction(SIGINT, &sigIntHandler, NULL);
}
#endif

struct queueStruct {
	bool error;
	std::string errorString;
	Expression expression;
};

queueStruct output;
bool stopQueue = false;

void prompt(){
  std::cout << "\nplotscript> ";
}

std::string readline(){
  std::string line;
  std::getline(std::cin, line);

  return line;
}

void error(const std::string & err_str){
  std::cerr << "Error: " << err_str << std::endl;
}

void info(const std::string & err_str){
  std::cout << "Info: " << err_str << std::endl;
}

int eval_from_stream(std::istream & stream){

  Interpreter interp;
  
  std::ifstream ifs(STARTUP_FILE);


  if (!interp.parseStream(ifs)) {
	  error("Invalid Program. Could not parse.");
  }
  else {
	  try {
		  Expression exp = interp.evaluate();
	  }
	  catch (const SemanticError & ex) {
		  std::cerr << ex.what() << std::endl;
	  }
  }
  if(!interp.parseStream(stream)){
    error("Invalid Program. Could not parse.");
    return EXIT_FAILURE;
  }
  else{
    try{
      Expression exp = interp.evaluate();
      std::cout << exp << std::endl;
    }
    catch(const SemanticError & ex){
      std::cerr << ex.what() << std::endl;
      return EXIT_FAILURE;
    }	
  }

  return EXIT_SUCCESS;
}

int eval_from_file(std::string filename){
      
  std::ifstream ifs(filename);
  
  if(!ifs){
    error("Could not open file for reading.");
    return EXIT_FAILURE;
  }
  
  return eval_from_stream(ifs);
}

int eval_from_command(std::string argexp){

  std::istringstream expression(argexp);

  return eval_from_stream(expression);
}

void threadWorker(MessageQueue<std::string> *inputQueue, MessageQueue<queueStruct> *outputQueue, Interpreter *interp) {
	std::ifstream ifs(STARTUP_FILE);

	if (!interp->parseStream(ifs)) {
		error("Invalid Program. Could not parse.");
	}
	else {
		try {
			Expression exp = interp->evaluate();
		}
		catch (const SemanticError & ex) {
			std::cerr << ex.what() << std::endl;
		}
	}
	while (stopQueue == false) {
		std::string expressionString;
		if (inputQueue->try_pop(expressionString)) {
			std::istringstream expression(expressionString);
			if (expressionString != "" && !interp->parseStream(expression)) {
				output.errorString = "Error: Could not Parse";
				output.error = true;
				outputQueue->push(output);
			}
			else {
				try {
					Expression exp = interp->evaluate();
					output.expression = exp;
					output.error = false;
					outputQueue->push(output);
				}
				catch (const SemanticError & ex) {
					output.errorString = ex.what();
					output.error = true;
					outputQueue->push(output);
				}
			}
		}
	}
}

// A REPL is a repeated read-eval-print loop
int repl() {
	Interpreter interp;
	MessageQueue<std::string> inputQueue;
	MessageQueue<queueStruct> outputQueue;

	std::thread secondThread(&threadWorker, &inputQueue, &outputQueue, &interp);
	while (!std::cin.eof()) {
		prompt();
		std::string line = readline();

		if (line.empty()) continue;

		if (line == "%start") {
			Interpreter *new_interp = new Interpreter;
			if (stopQueue == false) {
				continue;
			}
			else {
				std::thread secondThread(&threadWorker, &inputQueue, &outputQueue, new_interp);
				secondThread.detach();
				stopQueue = false;
			}
			continue;
		}
		else if (line == "%stop") {
			stopQueue = true;
			if (secondThread.joinable()) {
				secondThread.join();
			}
			continue;
		}
		else if (line == "%reset") {
			Interpreter *new_interp = new Interpreter;

			stopQueue = true;
			if (secondThread.joinable()) {
				secondThread.join();
			}

			std::thread secondThread(&threadWorker, &inputQueue, &outputQueue, new_interp);
			stopQueue = false;

			if (secondThread.joinable()) {
				secondThread.detach();
			}
			continue;
		}
		else if (line == "%exit") {
			break;
		}

		std::istringstream expression(line);

		if (stopQueue == true) {
			std::cout << "Error: interpreter kernel not running";
		}
		else {
			inputQueue.push(line);
			outputQueue.wait_and_pop(output);

			if (output.error == true) {
				std::cout << output.errorString;
			}
			else {
				std::cout << output.expression;
			}
		}

	}
	if (secondThread.joinable()) {
		stopQueue = true;
		secondThread.join();
	}
	return EXIT_SUCCESS;
}

int main(int argc, char *argv[])
{  
  if(argc == 2){
    return eval_from_file(argv[1]);
  }
  else if(argc == 3){
    if(std::string(argv[1]) == "-e"){
      return eval_from_command(argv[2]);
    }
    else{
      error("Incorrect number of command line arguments.");
    }
  }
  else{
    return repl();
  }
    
  return EXIT_SUCCESS;
}
