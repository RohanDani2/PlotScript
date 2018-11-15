#include "output_widget.hpp"

#include <QVBoxLayout>
#include <QLayout>
#include <QScrollBar>
#include <iostream>


OutputWidget::OutputWidget(QWidget * parent) : QWidget(parent) {
	scene = new QGraphicsScene;
	view = new QGraphicsView(scene);
	auto layout = new QVBoxLayout;
	view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	view->verticalScrollBar()->hide();
	view->verticalScrollBar()->resize(0, 0);
	layout->addWidget(view);
	setLayout(layout);
}

void OutputWidget::displayErase() {
	scene->clear();
}

void OutputWidget::displayError(std::string errorString) {
	QGraphicsTextItem *text;
	QFont font = QFont("Courier",1);
	QString qstr;
	qstr = QString::fromStdString(errorString);
	text = scene->addText(qstr);
	text->setPos(0, 0);
	
}

void OutputWidget::displayText(std::string textString, double x, double y, int textScale, double rotation) {
	QString qstr;
	qstr = QString::fromStdString(textString);
	QGraphicsTextItem *text = new QGraphicsTextItem(qstr);
	QFont font = QFont("Courier", 1);
	text->setPos(x, y);
	text->setScale(textScale);
	text->setRotation(rotation);
	scene->addItem(text);

}

void OutputWidget::displayExpression(std::string expressionString) {
	/*QGraphicsTextItem *text;
	QFont font = QFont("Courier", 1);
	QString qstr;
	qstr = QString::fromStdString(expressionString);
	text = scene->addText(qstr);
	text->setPos(0, 0);*/
}

void OutputWidget::displayPoint(double x1, double y1, double size)
{
	x1++;
	y1++;
	size++;
}

void OutputWidget::displayLine(double x1, double x2, double y1, double y2,double thickness)
{
	x1++;
	x2++;
	y1++;
	y2++;
	thickness++;
}


