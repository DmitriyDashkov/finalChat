#include "startscreen.h"
#include "ui_startscreen.h"

StartScreen::StartScreen(std::shared_ptr<Database> dbPtr, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartScreen)
{
    ui->setupUi(this);
    if (dbPtr) {
        m_dbPtr = dbPtr;
    }
    else
        m_dbPtr = make_shared<Database>();
    connect(ui->loginWidget, &LoginForm::registerRequested, this, &StartScreen::setRegistrationForm);
    connect(ui->loginWidget, &LoginForm::accepted, this, &StartScreen::onLoggedIn);
    connect(ui->loginWidget, &LoginForm::rejected, this, &StartScreen::onRejectRequested);
    connect(ui->registerWidget, &RegistrationForm::loginRequested, this, &StartScreen::setLoginForm);
    connect(ui->registerWidget, &RegistrationForm::accepted, this, &StartScreen::onLoggedIn);
    connect(ui->registerWidget, &RegistrationForm::rejected, this, &StartScreen::onRejectRequested);

    ui->loginWidget->setDataBase(m_dbPtr);
    ui->registerWidget->setDataBase(m_dbPtr);
}

StartScreen::~StartScreen()
{
    delete ui;
}

void StartScreen::setLoginForm()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void StartScreen::setRegistrationForm()
{
    ui->stackedWidget->setCurrentIndex(1);
}

uint StartScreen::userID() const
{
    return m_userID;
}

QString StartScreen::userName() const
{
    return m_userName;
}

std::shared_ptr<Database> StartScreen::getDataBase() const
{
    return m_dbPtr;
}

void StartScreen::onLoggedIn(uint userID, QString userName)
{
    m_userID = userID;
    m_userName = userName;
    accept();
}

void StartScreen::onRejectRequested()
{
    reject();
}
