#include <QTimer>
#include <QPixmap>
#include <QScreen>
#include <QScrollBar>
#include "WMainWindow.h"
#include "WServersSelectDialog.h"


#define PORT 8088

QTcpSocket *socket;
int messageLength = 0;
bool listenState = false;

WMainWindow::WMainWindow() : QWidget() {
	int width = QApplication::primaryScreen()->geometry().width() / 2;
	int height = QApplication::primaryScreen()->geometry().height() / 2;
	setMinimumSize(width, height);

	vBoxLayout_main = new QVBoxLayout;

	hBoxLayout_toolbar = new QHBoxLayout;
	vBoxLayout_controls = new QVBoxLayout;

	lbl_serverAddress = new QLabel("Server address :");
	tbx_serverAddress = new QLineEdit;
	tbx_serverAddress->setMaximumWidth(200);
	QObject::connect(tbx_serverAddress, SIGNAL(returnPressed()), this, SLOT(WServerConnect()));

	hBoxLayout_buttons = new QHBoxLayout;
	btn_connect = new QPushButton("Connect");
	btn_connect->setAutoDefault(true);
	btn_connect->setDefault(true);
	QObject::connect(btn_connect, SIGNAL(clicked()), this, SLOT(WServerConnect()));

	btn_disconnect = new QPushButton("Disconnect");
	btn_disconnect->setEnabled(false);
	QObject::connect(btn_disconnect, SIGNAL(clicked()), this, SLOT(WServerDisconnect()));
	hBoxLayout_buttons->addWidget(btn_connect);
	hBoxLayout_buttons->addWidget(btn_disconnect);

	vBoxLayout_controls->addWidget(lbl_serverAddress);
	vBoxLayout_controls->addWidget(tbx_serverAddress);
	vBoxLayout_controls->addLayout(hBoxLayout_buttons);

	tbx_logs = new QTextEdit;
	tbx_logs->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Ignored);
	tbx_logs->setReadOnly(true);
	QObject::connect(tbx_logs, SIGNAL(textChanged()), this, SLOT(WLogsAppended()));




	hBoxLayout_toolbar->addLayout(vBoxLayout_controls);
	hBoxLayout_toolbar->addWidget(tbx_logs);


	lbl_status = new QLabel();
	lbl_status->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	btn_quit = new QPushButton("Quit");
	QObject::connect(btn_quit, SIGNAL(clicked()), qApp, SLOT(quit()));

	vBoxLayout_main->addLayout(hBoxLayout_toolbar);
	vBoxLayout_main->addWidget(lbl_status);
	vBoxLayout_main->addWidget(btn_quit);

	setLayout(vBoxLayout_main);

	socket = new QTcpSocket(this);
	QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(WReadServer()));
	QObject::connect(socket, SIGNAL(connected()), this, SLOT(WConnected()));
	QObject::connect(socket, SIGNAL(disconnected()), this, SLOT(WDisconnected()));
	QObject::connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(WErrorSocket(QAbstractSocket::SocketError)));


	resize(width, height + btn_quit->sizeHint().height());
}

WMainWindow::~WMainWindow() {
	socket->disconnectFromHost();
	socket->close();
	delete socket;
	delete btn_quit;
	delete lbl_status;
	delete vBoxLayout_main;
}

void WMainWindow::WServerConnect() {
	tbx_logs->append("Connecting to " + tbx_serverAddress->text());
	socket->connectToHost(tbx_serverAddress->text(), PORT);
	btn_connect->setEnabled(false);
	btn_disconnect->setEnabled(true);
}

void WMainWindow::WServerDisconnect() {
	socket->disconnectFromHost();
	lbl_status->setPixmap(QPixmap());
	btn_connect->setEnabled(true);
	btn_disconnect->setEnabled(false);
}

void WMainWindow::WConnected() {
	tbx_logs->append("Connected to " + QString::number(socket->peerAddress().toIPv4Address()));
}

void WMainWindow::WDisconnected() {
	tbx_logs->append("Disconnected from server");
}

void WMainWindow::WReadServer() {
	QDataStream in(socket);

	//messageLength equals 0 when it's the beginning of the message
	if(messageLength == 0) {
		//Return if available data is not enough big
		if(socket->bytesAvailable() < (int)sizeof(quint32))
			return;

		//Read length of message from datastream
		in >> messageLength;
	}

	//Wait until all data is gathered
	if(socket->bytesAvailable() < messageLength)
		return;

	QByteArray frameData;
	in >> frameData;

	QPixmap img;
	img.loadFromData(frameData, "PNG");
	img = img.scaled(lbl_status->width(), lbl_status->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
	lbl_status->setPixmap(img);

	messageLength = 0;
}

void WMainWindow::WErrorSocket(QAbstractSocket::SocketError error) {
	switch (error) {
		case QAbstractSocket::HostNotFoundError:
			tbx_logs->append("Host not found");
			break;
		case QAbstractSocket::ConnectionRefusedError:
			tbx_logs->append("Connection refused");
			break;
		case QAbstractSocket::RemoteHostClosedError:
			tbx_logs->append("Connection closed by peer");
			break;
		default:
			tbx_logs->append(socket->errorString());
	}

	btn_connect->setEnabled(true);
	btn_disconnect->setEnabled(false);
}

void WMainWindow::WLogsAppended() {
	tbx_logs->verticalScrollBar()->setValue(tbx_logs->verticalScrollBar()->maximum());
}
