//
// (c) Cindysoft, 2000
// ~~~~~~~~~~~~~~~~~~~

#include "Canvas.h"
#include "Tools.h"

void Win::Font::Stock::GetTextSize( int &width, int &height )
{
	Win::DisplayCanvas canvas;
	Font::Holder font(canvas, *this);
	TEXTMETRIC metric;
	::GetTextMetrics(canvas, &metric);
	width = metric.tmAveCharWidth;
	height = metric.tmHeight + metric.tmExternalLeading;
}
