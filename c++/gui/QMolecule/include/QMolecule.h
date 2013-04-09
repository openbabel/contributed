/*Author: Diego Saldana Miranda
LinkedIn: http://lnkd.in/b2kTzS*/

#ifndef QMOLECULE_H
#define QMOLECULE_H

#include <openbabel/depict/depict.h>
#include <openbabel/mol.h>
#include <openbabel/obconversion.h>
#include <openbabel/op.h>
#include <QOBPainter.h>
#include <QPainter>
#include <QSizePolicy>
#include <QTableWidgetItem>
#include <QWidget>

class QPaintEvent;
class QString;

class QMolecule : public QWidget
{
	Q_OBJECT
public:
	QMolecule(QWidget* parent = 0);
	void drawFromSMILES(QString smiles);
public slots:
	/*Defined a Qt slot for a table widget item selection, other slots 
	with different arguments (QString, etc) may be readily defined.*/
	void drawFromSMILES(QTableWidgetItem* item);
private:
	OpenBabel::OBMol molecule;
	void generate2DCoodinates();
	void drawFromFormat(QString keyword, const char* format);
	void paintEvent(QPaintEvent* paint_event);
};

#endif
