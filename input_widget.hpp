#ifndef INPUT_WIDGET_H
#define INPUT_WIDGET_H

#include "interpreter.hpp"
#include "expression.hpp"
#include "semantic_error.hpp"
#include "multiThread.hpp"

#include <QPlainTextEdit>
#include <QWidget>
#include <QLayout>
#include <QString>
#include <QInputDialog>

struct queueStruct {
	bool error;
	std::string errorString;
	Expression expression;
};

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
private slots:
	void startKernel();
	void stopKernel();
	void resetKernel();
	void interruptKernel();
private:
	Interpreter interp;
	MessageQueue<std::string> inputQueue;
	MessageQueue<queueStruct> outputQueue;
	std::ostringstream os;
};

#endif // ! INPUT_WIDGET_H