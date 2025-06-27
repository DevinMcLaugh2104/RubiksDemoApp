#include "PenaltyDialog.h"
#include <QVBoxLayout>

PenaltyDialog::PenaltyDialog(QWidget* parent) : QDialog(parent) {
    setWindowTitle("Penalty Options");
    setFixedSize(250, 120);

    QVBoxLayout* layout = new QVBoxLayout(this);

    removePenaltyBtn = new QPushButton("No Penalty");
    add2SecBtn = new QPushButton("+2");
    addDNFBtn = new QPushButton("DNF");

    layout->addWidget(removePenaltyBtn);
    layout->addWidget(add2SecBtn);
    layout->addWidget(addDNFBtn);

    connect(removePenaltyBtn, &QPushButton::clicked, this, [this]() {
        selectedPenalty = None;
        accept();
        });

    connect(add2SecBtn, &QPushButton::clicked, this, [this]() {
        selectedPenalty = Plus2;
        accept();
        });

    connect(addDNFBtn, &QPushButton::clicked, this, [this]() {
        selectedPenalty = DNF;
        accept();
        });
}
