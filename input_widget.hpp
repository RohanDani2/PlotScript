#ifndef INPUT_WIDGET_H
#define INPUT_WIDGET_H

#include "interpreter.hpp"
#include "expression.hpp"
#include "semantic_error.hpp"

#include <QPlainTextEdit>
#include <QWidget>
#include <QLayout>
#include <QString>
#include <QInputDialog>

class InputWidget : public QPlainTextEdit {
	Q_OBJECT
public:
	InputWidget(QWidget * parent = nullptr);
	void keyPressEvent(QKeyEvent *event);
signals:
	void sendErase();
	void sendExpression(std::string expressionString);
	void sendError(std::string errorString);
	void sendText(std::string textString,double x,double y,int textScale,double rotation);
	void sendPoint(double x1,double y1,double size);
	void sendLine(double x1, double x2, double y1, double y2, double thickness);
private:
	Interpreter interp;
	std::ostringstream os;
};

#endif // ! INPUT_WIDGET_H