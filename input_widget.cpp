#include "input_widget.hpp"
#include "startup_config.hpp"
#include <QKeyEvent>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <math.h>

InputWidget::InputWidget(QWidget * parent) : QPlainTextEdit(parent) {
	setReadOnly(false);
	std::ifstream ifs;
	ifs.open(STARTUP_FILE);
	if (!interp.parseStream(ifs)) {
		std::string errorString = "Error: Could not Parse";
		emit sendError(errorString);
	}
	else {
		try {
			Expression exp = interp.evaluate();
		}
		catch (const SemanticError & ex) {
			std::cerr << ex.what() << std::endl;
		}
	}
}

void InputWidget::keyPressEvent(QKeyEvent *event){
	std::string textString;
	std::string expressionString;
	std::stringstream ss;
	const double PI = std::atan2(0, -1);
	if ((event->matches(QKeySequence::InsertLineSeparator))) {
		emit sendErase();
		std::istringstream iss(QPlainTextEdit::toPlainText().toStdString());
		//std::cout << "keypressShiftEnter";
		if (!interp.parseStream(iss)) {
			//std::cout << "error";
			std::string errorString = "Error: Could not Parse";
			emit sendError(errorString);
		}
		else {
			try {
				Expression expression = interp.evaluate();
				std::string command = expression.getProp("\"object-name\"").head().asSymbol();
				//double thickness = expression.getProp("\thickness\"").head().asNumber();
				if (command == "\"point\"") {
					double x1 = expression.getProp("\"object-name\"").tailVal()[0].head().asNumber();
					double y1 = expression.getProp("\"object-name\"").tailVal()[1].head().asNumber();
					double size = expression.getProp("\"size\"").head().asNumber();
					if (size != 0) {
						emit sendPoint(x1, y1, size);
					}
					emit sendPoint(x1, y1, 0);
				}
				else if (command == "\"line\"") {
					//
				}
				else if (command == "\"text\"") {
					int textScale = 1;
					double rotation = 0;
					double x = 0;
					double y = 0;
					if (expression.tailVal().size() == 0) {
						emit sendText(expression.head().asSymbol(), x, y, textScale, rotation);
					}
					if (expression.getProp("\"position\"").getProp("\"object-name\"").head().isSymbol()) {
						/*double x = expression.getProp("\"position\"").tailVal()[0].head().asNumber();
						double y = expression.getProp("\"position\"").tailVal()[1].head().asNumber();*/
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
					emit sendText(expressionString,x,y,textScale,rotation);
				}
				else {
					//
				}
			}
			catch (const SemanticError & ex) {
				std::cerr << ex.what() << std::endl;
			}
			
		}

	}
	else {
		QPlainTextEdit::keyPressEvent(event);
	}
}
