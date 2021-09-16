#ifndef WSERVERSSELECTDIALOG_H
#define WSERVERSSELECTDIALOG_H


#include <QApplication>
#include <QDialog>
#include <QtNetwork/QHostAddress>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

class WServerSelectDialog : public QDialog {
	Q_OBJECT

	public:
		WServerSelectDialog(QWidget *parent, QHash<QString, QHostAddress> paramServers);
		~WServerSelectDialog();
		QHostAddress WGetSelectedServer();
	public slots:
		void WServerSelected();
	private:
		QVBoxLayout *vBoxLayout_main;
};

#endif // WSERVERSSELECTDIALOG_H
