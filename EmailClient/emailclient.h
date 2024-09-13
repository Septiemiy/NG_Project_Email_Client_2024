#ifndef EMAILCLIENT_H
#define EMAILCLIENT_H

#include "login.h"
#include "ui_login.h"
#include "sendtouser.h"
#include "ui_sendtouser.h"
#include "../src/SmtpMime"

#include <QMainWindow>
#include <QGraphicsBlurEffect>
#include <QDebug>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
class EmailClient;
}
QT_END_NAMESPACE

struct User
{
    QString name;
    QString email;
    QString appPassword;
};

class EmailClient : public QMainWindow
{
    Q_OBJECT

public:
    EmailClient(QWidget *parent = nullptr);
    ~EmailClient();

private slots:
    void checkUserLoginData();

private:
    void setupLoginDialog();
    void setsBlurToMainWindow();
    void connectToSmtp();
    void switchToEmailClientWindow();
    void createUserStruct();
    void onSendEmailClick();
    void sendEmailToUser();

private:
    Ui::EmailClient *ui;

    QGraphicsBlurEffect *m_blurEffect;
    Login *m_loginWidget;
    SmtpClient *m_smtp;
    SendToUser *m_sendToUser;
    User m_user;
};
#endif // EMAILCLIENT_H
