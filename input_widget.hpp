#pragma once
#ifndef  INPUT_WIDGET_H
#define INPUT_WIDGET_H

#include <QApplication>
#include <QPlainTextEdit>
#include <QWidget>
#include <QLayout>

class InputWidget : public QWidget {
Q_OBJECT
public:
	InputWidget(QWidget * parent = nullptr);
	void keyPressEvent(QKeyEvent *event);
};

#endif // ! INPUT_WIDGET_H