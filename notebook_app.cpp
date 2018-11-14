#include "notebook_app.hpp"
#include "interpreter.hpp"

NotebookApp::NotebookApp(QWidget * parent) : QWidget(parent) {
	InputWidget *input = new InputWidget;
	OutputWidget *output = new OutputWidget;
	QObject::connect(input, &InputWidget::sendExpression, output, &OutputWidget::displayExpression);
	QObject::connect(input, &InputWidget::sendError, output, &OutputWidget::displayError);
	QObject::connect(input, &InputWidget::sendText, output, &OutputWidget::displayText);
	QObject::connect(input, &InputWidget::sendLine, output, &OutputWidget::displayLine);
	QObject::connect(input, &InputWidget::sendPoint, output, &OutputWidget::displayPoint);
	auto layout = new QVBoxLayout;
	input->setObjectName("input");
	output->setObjectName("output");
	layout->addWidget(input);
	layout->addWidget(output);
	setLayout(layout);
}
