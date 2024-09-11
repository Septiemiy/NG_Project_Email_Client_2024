#ifndef SENDTOUSER_H
#define SENDTOUSER_H

#include <QFrame>

namespace Ui {
class SendToUser;
}

class SendToUser : public QFrame
{
    Q_OBJECT

public:
    explicit SendToUser(QWidget *parent = nullptr);
    ~SendToUser();

private:
    Ui::SendToUser *ui;
};

#endif // SENDTOUSER_H
