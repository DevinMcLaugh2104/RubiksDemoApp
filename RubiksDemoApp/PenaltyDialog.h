#pragma once
#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>

class PenaltyDialog :  public QDialog
{
public:
	explicit PenaltyDialog(QWidget* parent);

	QPushButton* removePenaltyBtn;
	QPushButton* add2SecBtn;
	QPushButton* addDNFBtn;

	void removePenalty(QVector<double> param);
	void add2SecPenalty(QVector<double> param);
	void addDNFPenalty(QVector<double> param);

private:

};

