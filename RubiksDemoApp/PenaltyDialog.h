#pragma once
#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>

class PenaltyDialog : public QDialog {
    Q_OBJECT
public:
    explicit PenaltyDialog(QWidget* parent = nullptr);

    enum PenaltyType { None, Plus2, DNF };
    PenaltyType selectedPenalty = None;

    QPushButton* removePenaltyBtn;
    QPushButton* add2SecBtn;
    QPushButton* addDNFBtn;
};


