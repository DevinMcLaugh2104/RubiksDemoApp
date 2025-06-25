#include "PenaltyDialog.h"

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

}

void PenaltyDialog::removePenalty(QVector<double> param) {

}

void PenaltyDialog::add2SecPenalty(QVector<double> param) {

}

void PenaltyDialog::addDNFPenalty(QVector<double> param) {

}
