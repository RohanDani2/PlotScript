#include "notebook_app.hpp"
#include "interpreter.hpp"
#include <QHBoxLayout>

NotebookApp::NotebookApp(QWidget * parent) : QWidget(parent) {
	InputWidget *input = new InputWidget;
	OutputWidget *output = new OutputWidget;
	start = new QPushButton("Start Kernel");
	stop = new QPushButton("Stop Kernel");
    reset = new QPushButton("Reset Kernel");
	interrupt = new QPushButton("Interrupt");
	auto layout = new QGridLayout;
	auto qhlayout = new QHBoxLayout;
	input->setObjectName("input");
	output->setObjectName("output");
	start->setObjectName("start");
	stop->setObjectName("stop");
	reset->setObjectName("reset");
	interrupt->setObjectName("interrupt");
	QObject::connect(input, &InputWidget::sendErase, output, &OutputWidget::displayErase);
	QObject::connect(input, &InputWidget::sendError, output, &OutputWidget::displayError);
	QObject::connect(input, &InputWidget::sendText, output, &OutputWidget::displayText);
	QObject::connect(input, &InputWidget::sendLine, output, &OutputWidget::displayLine);
	QObject::connect(input, &InputWidget::sendPoint, output, &OutputWidget::displayPoint);
	QObject::connect(start, SIGNAL(clicked()), input, SLOT(startKernel()));
	QObject::connect(stop, SIGNAL(clicked()), input, SLOT(stopKernel()));
	QObject::connect(reset, SIGNAL(clicked()), input, SLOT(resetKernel()));
	QObject::connect(interrupt, SIGNAL(clicked()), input, SLOT(interruptKernel()));

	layout->addWidget(input, 1, 0);
	layout->addWidget(output, 2, 0);
	qhlayout->addWidget(start);
	qhlayout->addWidget(stop);
	qhlayout->addWidget(reset);
	qhlayout->addWidget(interrupt);
	layout->addLayout(qhlayout, 0, 0);
	setLayout(layout);
}

