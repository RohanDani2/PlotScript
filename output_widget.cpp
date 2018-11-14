#include "output_widget.hpp"

#include <QVBoxLayout>
#include <QLayout>
#include <iostream>


OutputWidget::OutputWidget(QWidget * parent) : QWidget(parent) {
	scene = new QGraphicsScene;
	view = new QGraphicsView(scene);
	auto layout = new QVBoxLayout;
	layout->addWidget(view);
	setLayout(layout);
}

void OutputWidget::displayError(std::string errorString) {
	QFont font = QFont("Courier");
	QString qstr;
	qstr = QString::fromStdString(errorString);
	QGraphicsTextItem *text = scene->addText(qstr,font);
	text->setPos(0, 0);
	scene->addItem(text);
	//QRect rect = text->boundingRect();
	
}

void OutputWidget::displayText(std::string textString) {
	QFont font = QFont("Courier");
	QString qstr;
	qstr = QString::fromStdString(textString);
	QGraphicsTextItem *text = scene->addText(qstr, font);
	text->setPos(0, 0);
	scene->addItem(text);

}

void OutputWidget::displayExpression(std::string expressionString) {
	QFont font = QFont("Courier");
	QString qstr;
	qstr = QString::fromStdString(expressionString);
	QGraphicsTextItem *text = scene->addText(qstr, font);
	text->setPos(0, 0);
	scene->addItem(text);
}

void OutputWidget::displayPoint(double x1, double x2, double y1, double y2)
{
}

void OutputWidget::displayLine(double x1, double x2, double y1, double y2)
{
}


