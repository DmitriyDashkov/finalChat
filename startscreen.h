#ifndef STARTSCREEN_H
#define STARTSCREEN_H

#include <QDialog>
#include <memory>
#include "database.h"

namespace Ui {
class StartScreen;
}

class StartScreen : public QDialog
{
    Q_OBJECT

public:
    explicit StartScreen(std::shared_ptr<Database> dbPtr = nullptr, QWidget *parent = nullptr);
    ~StartScreen();
    void setLoginForm();
    void setRegistrationForm();

    uint userID() const;

    QString userName() const;

    std::shared_ptr<Database> getDataBase() const;

private:
    Ui::StartScreen *ui;
    uint m_userID;
    QString m_userName;
    std::shared_ptr<Database> m_dbPtr;

public slots:
    void onLoggedIn(uint userId, QString userName);
    void onRejectRequested();
};

#endif // STARTSCREEN_H
