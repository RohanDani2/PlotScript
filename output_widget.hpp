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
	void displayErase();
	void displayError(std::string errorString);
	void displayText(std::string textString, double x, double y, int textScale, double rotation);
	void displayPoint(double x1, double y1, double size);
	void displayLine(double x1, double x2, double y1, double y2,double thickness);
private:
	QGraphicsView *view;
	QGraphicsScene *scene;
};

#endif // ! OUTPUT_WIDGET_H