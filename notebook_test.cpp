#include <QTest>
#include <qsignalspy.h>
#include "notebook_app.hpp"

class NotebookTest : public QObject {
  Q_OBJECT

private slots:

  void initTestCase();
  void testConstructor();


  // TODO: implement additional tests here
private:
	NotebookApp notebook;
	InputWidget * input;
	OutputWidget * output;
  

};

void NotebookTest::initTestCase(){
	input = notebook.findChild<InputWidget *>("input");
	output = notebook.findChild<OutputWidget *>("output");
}


QTEST_MAIN(NotebookTest)
#include "notebook_test.moc"

void NotebookTest::testConstructor()
{
	QVERIFY2(input, "Could not find: 'input'");
	QVERIFY2(output, "Could not find: 'output'");
}
