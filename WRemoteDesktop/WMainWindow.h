#ifndef WMAINWINDOW_H
#define WMAINWINDOW_H


#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>


class WMainWindow : public QWidget {
	Q_OBJECT

	public:
		WMainWindow();
		~WMainWindow();
	public slots:
		void WSendFrame();
		void WClientConnected();
		void WClientDisconnected();
		void WReadClient();
	private:
		QVBoxLayout *vBoxLayout_main;
		QLabel *lbl_status;
		QLabel *lbl_addresses;
		QPushButton *btn_quit;
};


#endif // WMAINWINDOW_H
