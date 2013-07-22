/*Author: Diego Saldana Miranda
LinkedIn: http://lnkd.in/b2kTzS*/

#ifndef QOBPAINTER_H
#define QOBPAINTER_H

#include <QBrush>
#include <QColor>
#include <QFont>
#include <QFontMetrics>
#include <QPaintDevice>
#include <QPainter>
#include <QPen>
#include <QPolygon>
#include <QString>
#include <QWidget>
#include <openbabel/depict/painter.h>
#include <algorithm>
#include <string>
#include <utility>
#include <vector>

class QOBPainter : public OpenBabel::OBPainter {
public:
	QOBPainter(QPainter* painter);
	void NewCanvas(double width, double height);
	bool IsGood() const;
	void SetFontFamily(const std::string& fontFamily);
	void SetFontSize(int pointSize);
	void SetFillColor(const OpenBabel::OBColor& color);
	void SetPenColor(const OpenBabel::OBColor& color);
	void SetPenWidth(double width);
	void DrawLine(double x1, double y1, double x2, double y2);
	void DrawCircle(double x, double y, double r);
	void DrawPolygon(const std::vector<std::pair<double, double> >& points);
	void DrawText(double x, double y, const std::string& text);
	OpenBabel::OBFontMetrics GetFontMetrics(const std::string& text);
private:
	QPainter* qpainter;
};

#endif
