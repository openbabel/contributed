#include <QApplication>
#include <QHBoxLayout>
#include <QMolecule.h>
#include <QString>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QWidget>

void addItem(const int row, const char* smiles, 
QTableWidget* table_widget)
{
	QString qstring = QString::fromStdString(smiles);
	QTableWidgetItem* item = new QTableWidgetItem(qstring, 
	QTableWidgetItem::Type);
	table_widget->setItem(row, 0, item);
}

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QWidget* hbox_widget = new QWidget();
	QHBoxLayout* hbox_layout = new QHBoxLayout(hbox_widget);
	QTableWidget* table_widget = new QTableWidget(hbox_widget);
	table_widget->setColumnCount(1);
	table_widget->setRowCount(4);
	addItem(0, "Cc1ccccc1C", table_widget);
	addItem(1, "Cc1cc(C)ccc1", table_widget);
	addItem(2, "CCO", table_widget);
	addItem(3, "CC(=O)C", table_widget);
	table_widget->resizeRowsToContents();
	table_widget->resizeColumnsToContents();
	hbox_layout->addWidget(table_widget);
	QMolecule* molecule = new QMolecule(hbox_widget);
	
	molecule->connect(table_widget, 
	SIGNAL(itemPressed(QTableWidgetItem*)), 
	molecule, 
	SLOT(drawFromSMILES(QTableWidgetItem*)));
	
	hbox_layout->addWidget(molecule);
	hbox_widget->setLayout(hbox_layout);
	hbox_widget->show();
	return app.exec();
}
