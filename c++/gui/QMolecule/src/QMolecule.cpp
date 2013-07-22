/*Author: Diego Saldana Miranda
LinkedIn: http://lnkd.in/b2kTzS*/

#include <QMolecule.h>

QMolecule::QMolecule(QWidget* parent) : QWidget(parent) {
	this->molecule = OpenBabel::OBMol();
	this->setMinimumSize(200, 200);
	QSizePolicy size_policy = QSizePolicy(QSizePolicy::Minimum, 
	QSizePolicy::Minimum);
	this->setSizePolicy(size_policy);
}

void QMolecule::drawFromFormat(QString keyword, const char* format) {
	this->molecule = OpenBabel::OBMol();
	OpenBabel::OBConversion conversion = OpenBabel::OBConversion();
	conversion.SetInFormat(format);
	std::string keyword_string = keyword.toStdString();
	conversion.ReadString(&(this->molecule), keyword_string);
	this->generate2DCoodinates();
	//TODO: Catch possible exceptions.
	this->update();
}

void QMolecule::drawFromSMILES(QString smiles) {
	const char* format = "SMI";
	this->drawFromFormat(smiles, format);
}

void QMolecule::drawFromSMILES(QTableWidgetItem* item) {
	QString smiles = item->text();
	this->drawFromSMILES(smiles);
}

void QMolecule::generate2DCoodinates() {
    OpenBabel::OBOp* generator = OpenBabel::OBOp::FindType("gen2D");
    generator->Do(&(this->molecule));
}
	
void QMolecule::paintEvent(QPaintEvent* paint_event) {
	QPainter qpainter(this);
	qpainter.setRenderHint(QPainter::Antialiasing);
	QOBPainter* ob_painter = new QOBPainter(&qpainter);
	OpenBabel::OBDepict ob_depict = OpenBabel::OBDepict(ob_painter);
	ob_depict.DrawMolecule(&(this->molecule));
	delete ob_painter;
}
