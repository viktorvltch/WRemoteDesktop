#ifndef WMAINWINDOW_H
#define WMAINWINDOW_H


#include <QApplication>
#include <QtNetwork/QTcpSocket>
#include <QMouseEvent>
#include <QWidget>
#include <QBoxLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QPushButton>


class WMainWindow : public QWidget {
	Q_OBJECT

	public:
		WMainWindow();
		~WMainWindow();
	public slots:
		void WServerConnect();
		void WServerDisconnect();
		void WConnected();
		void WDisconnected();
		void WReadServer();
		void WErrorSocket(QAbstractSocket::SocketError erreur);
		void WLogsAppended();
	private:
		QVBoxLayout *vBoxLayout_main;
		QHBoxLayout *hBoxLayout_toolbar;
			QVBoxLayout *vBoxLayout_controls;
				QLabel *lbl_serverAddress;
				QLineEdit *tbx_serverAddress;
				QHBoxLayout *hBoxLayout_buttons;
					QPushButton *btn_connect;
					QPushButton *btn_disconnect;
			QTextEdit *tbx_logs;

		QLabel *lbl_status;
		QPushButton *btn_quit;
};


#endif // WMAINWINDOW_H
