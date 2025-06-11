#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include "MainWindow.h"
#include <QDialog>
#include <QSpinBox>
#include <QPushButton>
#include <QColor>
#include <QColorDialog>

class SettingsDialog : public QDialog {
public:
    explicit SettingsDialog(int currentBufferTime, QColor color, QWidget* parent = nullptr);
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


