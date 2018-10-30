#pragma once
#ifndef  INPUT_WIDGET_H
#define INPUT_WIDGET_H

#include <QApplication>
#include <QPlainTextEdit>
#include <QWidget>

class InputWidget : QPlainTextEdit {
Q_OBJECT
public:
	InputWidget(const char * input);
	void keyPressEvent();
};

#endif // ! INPUT_WIDGET_H