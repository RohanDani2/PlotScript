#pragma once
#ifndef  OUTPUT_WIDGET_H
#define OUTPUT_WIDGET_H

#include <QApplication>
#include <QWidget>
#include <QPlainTextEdit>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QLayout>

class OutputWidget : public QWidget {
Q_OBJECT
public:
	OutputWidget(QWidget * parent = nullptr);
private:
	QGraphicsScene * scene;
	QGraphicsTextItem *text;
};

#endif // ! OUTPUT_WIDGET_H