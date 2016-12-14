#include "displayplot.h"



// Helper class for QwtPlot to display time on x-Axis
class TimeScaleDraw:public QwtScaleDraw
{
public:
    TimeScaleDraw(const QTime & base):baseTime(base) {}

virtual QwtText label(double v)const
{
    QTime upTime = baseTime.addMSecs((int)v);
    return upTime.toString();
}
private:
    QTime baseTime;
};


displayplot::displayplot(QString title, QString unit) : m_sTitle(title), m_sUnit(unit)
{

    /* Plot initialisation */
    setAxisScaleDraw(QwtPlot::xBottom,new TimeScaleDraw(QTime::currentTime()));
    setAxisTitle(QwtPlot::xBottom,"Time");
    setAxisTitle(QwtPlot::yLeft, QString(title + " [" + unit + "]"));
    setAxisLabelRotation(QwtPlot::xBottom,-50.0);
    setAxisLabelAlignment(QwtPlot::xBottom,Qt::AlignLeft|Qt::AlignBottom);
    setCanvasBackground( Qt::white );
    setAxisAutoScale(1); // auto scale y axis

    /* Grid initialisation */
    m_pGrid = new QwtPlotGrid();
    m_pGrid->attach(this);

    /* Curve initialisation */
    m_pCurve = new QwtPlotCurve();
    m_pCurve->setPen( Qt::blue, 2 ),
    m_pCurve->setRenderHint( QwtPlotItem::RenderAntialiased, true );
    m_pCurve->attach(this);

    /* Variable initialisation */
    m_iElapsedTimeMS = 0;
    m_iCurrentSampleMS = 0;
    m_iPreviousSampleMS = QTime::currentTime().msecsSinceStartOfDay();

    //resize( 600, 400 );
}

void displayplot::updatePlot(float data){

    m_iCurrentSampleMS = QTime::currentTime().msecsSinceStartOfDay();

    m_iElapsedTimeMS = (m_iCurrentSampleMS-m_iPreviousSampleMS)+m_iElapsedTimeMS;

    m_points << QPointF(m_iElapsedTimeMS,data);
    m_pCurve->setSamples(m_points);
    replot();
    m_iPreviousSampleMS = QTime::currentTime().msecsSinceStartOfDay();
}
