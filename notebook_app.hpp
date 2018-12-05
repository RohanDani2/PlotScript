#pragma once
#ifndef  NOTEBOOK_APP_H
#define  NOTEBOOK_APP_H

#include <QApplication>
#include <QWidget>
#include <qpushbutton.h>
#include <input_widget.hpp>
#include <output_widget.hpp>

class NotebookApp : public QWidget{
Q_OBJECT
public:
	NotebookApp(QWidget * parent = nullptr);
public slots:

private:
	QPushButton * start;
	QPushButton *stop;
	QPushButton *reset;
	QPushButton *interrupt;
};
#endif