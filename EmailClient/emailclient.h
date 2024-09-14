#ifndef EMAILCLIENT_H
#define EMAILCLIENT_H

#include "login.h"
#include "ui_login.h"
#include "sendtouser.h"
#include "ui_sendtouser.h"
#include "createcontact.h"
#include "ui_createcontact.h"
#include "../src/SmtpMime"

#include <QMainWindow>
#include <QGraphicsBlurEffect>
#include <QDebug>
#include <QTimer>
#include <QDateTime>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QListWidgetItem>

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
    void onSendEmailClick();
    void sendEmailToUser();
    void saveIntoFile(int fileType);
    void onCreateContactClick();
    void addToContacts();
    void recentWidgetItemDoubleClicked(QListWidgetItem *item);
    void contactsWidgetItemDoubleClicked(QListWidgetItem *item);
    void clearRecent();
    void onLogoutClick();

private:
    void setupLoginDialog();
    void setsBlurToMainWindow();
    void connectToSmtp();
    void switchToEmailClientWindow();
    void createUserStruct();
    QStringList stringToRecipient(QString user);
    void addToRecent(QStringList &users);
    void writeJsonIntoFile(QFile &file, int fileType);
    void loadRecentFromFile();
    void loadContactsFromFile();
    void loadJsonIntoListWidget(QByteArray &recentFileData, int fileType);
    void setDeleteRecentButtonIcon();
    void setLogoutButtonIcon();

signals:
    void listWidgetChanged(int recentFile);

private:
    Ui::EmailClient *ui;

    QGraphicsBlurEffect *m_blurEffect;
    Login *m_loginWidget;
    SmtpClient *m_smtp;
    SendToUser *m_sendToUser;
    User m_user;
    CreateContact *m_createContact;

    enum UserData
    {
        Email,
        Name
    };

    enum Contacts
    {
        Recent,
        Contact
    };

    enum FileType
    {
        RecentFile,
        ContactsFile
    };
};
#endif // EMAILCLIENT_H
