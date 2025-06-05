#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QSpinBox>
#include <QPushButton>
#include "MainWindow.h"

class SettingsDialog : public QDialog {
public:
    explicit SettingsDialog(int currentBufferTime, QWidget* parent = nullptr);
    int getBufferTime() const;
    QColor getSelectedColor() const;

private:
    void applySettings();
    void chooseColor();

    QSpinBox* bufferSpinBox;
    QPushButton* colorButton;
    QColor selectedColor;
};

#endif


