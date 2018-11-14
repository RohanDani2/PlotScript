#include "input_widget.hpp"
#include "startup_config.hpp"
#include <QKeyEvent>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

InputWidget::InputWidget(QWidget * parent) : QPlainTextEdit(parent) {
	setReadOnly(false);
	std::ifstream ifs;
	std::string line;
	ifs.open(STARTUP_FILE);
	while (!ifs.eof()) {
		ifs >> line;
	}
}

void InputWidget::keyPressEvent(QKeyEvent *event){
	double x1;
	double x2;
	double y1;
	double y2;
	std::string textString;
	if ((event->matches(QKeySequence::InsertLineSeparator))) {
		std::stringstream iss(QPlainTextEdit::toPlainText().toStdString());
		std::cout << "keypressShiftEnter";
		if (!interp.parseStream(iss)) {
			std::cout << "error";
			std::string errorString = "Parsing Error";
			emit sendError(errorString);
		}
		else {
			try {
				Expression expression = interp.evaluate();
				std::string command = expression.getProp("\"object-name\"").head().asSymbol();
				if (command == "make-point") {
					for (auto it = expression.tailConstBegin(); it != expression.tailConstEnd(); it++) {
						int count = 0;
						Expression tempVal;
						tempVal.append(*it);
						if (count == 0) {
							x1 = tempVal.head().asNumber();
						}
						if (count == 1) {
							x2 = tempVal.head().asNumber();
						}
						count++;
					}
					emit sendPoint(x1, x2, 0, 0);
				}
				else if (command == "make-line") {
					//
				}
				else if (command == "make-text") {
					Expression tempVal;
					for (auto it = expression.tailConstBegin(); it != expression.tailConstEnd(); it++) {
						Expression tempVal;
						tempVal.append(*it);
					}
					emit sendText(tempVal.head().asSymbol());
				}
				os << expression;
				emit sendExpression(os.str());
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
