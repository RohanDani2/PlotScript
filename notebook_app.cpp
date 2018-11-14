#include "notebook_app.hpp"
#include "interpreter.hpp"

NotebookApp::NotebookApp(QWidget * parent) : QWidget(parent) {
	QObject::connect(input, SIGNAL(sendError(std::string)), output, SLOT(displayError(std::string)));
	QObject::connect(input, SIGNAL(sendText(std::string)), output, SLOT(displayText(std::string)));
	QObject::connect(input, SIGNAL(sendLine(double x1, double x2, double y1, double y2)), output, SLOT(displayLine(double x1, double x2, double y1, double y2)));
	QObject::connect(input, SIGNAL(sendPoint(double x1, double x2, double width, double height)), output, SLOT(displayPoint(double x1, double x2, double width, double height)));
	auto layout = new QVBoxLayout;
	input->setObjectName("input");
	output->setObjectName("output");
	layout->addWidget(input);
	layout->addWidget(output);
	setLayout(layout);
}
