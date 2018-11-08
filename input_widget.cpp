#include "input_widget.hpp"
#include "interpreter.hpp"

InputWidget::InputWidget(QPlainTextEdit * parent) : QPlainTextEdit(parent) {
	auto input = new QPlainTextEdit;
	input->setObjectName("input");
	auto layout = new QGridLayout;
	layout->addWidget(input, 0, 0);
	setLayout(layout);
}

void InputWidget::keyPressEvent(QKeyEvent *event){
	if (event->type() == QEvent::KeyPress) {
		if ((event->modifiers() & Qt::ShiftModifier) &&
			((event->modifiers() & Qt::Key_Enter) || (event->modifiers() & Qt::Key_Return))) {
			//
		}
	}
}
