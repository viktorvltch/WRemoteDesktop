#include <QApplication>
#include "WMainWindow.h"



int main(int argc, char *argv[]) {
	QApplication app(argc, argv);

	WMainWindow main;
	main.show();

	return app.exec();
}
