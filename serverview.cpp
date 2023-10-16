#include "serverview.h"
#include "ui_serverview.h"
#include <QDebug>
#include <QMessageBox>

ServerView::ServerView(std::shared_ptr<Database> dbPtr,
                       QVector<MainWindow*> *clientWindows,
                       QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ServerView),
    m_dbPtr(dbPtr),
    m_clientWindows(clientWindows)
{
    ui->setupUi(this);
    setModal(true);
    setConnect();
    fillingUsersComboBox();     //заполнение комбобокса юзерами
    setCheckBoxBun();
}

ServerView::~ServerView()
{
    delete ui;
}

void ServerView::setConnect()
{
    connect(ui->usersComboBox, SIGNAL(currentTextChanged(const QString &)),
            this, SLOT(updateTextBrowsers(const QString &)));
}

void ServerView::setStatusBun(bool status)
{
    m_dbPtr->changeUserStatusBun(getCurrentUser().getuserName(), status);
}

void ServerView::setCheckBoxBun()
{
    ui->isBunCheckBox->setChecked(getCurrentUser().isBun());
}

User ServerView::getCurrentUser()
{
    User u;
    QString currentUserName = ui->usersComboBox->currentText();
    for(uint i=0; i<m_dbPtr->getUsers().size(); i++) {
        if(m_dbPtr->getUsers().at(i).getuserName() == currentUserName.toStdString()) {
            u = m_dbPtr->getUsers().at(i);
        }
    }
    return u;
}

void ServerView::updateTextBrowsers(const QString &currentUser)
{
    ui->toAllTextBrowser->clear();
    ui->sentPrivateTextBrowser->clear();
    ui->recivedPrivateTextBrowser->clear();
    vector<string> vecChat = m_dbPtr->getChatMessages();            //вектор всех общих сообщений
    vector<Message> vecPrivate = m_dbPtr->getPrivateMessage();      //вектор всех личных сообщений

    QString chatMessages{""};               //общие сообщения текущего пользователя
    QString privateSentMessages{""};        //отправленные личные сообщения текущего пользователя
    QString privateReceivedMessages{""};    //входящие личные сообщения текущего пользователя
//--------
    QString metka = "<" + currentUser + ">";
    for(uint i=0; i<vecChat.size(); i++) {
        QString str = vecChat.at(i).c_str();
        if(str.contains(metka)) {
            chatMessages.append(str);
            chatMessages.append('\n');
        }
    }
//--------
    for(uint i=0; i<vecPrivate.size(); i++) {
        if(vecPrivate.at(i).getSender().c_str() == currentUser) {
            privateSentMessages.append("to <");
            privateSentMessages.append(QString::fromStdString(m_dbPtr->getUserName(vecPrivate.at(i).getDest())));
            privateSentMessages.append(">: ");
            privateSentMessages.append(vecPrivate.at(i).getText().c_str());
            privateSentMessages.append('\n');
        }
        else if(m_dbPtr->getUserName(vecPrivate.at(i).getDest()).c_str() == currentUser) {
            privateReceivedMessages.append("from <");
            privateReceivedMessages.append(vecPrivate.at(i).getSender().c_str());
            privateReceivedMessages.append(">: ");
            privateReceivedMessages.append(vecPrivate.at(i).getText().c_str());
            privateReceivedMessages.append('\n');
        }
    }

    ui->toAllTextBrowser->setText(chatMessages);
    ui->sentPrivateTextBrowser->setText(privateSentMessages);
    ui->recivedPrivateTextBrowser->setText(privateReceivedMessages);
}

void ServerView::fillingUsersComboBox()
{
    ui->usersComboBox->clear();
    vector<string> userList = m_dbPtr->getUserList();
    for(uint i=0; i< userList.size(); i++) {
        ui->usersComboBox->addItem(userList.at(i).c_str());
    }
}

void ServerView::on_pushButton_clicked()
{
    if(ui->usersComboBox->currentText() != "") {
        if(ui->isBunCheckBox->isChecked()) {
            setStatusBun(true);
            QMessageBox::information(this,"Information",
                                "User <" + ui->usersComboBox->currentText()+
                                "> has been banned and will not be able to write messages. Viewing messages remains available");
        }

        else {
            setStatusBun(false);
            QMessageBox::information(this,"Information",
                                "User <" + ui->usersComboBox->currentText()+
                                "> has been unbanned and can write messages");
        }

    }
}

void ServerView::on_usersComboBox_currentIndexChanged(int index)
{
    setCheckBoxBun();
}

void ServerView::on_pushButton_2_clicked()
{
    close();
}

void ServerView::on_deleteUserPushButton_clicked()
{
    QMessageBox *pmbDeleleteUser =
            new QMessageBox(QMessageBox::Question,
                            "Delete user",
                            "You are going to delete the user <" +
                            ui->usersComboBox->currentText() +
                            ">. Do you want to continue?",
                            QMessageBox::Yes | QMessageBox::No);
    int n = pmbDeleleteUser->exec();
    if(n == QMessageBox::Yes) {
        for(int i=0; i<m_clientWindows->size(); i++) {
            if(m_clientWindows->at(i)->userName() == ui->usersComboBox->currentText()) {
                m_clientWindows->at(i)->close();
                m_clientWindows->remove(i);
                break;
            }
        }
        m_dbPtr->deleteUser(getCurrentUser().getuserName());                                //удаление из базы
        fillingUsersComboBox();
    }
    if(n == QMessageBox::No) {
        close();
    }
}
