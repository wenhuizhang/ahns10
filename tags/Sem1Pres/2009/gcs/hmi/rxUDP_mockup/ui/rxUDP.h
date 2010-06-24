
#ifndef _IRIDIUMTESTCTRL_FORM_H_
#define _IRIDIUMTESTCTRL_FORM_H_

#include "ui_rxUDPGUI.h"
#include "../../reuse/AP_HMI.h"

#include <QWidget>
#include <QString>
#include <QMap>
#include <QThread>
#include <QDialog>
// ==================================
#include <QTimer> // ADDED FOR TIMER
#include <QTime>
#include <QCheckBox>
#include <QTextEdit>
#include <../QtNetwork/QUdpSocket> // Added for UDP
#include <../QtNetwork/QAbstractSocket>
// ==================================
#include <fstream>

class QCheckBox;

class rxUDP : public QMainWindow, private Ui::rxSectUDP
{
	Q_OBJECT

public:
	
	rxUDP(QWidget* parent = 0);
	~rxUDP();
		
private slots:

	// BUTTON SLOTS
	void on_closeBtn_clicked();
	void processPendingDatagrams();
	void on_dispClear_clicked();
	void readData();
	void processData();

private:

	QUdpSocket *udpSocket;
	QByteArray datagram;
	char *data;
	uint8_t dataPacket[PACKSIZE];
	uint8_t buffer[PACKSIZE];
	unsigned short buffer_index;
	unsigned short state;
};



#endif

