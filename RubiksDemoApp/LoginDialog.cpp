#include "LoginDialog.h"
#include <QMessageBox>

LoginDialog::LoginDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("Login");
    setModal(true);

    auto* layout = new QVBoxLayout(this);

    // Username
    layout->addWidget(new QLabel("Username:"));
    usernameLineEdit = new QLineEdit(this);
    layout->addWidget(usernameLineEdit);

    // Password
    layout->addWidget(new QLabel("Password:"));
    passwordLineEdit = new QLineEdit(this);
    passwordLineEdit->setEchoMode(QLineEdit::Password);
    layout->addWidget(passwordLineEdit);

    // Buttons
    auto* buttonLayout = new QHBoxLayout;
    loginButton = new QPushButton("Login", this);
    cancelButton = new QPushButton("Cancel", this);
    buttonLayout->addWidget(loginButton);
    buttonLayout->addWidget(cancelButton);
    layout->addLayout(buttonLayout);

    // Connect signals
    connect(loginButton, &QPushButton::clicked, this, &LoginDialog::handleLogin);
    connect(cancelButton, &QPushButton::clicked, this, &LoginDialog::handleCancel);
}

LoginDialog::~LoginDialog() {}

QString LoginDialog::username() const {
    return usernameLineEdit->text();
}

QString LoginDialog::password() const {
    return passwordLineEdit->text();
}

void LoginDialog::handleLogin() {
    if (username().isEmpty() || password().isEmpty()) {
        QMessageBox::warning(this, "Login Failed", "Please enter both username and password.");
        return;
    }

    // Add actual auth logic here later
    accept();
}

void LoginDialog::handleCancel() {
    reject();
}


