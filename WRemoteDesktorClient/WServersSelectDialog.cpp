#include "WServersSelectDialog.h"

QHash<QString, QHostAddress> servers;
QList<QPushButton *> serversButtons;
QHostAddress selectedServer;

WServerSelectDialog::WServerSelectDialog(QWidget *parent, QHash<QString, QHostAddress> paramServers) : QDialog(parent) {
	vBoxLayout_main = new QVBoxLayout;

	servers = paramServers;
	for(QString server : servers.keys()) {
		QPushButton *btn_server = new QPushButton(server);
		serversButtons.append(btn_server);
		QObject::connect(btn_server, SIGNAL(clicked()), this, SLOT(WServerSelected()));
	}

	setLayout(vBoxLayout_main);
}

WServerSelectDialog::~WServerSelectDialog() {
	for(int i = 0; i < serversButtons.size(); i++) {
		vBoxLayout_main->removeWidget(serversButtons.at(i));
		serversButtons.at(i)->deleteLater();
	}
	serversButtons.clear();
	delete vBoxLayout_main;
}

void WServerSelectDialog::WServerSelected() {
	selectedServer = servers[qobject_cast<QPushButton *>(sender())->text()];
	this->accept();
}

QHostAddress WServerSelectDialog::WGetSelectedServer() {
	return selectedServer;
}
