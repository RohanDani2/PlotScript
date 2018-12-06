#include "output_widget.hpp"

#include <QVBoxLayout>
#include <QLayout>
#include <QScrollBar>
#include <QPushButton>
#include <iostream>


OutputWidget::OutputWidget(QWidget * parent) : QWidget(parent) {
	scene = new QGraphicsScene;  //add the scene
	view = new QGraphicsView(scene);
	auto layout = new QVBoxLayout;
	view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); //turn off scroll bar 
	view->verticalScrollBar()->hide();
	view->verticalScrollBar()->resize(0, 0);
	layout->addWidget(view); //add the widget with the view 
	setLayout(layout);
}

void OutputWidget::displayErase() {
	scene->clear(); //erase screen
}

void OutputWidget::displayError(std::string errorString) {
	QGraphicsTextItem *text;
	QFont font = QFont("Courier",1); //set the font 
	QString qstr;
	qstr = QString::fromStdString(errorString); //convert to the qstring 
	text = scene->addText(qstr) ;
	text->setPos(0, 0); //set the position
	
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

void OutputWidget::displayPoint(double x1, double y1, double size) {
	QPen pen;
	scene->addEllipse(x1-(size/2), y1 - (size / 2), size, size, Qt::NoPen, QBrush(QColor("000000")));
}

void OutputWidget::displayLine(double x1, double x2, double y1, double y2,double thickness) {
	QPen pen;
	pen.setWidth(thickness);
	pen.setColor("000000");
	scene->addLine(x1, y1, x2, y2, pen);
}



