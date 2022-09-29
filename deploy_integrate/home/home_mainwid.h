#ifndef HOME_MAINWID_H
#define HOME_MAINWID_H

#include <QWidget>
#include "home_workwid.h"
#include "backcolourcom.h"

namespace Ui {
class Home_MainWid;
}

class Home_MainWid : public QWidget
{
    Q_OBJECT

public:
    explicit Home_MainWid(QWidget *parent = nullptr);
    ~Home_MainWid();

protected:
    void initWid();

private slots:
    void onStart();
    void onDown(const QString &msg);

private:
    Ui::Home_MainWid *ui;
    Home_WorkWid *mWorkWid;
    //Td_MainWid *mTabWid;
    int mId = 0;
};

#endif // HOME_MAINWID_H
