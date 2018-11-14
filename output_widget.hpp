#ifndef OUTPUT_WIDGET_H
#define OUTPUT_WIDGET_H

#include "expression.hpp"

#include <QWidget>
#include <QGraphicsTextItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <string>


class OutputWidget : public QWidget {
	Q_OBJECT
public:
	OutputWidget(QWidget * parent = nullptr);
public slots:
	void displayExpression(std::string expressionString);
	void displayError(std::string errorString);
	void displayText(std::string textString);
	void displayPoint(double x1, double x2, double width, double height);
	void displayLine(double x1, double x2, double y1, double y2);
private:
	QGraphicsView *view;
	QGraphicsScene *scene;
};

#endif // ! OUTPUT_WIDGET_H