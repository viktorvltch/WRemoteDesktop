#include <QScreen>
#include <QPixmap>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QUdpSocket>
#include <QBuffer>
#include <QTimer>
#include <QNetworkInterface>
#include "WMainWindow.h"


#define PORT 8088

QTimer *timer;
QTcpServer *tcp_server;
QList<QTcpSocket *> clients;

QScreen *wscreen;

WMainWindow::WMainWindow() : QWidget() {
	vBoxLayout_main = new QVBoxLayout();

	lbl_status = new QLabel("Server offline");
	lbl_addresses = new QLabel;
	lbl_addresses->setStyleSheet("color:#600");
	btn_quit = new QPushButton("Quit");
	QObject::connect(btn_quit, SIGNAL(clicked()), qApp, SLOT(quit()));

	vBoxLayout_main->addWidget(lbl_status);
	vBoxLayout_main->addWidget(lbl_addresses);
	vBoxLayout_main->addWidget(btn_quit);

	setLayout(vBoxLayout_main);

	wscreen = QApplication::primaryScreen();

	tcp_server = new QTcpServer(this);
	if(!tcp_server->listen(QHostAddress::AnyIPv4, PORT)) {
		qDebug() << "Starting server failed!";
		return;
	}
	QStringList finalStr;
	for(QHostAddress &addr : QNetworkInterface::allAddresses())
		if(addr != QHostAddress::LocalHost && addr != QHostAddress::LocalHostIPv6)
			finalStr.append(addr.toString());

	lbl_status->setText("Server running on");

	lbl_addresses->setText(finalStr.join("\n"));

	QObject::connect(tcp_server, SIGNAL(newConnection()), this, SLOT(WClientConnected()));

	timer = new QTimer;
	QObject::connect(timer, SIGNAL(timeout()), this, SLOT(WSendFrame()));
	timer->start(50);

}

WMainWindow::~WMainWindow() {
	for(int i = 0; i < clients.size(); i++) {
		clients[i]->disconnectFromHost();
		clients[i]->deleteLater();
	}
	clients.clear();
	tcp_server->close();
	delete tcp_server;
	delete timer;
	delete wscreen;
	delete btn_quit;
	delete lbl_status;
	delete vBoxLayout_main;
}

void WMainWindow::WSendFrame() {
	QPixmap screenImg = wscreen->grabWindow(0);

	QByteArray bytes;
	QBuffer buffer(&bytes);
	buffer.open(QIODevice::WriteOnly);
	screenImg.save(&buffer, "PNG");
	buffer.close();

	QByteArray packet;
	QDataStream out(&packet, QIODevice::WriteOnly);

	out << (quint32) 0;
	out << bytes;
	out.device()->seek(0);
	out << (quint32) (packet.size() - sizeof(quint32));

	for(int i = 0; i < clients.size(); i++)
		clients[i]->write(packet);
}

void WMainWindow::WClientConnected() {
	QTcpSocket *client = tcp_server->nextPendingConnection();
	QObject::connect(client, SIGNAL(readyRead()), this, SLOT(WReadClient()));
	QObject::connect(client, SIGNAL(disconnected()), this, SLOT(WClientDisconnected()));

	clients << client;
}

void WMainWindow::WClientDisconnected() {
	QTcpSocket *client = qobject_cast<QTcpSocket *>(sender());
	if (client == 0)
		return;
	clients.removeOne(client);
	client->deleteLater();
}

void WMainWindow::WReadClient() {
	qDebug() << "Read client";
}
