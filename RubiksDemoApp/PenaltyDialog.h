#pragma once
#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

class PenaltyDialog :  public QDialog
{
public:
	explicit PenaltyDialog(QWidget* parent);

	QPushButton* mainButton;
	QPushButton* removePenaltyBtn;
	QPushButton* add2SecBtn;
	QPushButton* addDNFBtn;

private:

};

