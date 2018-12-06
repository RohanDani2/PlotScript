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
#include <thread>


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
	void threadWorker(MessageQueue<std::string> *inputQueue, MessageQueue<queueStruct> *outputQueue, Interpreter * interp);
	void startThread();
signals:
	void sendErase();
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
	std::thread *secondThread;
	MessageQueue<std::string> inputQueue;
	MessageQueue<queueStruct> outputQueue;
};

#endif // ! INPUT_WIDGET_H