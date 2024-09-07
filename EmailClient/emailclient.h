#ifndef EMAILCLIENT_H
#define EMAILCLIENT_H

#include "login.h"

#include <QMainWindow>
#include <QGraphicsBlurEffect>

QT_BEGIN_NAMESPACE
namespace Ui {
class EmailClient;
}
QT_END_NAMESPACE

class EmailClient : public QMainWindow
{
    Q_OBJECT

public:
    EmailClient(QWidget *parent = nullptr);
    ~EmailClient();

private:
    void setupLoginDialog();

private:
    Ui::EmailClient *ui;

    QGraphicsBlurEffect *m_blurEffect;
    Login *m_loginWidget;
};
#endif // EMAILCLIENT_H
