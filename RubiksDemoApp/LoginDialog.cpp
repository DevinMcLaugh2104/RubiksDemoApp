#include "LoginDialog.h"
#include <QMessageBox>

LoginDialog::LoginDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("Login");
    setModal(true);
    setFixedSize(500, 340);
   
    QFile file("LoginSS.css"); 

    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QString styleSheet = QString::fromUtf8(file.readAll());
        this->setStyleSheet(styleSheet);
        file.close();
    }
    else {
        qDebug() << "Failed to open stylesheet file";
    }

    auto* layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter); 
    layout->setSpacing(10);                
    layout->setContentsMargins(0, 50, 0, 50); 

    // Transparent overlay
    auto* overlay = new QWidget(this);
    overlay->setGeometry(0, 0, width(), height()); 
    overlay->setStyleSheet("background-color: rgba(150, 150, 150, 80);"); 
    overlay->lower();

    // Username group
    auto* usernameLayout = new QVBoxLayout;
    auto* usernameLabel = new QLabel("Username:");
    usernameLabel->setAlignment(Qt::AlignCenter);
    usernameLabel->setFixedSize(70, 20);
    usernameLineEdit = new QLineEdit(this);
    usernameLineEdit->setFixedSize(200, 25);
    usernameLayout->addWidget(usernameLabel);
    usernameLayout->addWidget(usernameLineEdit);
    layout->addLayout(usernameLayout);

    // Password group
    auto* passwordLayout = new QVBoxLayout;
    auto* passwordLabel = new QLabel("Password:");
    passwordLabel->setAlignment(Qt::AlignCenter);
    passwordLabel->setFixedSize(70, 20);
    passwordLineEdit = new QLineEdit(this);
    passwordLineEdit->setFixedSize(200, 25);
    passwordLineEdit->setEchoMode(QLineEdit::Password);
    passwordLayout->addWidget(passwordLabel);
    passwordLayout->addWidget(passwordLineEdit);
    layout->addLayout(passwordLayout);

    // Buttons
    auto* buttonLayout = new QHBoxLayout;
    buttonLayout->setAlignment(Qt::AlignCenter);
    loginButton = new QPushButton("Login", this);
    loginButton->setFixedSize(100, 25);
    cancelButton = new QPushButton("Cancel", this);
    cancelButton->setFixedSize(100, 25);
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


