#ifndef LOGIN_H
#define LOGIN_H

#include <QFrame>

namespace Ui {
class Login;
}

class Login : public QFrame
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

private slots:
    void onClickExit();

private:
    Ui::Login *ui;
};

#endif // LOGIN_H
