#include "input_widget.hpp"
#include "startup_config.hpp"
#include <QKeyEvent>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <math.h>

queueStruct output;
bool stopQueue = false;


InputWidget::InputWidget(QWidget * parent) : QPlainTextEdit(parent) {
	setReadOnly(false);
	std::ifstream ifs;
	ifs.open(STARTUP_FILE); //opens up startup file for make functions
	startThread();
	if (!interp.parseStream(ifs)) {
		std::string errorString = "Error: Could not Parse";
		emit sendError(errorString);
	}
	else {
		Expression exp = interp.evaluate();
	}
}

void InputWidget::startThread() { //creates the new thread
	Interpreter *new_interp = new Interpreter;
	secondThread = new std::thread(&InputWidget::threadWorker,this, &inputQueue, &outputQueue,new_interp);
}

void InputWidget::threadWorker(MessageQueue<std::string> *inputQueue, MessageQueue<queueStruct> *outputQueue, Interpreter * interp) {
	std::ifstream ifs(STARTUP_FILE);

	if (!interp->parseStream(ifs)) { //sends error if can't parse 
		std::string errorString = "Error: Could not Parse";
		output.errorString = "Error: Could not Parse";
		output.error = true;
		outputQueue->push(output);
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
		std::string expressionString; //does when 2nd thread is running 
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

void InputWidget::startKernel() {
	startThread();
	secondThread->detach();
	stopQueue = false;
}

void InputWidget::stopKernel() {
	stopQueue = true;
	if (secondThread->joinable()) {
		secondThread->join();
	}
}

void InputWidget::resetKernel() {
	stopKernel();
	startKernel();
}

void InputWidget::interruptKernel() {
}

void InputWidget::keyPressEvent(QKeyEvent *event){
	const double PI = std::atan2(0, -1);
	if ((event->matches(QKeySequence::InsertLineSeparator))) {
		emit sendErase();
		if (stopQueue == true) {
			std::string errorString = "Error: interpreter kernel not running";
			emit sendError(errorString);
		}
		else {
			std::istringstream iss(QPlainTextEdit::toPlainText().toStdString());
			if (!interp.parseStream(iss)) {
				std::string errorString = "Error: Could not Parse";
				emit sendError(errorString);
			}
			else {
				std::string textString;
				std::string errorString;
				std::string expressionString;
				std::stringstream ss;
				double x1 = 0;
				double y1 = 0;
				double x2 = 0;
				double y2 = 0;
				double size = 0;
				double thickness = 1;
				int textScale = 1;
				double rotation = 0;
				double x = 0;
				double y = 0;
				try {
					Expression expression = interp.evaluate();

					std::string command = expression.getProp("\"object-name\"").head().asSymbol();
					if (command == "\"point\"") {
						x1 = expression.tailVal()[0].head().asNumber();
						y1 = expression.tailVal()[1].head().asNumber();
						if (expression.getProp("\"size\"").head().isNumber()) {
							size = expression.getProp("\"size\"").head().asNumber();
						}
						if (size < 0) {
							errorString = "Error: Size is not a positive Number";
							emit sendError(errorString);
						}
						else {
							emit sendPoint(x1, y1, size);
						}
					}
					else if (expression.head().asSymbol() == "list" && expression.tailVal()[0].getProp("\"object-name\"").head().asSymbol() == "\"point\""
						&& expression.getProp("\"object-name\"").head().asSymbol() != "\"line\"") {
						int count = 0;
						for (auto it = expression.tailConstBegin(); it != expression.tailConstEnd(); it++) {
							x1 = expression.tailVal()[count].tailVal()[0].head().asNumber();
							y1 = expression.tailVal()[count].tailVal()[1].head().asNumber();
							size = expression.tailVal()[count].getProp("\"size\"").head().asNumber();
							count++;
							if (size < 0) {
								errorString = "Error: Size is not a positive Number";
								emit sendError(errorString);
							}
							else {
								emit sendPoint(x1, y1, size);
							}
						}
					}
					else if (command == "\"line\"") {

						if (expression.getProp("\"thickness\"").head().isNumber()) {
							thickness = expression.getProp("\thickness\"").head().asNumber();
						}
						x1 = expression.tailVal()[0].head().asNumber();
						y1 = expression.tailVal()[1].head().asNumber();
						x2 = expression.tailVal()[1].tailVal()[0].head().asNumber();
						y2 = expression.tailVal()[1].tailVal()[1].head().asNumber();
						if (thickness < 0) {
							errorString = "Error: Thickness is not a positive Number";
							emit sendError(errorString);
						}
						else {
							emit sendLine(x1, x2, y1, y2, thickness);
						}
					}
					else if (expression.head().asSymbol() == "list" && expression.tailVal()[0].getProp("\"object-name\"").head().asSymbol() == "\"line\"") {
						int lineCount = 0;
						for (auto it = expression.tailConstBegin(); it != expression.tailConstEnd(); it++) {
							x1 = expression.tailVal()[lineCount].tailVal()[0].tailVal()[0].head().asNumber();
							y1 = expression.tailVal()[lineCount].tailVal()[0].tailVal()[1].head().asNumber();
							x2 = expression.tailVal()[lineCount].tailVal()[1].tailVal()[0].head().asNumber();
							y2 = expression.tailVal()[lineCount].tailVal()[1].tailVal()[1].head().asNumber();
							if (expression.tailVal()[lineCount].getProp("\"thickness\"").head().isNumber()) {
								thickness = expression.tailVal()[lineCount].getProp("\thickness\"").head().asNumber();
							}
							if (thickness < 0) {
								errorString = "Error: Thickness is not a positive Number";
								emit sendError(errorString);
							}
							else {
								emit sendLine(x1, x2, y1, y2, thickness);
							}
							lineCount++;
						}
					}
					else if (command == "\"text\"") {
						if (expression.getProp("\"position\"").getProp("\"object-name\"").head().isSymbol()) {
							x = expression.getProp("\"position\"").tailVal()[0].head().asNumber();
							y = expression.getProp("\"position\"").tailVal()[1].head().asNumber();
						}
						if (expression.getProp("\"text-scale\"").head().isNumber()) {
							textScale = expression.getProp("\"text-scale\"").head().asNumber();
						}
						if (expression.getProp("\"text-rotation\"").head().isNumber()) {
							rotation = expression.getProp("\"text-rotation\"").head().asNumber();
							rotation = (rotation * (PI / 180));
						}
						ss << expression;
						expressionString = ss.str();
						if (command == "\"text\"") {
							std::string makeTextString;
							for (size_t i = 0; i < expressionString.length(); i++) {
								if ((expressionString[i] != '"') && (expressionString[i] != '(') && (expressionString[i] != ')')) {
									makeTextString.push_back(expressionString[i]);
								}
							}
							expressionString = makeTextString;
						}
						if (expression.getProp("\"position\"").getProp("\"object-name\"").head().asSymbol() != "\"point\"") {
							errorString = "Error: Position Not a Point";
							emit sendError(errorString);
						}
						else {
							emit sendText(expressionString, x, y, textScale, rotation);
						}
					}
					else if (expression.head().asSymbol() == "list" && expression.tailVal()[0].getProp("\"object-name\"").head().asSymbol() == "\"text\"") {
						int textCount = 0;
						for (auto it = expression.tailConstBegin(); it != expression.tailConstEnd(); it++) {
							if (expression.tailVal()[textCount].getProp("\"position\"").head().isSymbol()) {
								x = expression.tailVal()[textCount].getProp("\"position\"").tailVal()[0].head().asNumber();
								y = expression.tailVal()[textCount].getProp("\"position\"").tailVal()[1].head().asNumber();
							}
							if (expression.tailVal()[textCount].getProp("\"text-scale\"").head().isNumber()) {
								textScale = expression.getProp("\"text-scale\"").head().asNumber();
							}
							if (expression.tailVal()[textCount].getProp("\"text-rotation\"").head().isNumber()) {
								rotation = expression.getProp("\"text-rotation\"").head().asNumber();
								rotation = (rotation * (PI / 180));
							}
							std::stringstream ss;
							ss << expression.tailVal()[textCount].head().asSymbol();
							std::string expressionString = ss.str();
							if (expression.tailVal()[textCount].getProp("\"position\"").getProp("\"object-name\"").head().asSymbol() != "\"point\"") {
								errorString = "Error: Position Not a Point";
								emit sendError(errorString);
							}
							else {
								emit sendText(expressionString, x, y, textScale, rotation);
							}
							textCount++;
						}

					}
					else if (expression.head().asSymbol() == "lambda") {
						emit sendErase();
					}
					else {
						ss << expression;
						expressionString = ss.str();
						emit sendText(expressionString, 0, 0, 1, 0);
					}
				}
				catch (const SemanticError & ex) {
					ex.what();
					errorString = "Error: During Evaluation Unknown Symbol";
					emit sendError(errorString);

				}

			}
		}
	}
	else {
		QPlainTextEdit::keyPressEvent(event);
	}
	
}

