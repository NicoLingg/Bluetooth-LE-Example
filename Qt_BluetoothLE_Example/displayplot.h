/****************************************************************************
**
** This file is part of the Bluetooth LE UART example written by Nico Lingg
**
**
** Author: Nico Lingg
** VERSION: 0.1
** Date: 09/12/2016 (DD/MM/YYYY)
**
**
** Permission is hereby granted, free of charge, to any person obtaining a copy of
** this software and associated documentation files (the "Software"), to deal in
** the Software without restriction, including without limitation the rights to
** use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
** the Software, and to permit persons to whom the Software is furnished to do so,
** subject to the following conditions:
**
** The above copyright notice and this permission notice shall be included in all
** copies or substantial portions of the Software.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
****************************************************************************/

#ifndef DISPLAYPLOT_H
#define DISPLAYPLOT_H

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_scale_draw.h>
#include <qwt_plot_curve.h>
#include <QTime>


class displayplot : public QwtPlot
{
    Q_OBJECT


public:
    displayplot(QString title, QString unit);
    void updatePlot(float data);
    QString getTitle(){return m_sTitle;}
    QString getUnit(){return m_sUnit;}


private:
    QString m_sTitle;
    QString m_sUnit;


    QwtPlotGrid *m_pGrid;
    QwtPlotCurve *m_pCurve;
    QPolygonF m_points;
    QTime m_time;


    // Time for current and previous sample in msecs since the start of the day
    int m_iCurrentSampleMS;
    int m_iPreviousSampleMS;

    qint32 m_iElapsedTimeMS;
};

#endif // DISPLAYPLOT_H
