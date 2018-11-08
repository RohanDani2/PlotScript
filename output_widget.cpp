#include <output_widget.hpp>

OutputWidget::OutputWidget(QWidget * parent) : QWidget(parent) {
	auto output = new QPlainTextEdit;
	output->setObjectName("output");
	auto layout = new QGridLayout;
	layout->addWidget(output, 0, 0);
	setLayout(layout);
}