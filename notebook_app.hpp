#pragma once
#ifndef  NOTEBOOK_APP_H
#define  NOTEBOOK_APP_H

#include <QApplication>
#include <QWidget>

class NotebookApp : public QWidget{
Q_OBJECT
public:
	bool event();
};
#endif