/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "remote_recvwid.h"
#include "ui_remote_recvwid.h"
#include "backcolourcom.h"
#include <QDateTime>

Remote_RecvWid::Remote_RecvWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Remote_RecvWid)
{
    ui->setupUi(this);
    groupBox_background_icon(this);
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(QHostAddress::Any, 21437, QUdpSocket::ShareAddress);
    connect(udpSocket, &QUdpSocket::readyRead, this, &Remote_RecvWid::udpRecvSlot);
}

Remote_RecvWid::~Remote_RecvWid()
{
    delete ui;
}

void Remote_RecvWid::startSlot()
{
    ui->textEdit->clear();  mCnt = 0;
}

void Remote_RecvWid::insertText(const QString &dst, const QString &str)
{
    ui->textEdit->moveCursor(QTextCursor::Start);
    QString t = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz\t");
    if(str.size()>2) ui->textEdit->insertPlainText(t + dst+"\t: "+str+"\n");
//    QTextCursor cursor = ui->textEdit->textCursor();
//    cursor.movePosition(QTextCursor::End);
//    ui->textEdit->setTextCursor(cursor);
    if(mCnt++ > 5000) startSlot();
}

void Remote_RecvWid::udpRecvSlot()
{
    QByteArray datagram; QHostAddress host;
    while(udpSocket->hasPendingDatagrams()) {
        datagram.resize(int(udpSocket->pendingDatagramSize()));
        int ret = udpSocket->readDatagram(datagram.data(), datagram.size(), &host);
        if(ret > 0) insertText(host.toString().remove("::ffff:"), QString(datagram).remove("::ffff:"));
        else qCritical() << udpSocket->errorString();
    }
}
