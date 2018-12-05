#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <thread>
#include <atomic> 
#include <startup_config.hpp>

#include "interpreter.hpp"
#include "semantic_error.hpp"
#include "multiThread.hpp"

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

void threadWorker(MessageQueue<std::string> *inputQueue, MessageQueue<queueStruct> *outputQueue) {
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
	while (stopQueue == false) {
		std::string expressionString;
		if (inputQueue->try_pop(expressionString)) {
			std::istringstream expression(expressionString);
			if (expressionString != "" && !interp.parseStream(expression)) {
				output.errorString = "Error: Could not Parse";
				output.error = true;
				outputQueue->push(output);
			}
			else {
				try {
					Expression exp = interp.evaluate();
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

	std::thread secondThread(&threadWorker, &inputQueue, &outputQueue);
	while (!std::cin.eof()) {
		prompt();
		std::string line = readline();

		if (line.empty()) continue;

		if (line == "%start") {
			if (stopQueue == false) {
				continue;
			}
			else {
				std::thread secondThread(&threadWorker, &inputQueue, &outputQueue);
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
