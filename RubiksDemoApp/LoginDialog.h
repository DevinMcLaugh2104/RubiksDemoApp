#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFile>

class LoginDialog : public QDialog {
    Q_OBJECT

public:
    explicit LoginDialog(QWidget* parent = nullptr);
    ~LoginDialog();

    QString username() const;
    QString password() const;

private slots:
    void handleLogin();
    void handleCancel();

private:
    QLineEdit* usernameLineEdit;
    QLineEdit* passwordLineEdit;
    QPushButton* loginButton;
    QPushButton* cancelButton;
};


