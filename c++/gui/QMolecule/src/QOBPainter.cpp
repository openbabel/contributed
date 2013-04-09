/*Author: Diego Saldana Miranda
LinkedIn: http://lnkd.in/b2kTzS*/

#include <QOBPainter.h>

QOBPainter::QOBPainter(QPainter* qpainter) {
	this->qpainter = qpainter;
}

void QOBPainter::NewCanvas(double width, double height) {
	QWidget* widget = (QWidget*)this->qpainter->device();
	double scaled_width = widget->width() / width;
	double scaled_height = widget->height() / height;
	double scale = std::min(scaled_width, scaled_height);
	this->qpainter->scale(scale, scale);
}

bool QOBPainter::IsGood() const {
	return true;
}

void QOBPainter::SetFontFamily(const std::string& fontFamily) {
	int i = 0;
	//TODO: Implement!
}

void QOBPainter::SetFontSize(int point_size) {
	QFont font = this->qpainter->font();
	font.setPointSize(point_size);
	this->qpainter->setFont(font);
}

void QOBPainter::SetFillColor(const OpenBabel::OBColor& color) {
	QBrush brush = this->qpainter->brush();
	QColor qcolor = QColor(color.red, color.green, color.blue);
	brush.setColor(qcolor);
	this->qpainter->setBrush(brush);
}

void QOBPainter::SetPenColor(const OpenBabel::OBColor& color) {
	QPen pen = this->qpainter->pen();
	QColor qcolor = QColor(color.red, color.green, color.blue);
	pen.setColor(qcolor);
	this->qpainter->setPen(pen);
}

void QOBPainter::SetPenWidth(double width) {
	QPen pen = this->qpainter->pen();
	pen.setWidth(width);
	this->qpainter->setPen(pen);
}

void QOBPainter::DrawLine(double x1, double y1, double x2, double y2) {
	this->qpainter->drawLine((int)x1, (int)y1, (int)x2, (int)y2);
}

void QOBPainter::DrawCircle(double x, double y, double r) {
	double diameter = r * 2;
	this->qpainter->drawEllipse((int)x, (int)y, (int)diameter, (int)diameter);
}

void QOBPainter::DrawPolygon(
const std::vector<std::pair<double, double> >& points) {
	int size = points.size();
	QPolygon polygon = QPolygon(size);
	for (int index = 0; index < size; index++) {
		double x = points.at(index).first;
		double y = points.at(index).second;
		polygon.setPoint(index, (int)x, (int)y);
	}
	this->qpainter->drawPolyline(polygon);
}

void QOBPainter::DrawText(double x, double y, const std::string& text) {
	QString text_reference = QString::fromStdString(text);
	this->qpainter->drawText((int)x, (int)y, text_reference);
}

OpenBabel::OBFontMetrics QOBPainter::GetFontMetrics(const std::string& text) {
	QFont font = this->qpainter->font();
	int font_size = font.pointSize();
	QFontMetrics metrics = this->qpainter->fontMetrics();
	QString text_reference = QString::fromStdString(text);
	double width = (double)metrics.width(text_reference);
	double height = (double)metrics.height();
	double ascent = (double)metrics.ascent();
	double descent = (double)metrics.descent();
	OpenBabel::OBFontMetrics ob_metrics = OpenBabel::OBFontMetrics();
	ob_metrics.fontSize = font_size;
	ob_metrics.width = width;
	ob_metrics.height = height;
	ob_metrics.ascent = ascent;
	ob_metrics.descent = descent;
	return ob_metrics;
}
