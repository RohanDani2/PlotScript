#include "notebook_app.hpp"
#include "interpreter.hpp"
#include <QHBoxLayout>
#include <qpushbutton.h>

NotebookApp::NotebookApp(QWidget * parent) : QWidget(parent) {
	InputWidget *input = new InputWidget;
	OutputWidget *output = new OutputWidget;
	QPushButton *start = new QPushButton("Start Kernel");
	QPushButton *stop = new QPushButton("Stop Kernel");
	QPushButton *reset = new QPushButton("Reset Kernel");
	QPushButton *interrupt = new QPushButton("Interrupt");
	auto layout = new QGridLayout;
	auto qhlayout = new QHBoxLayout;
	input->setObjectName("input");
	output->setObjectName("output");
	start->setObjectName("start");
	stop->setObjectName("stop");
	reset->setObjectName("reset");
	layout->addWidget(input,1,0);
	layout->addWidget(output,2,0);
	qhlayout->addWidget(start);
	qhlayout->addWidget(stop);
	qhlayout->addWidget(reset);
	qhlayout->addWidget(interrupt);
	layout->addLayout(qhlayout,0,0);
	setLayout(layout);
	QObject::connect(input, &InputWidget::sendErase, output, &OutputWidget::displayErase);
	QObject::connect(input, &InputWidget::sendError, output, &OutputWidget::displayError);
	QObject::connect(input, &InputWidget::sendText, output, &OutputWidget::displayText);
	QObject::connect(input, &InputWidget::sendLine, output, &OutputWidget::displayLine);
	QObject::connect(input, &InputWidget::sendPoint, output, &OutputWidget::displayPoint);
}
