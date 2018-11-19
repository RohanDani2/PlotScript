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
	const double PI = std::atan2(0, -1);
	if ((event->matches(QKeySequence::InsertLineSeparator))) {
		emit sendErase();
		std::istringstream iss(QPlainTextEdit::toPlainText().toStdString());
		if (!interp.parseStream(iss)) {
			std::string errorString = "Error: Could not Parse";
			emit sendError(errorString);
		}
		else {
			std::string textString;
			std::string expressionString;
			std::stringstream ss;
			double x1 = 0;
			double y1 = 0;
			double x2 = 0;
			double y2 = 0;
			try {
				Expression expression = interp.evaluate();
				std::string command = expression.getProp("\"object-name\"").head().asSymbol();
				if (command == "\"point\"") {
					double size = 0;
					x1 = expression.tailVal()[0].head().asNumber();
					y1 = expression.tailVal()[1].head().asNumber();
					if (expression.getProp("\"size\"").head().isNumber()) {
						size = expression.getProp("\"size\"").head().asNumber();
					}
					emit sendPoint(x1, y1, size);
				}
				else if (command == "\"line\"") {
					double thickness = 1;
					if (true) {
						thickness = expression.getProp("\thickness\"").head().asNumber();
					}
					x1 = expression.tailVal()[0].head().asNumber();
					y1 = expression.tailVal()[1].head().asNumber();
					x2 = expression.tailVal()[1].tailVal()[0].head().asNumber();
					y2 = expression.tailVal()[1].tailVal()[1].head().asNumber();
					emit sendLine(x1, x2, y1, y2, thickness);
				}
				else if (command == "\"text\"") {
					int textScale = 1;
					double rotation = 0;
					double x = 0;
					double y = 0;
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
					emit sendText(expressionString, x, y, textScale, rotation);
				}
				else {
					ss << expression;
					expressionString = ss.str();
					emit sendText(expressionString, 0, 0, 1, 0);
				}
			}
			catch (const SemanticError & ex) {
				std::string errorString = "Error: Error During Evaluation Unknown Symbol";
				emit sendError(errorString);
				
			}
			
		}
	}
	else {
		QPlainTextEdit::keyPressEvent(event);
	}
}
