#include "SettingsDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <qcolordialog.h>

SettingsDialog::SettingsDialog(int currentBufferTime, QColor color, QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("Settings");
    setFixedSize(250, 120);

    QVBoxLayout* layout = new QVBoxLayout(this);

    QHBoxLayout* bufferLayout = new QHBoxLayout();
    QLabel* bufferLabel = new QLabel("Hold Buffer (ms):", this);
    bufferSpinBox = new QSpinBox(this);
    bufferSpinBox->setRange(0, 3000);
    bufferSpinBox->setSingleStep(100);
    bufferSpinBox->setValue(currentBufferTime); 
    bufferLayout->addWidget(bufferLabel);
    bufferLayout->addWidget(bufferSpinBox);
    layout->addLayout(bufferLayout);

    QHBoxLayout* colorLayout = new QHBoxLayout();
    QLabel* colorLabel = new QLabel("Background Color:", this);
    colorButton = new QPushButton("Choose Color", this);
    connect(colorButton, &QPushButton::clicked, this, &SettingsDialog::chooseColor);
    colorLayout->addWidget(colorLabel);
    colorLayout->addWidget(colorButton);
    layout->addLayout(colorLayout);

    QPushButton* applyButton = new QPushButton("Apply", this);
    layout->addWidget(applyButton);
    connect(applyButton, &QPushButton::clicked, this, &SettingsDialog::applySettings);
}


int SettingsDialog::getBufferTime() const {
    return bufferSpinBox->value();
}

QColor SettingsDialog::getSelectedColor() const {
    return selectedColor;
}

void SettingsDialog::applySettings() {
    accept();  
}

void SettingsDialog::chooseColor() {
    QColor color = QColorDialog::getColor(selectedColor, this, "Choose Background Color");
    if (color.isValid()) {
        selectedColor = color;
        colorButton->setStyleSheet(QString("background-color: %1").arg(color.name()));
    }
}
