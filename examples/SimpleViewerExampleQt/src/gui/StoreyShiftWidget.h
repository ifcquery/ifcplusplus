/* -*-c++-*- IfcQuery www.ifcquery.com
*
MIT License

Copyright (c) 2017 Fabian Gerold

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once
#include <QtWidgets/QSlider>
#include "ScopedLockAndBlock.h"

class IfcPlusPlusSystem;

class StoreyShiftWidget : public QWidget
{
	Q_OBJECT
public:
	StoreyShiftWidget(IfcPlusPlusSystem* system );
	~StoreyShiftWidget();

	virtual QSize sizeHint() const { return QSize( 250, 40 ); }
	void shiftStoreys();

	IfcPlusPlusSystem* m_system;
	QSlider* m_slide_x;
	QSlider* m_slide_y;
	QSlider* m_slide_z;
	ScopedBlockCount	m_block_counter;

public Q_SLOTS:
	void slotModelLoadingDone();
	void slotResetSliders();
	void slotSlideX(int);
	void slotSlideY(int);
	void slotSlideZ(int);
};
