#include "output_widget.hpp"

#include <QVBoxLayout>
#include <QLayout>
#include <iostream>


OutputWidget::OutputWidget(QWidget * parent) : QWidget(parent) {
	scene = new QGraphicsScene;
	QGraphicsView *view = new QGraphicsView(scene);
	auto layout = new QVBoxLayout;
	layout->addWidget(view);
	setLayout(layout);
}

void OutputWidget::displayError(std::string errorString) {
	std::cout << errorString;
}

void OutputWidget::displayPoint(double x1, double x2, double y1, double y2)
{
}

void OutputWidget::displayLine(double x1, double x2, double y1, double y2)
{
}

void OutputWidget::displayExpression(std::string interpretString){
	//
}

