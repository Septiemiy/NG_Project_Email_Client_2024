#ifndef SENDTOUSER_H
#define SENDTOUSER_H

#include <QFrame>
#include <QCloseEvent>
#include <QFileDialog>
#include <QStringListModel>
#include <QMenu>

namespace Ui {
class SendToUser;
}

class SendToUser : public QFrame
{
    Q_OBJECT

public:
    explicit SendToUser(QWidget *parent = nullptr);
    ~SendToUser();

private slots:
    void onClickCancel();
    void addFilePathToAttachmentList();
    void addUsersToUsersList();
    void showContextMenuFiles(const QPoint &pos);
    void showContextMenuUsers(const QPoint &pos);

private:
    void closeEvent(QCloseEvent *event);

public:
    Ui::SendToUser *ui;

private:
    QStringListModel *m_modelFiles;
    QStringListModel *m_modelUsers;
    QStringList m_filePath;
    QStringList m_users;

};

#endif // SENDTOUSER_H
