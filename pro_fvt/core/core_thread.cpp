/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "core_thread.h"

Core_Thread::Core_Thread(QObject *parent)
    : Core_SerialNumber{parent}
{
    Ssdp_Core::bulid(this);
}

QStringList Core_Thread::getFs()
{
    QString dir = "usr/data/clever/cfg/";
    FileMgr::build().mkpath(dir); QStringList fs; fs << "usr/data/clever/ver.ini";
    fs << dir+"alarm.cfg" << dir+"devParam.ini" << dir+"cfg.ini" << dir+"logo.png";
    fs << dir+"inet.ini" << dir+"alarm.df" << dir+"snmpd.conf" << dir+"mac.ini";
    return fs;
}

bool Core_Thread::fsCheck()
{
    bool ret = true;
    QStringList fs = getFs();
    foreach (const auto fn, fs) {
        if(!QFile::exists(fn)) {
            ret = false;
            emit msgSig(tr("文件未找到")+fn, ret);
        }
    } if(ret) emit msgSig(tr("文件未检查 OK!"), ret);
    return ret;
}


bool Core_Thread::searchDev()
{
    bool ret = true; if(m_ips.isEmpty()) {
        Ssdp_Core *ssdp = Ssdp_Core::bulid(this);
        cm_mdelay(15); QStringList ips = ssdp->searchAll();
        QString str = tr("未找到任何目标设备");
        if(ips.size()) str = tr("已找到%1个设备").arg(ips.size());
        else {ret = false;} m_ips = ips;
        emit msgSig(str, ret);
    }
    return ret;
}

void Core_Thread::timeSync()
{
    QString fmd = "yyyy-MM-dd hh:mm:ss";
    QString t = QTime::currentTime().toString(fmd);
    emit msgSig(tr("时间设置:")+t, true);
    Core_Http *http = Core_Http::bulid(this);
    sCfgItem it; it.type = 43; it.fc =1;
    http->setting(it, t);
}

bool Core_Thread::workDown(const QString &ip)
{
    bool res = true;
    emit msgSig(tr("目标设备:")+ip, true);
    Core_Http *http = Core_Http::bulid(this);
    http->initHost(ip); QStringList fs = getFs();
    timeSync(); foreach (const auto fn, fs) {
        bool ret = http->uploadFile(fn);
        if(!ret) res = false;
        emit msgSig(fn, ret);
        cm_mdelay(120);
    }

    if(res) {
        emit msgSig(tr("重启主程序，设备有响声"), true);
        http->execute("killall cores");
    }
    return res;
}

void Core_Thread::run()
{
    bool ret = searchDev();
    if(ret && fsCheck()) {
        foreach (const auto &ip, m_ips) {
            ret = workDown(ip); cm_mdelay(10);
            emit finshSig(ret, ip+" ");
        }
    } emit overSig();
}
