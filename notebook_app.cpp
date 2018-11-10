#include "notebook_app.hpp"
#include "interpreter.hpp"

NotebookApp::NotebookApp(QWidget * parent) : QWidget(parent) {
	auto input = new InputWidget;
	auto output = new OutputWidget;
	auto layout = new QGridLayout;
	input->setObjectName("input");
	output->setObjectName("output");
	layout->addWidget(input, 0, 0);
	layout->addWidget(output, 1, 0);
	setLayout(layout);
}