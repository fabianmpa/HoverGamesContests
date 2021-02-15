/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Charts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QSplineSeries>
#include <algorithm>
#include "filter.hpp"
#include "data.hpp"
#include "filter_order1.h"
#include "filter_order5.h"


QT_CHARTS_USE_NAMESPACE

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    /*Normal signal*/
    QSplineSeries *signal = new QSplineSeries();
    signal->setName("Digital Filter assessment");
    /*Append values to unfiltered signal from data array*/
    for(int i=0; i<100; i++)
    {
        signal->append(i,sample_data[i]);
    }

    /*Filtered signal floating point type, create two new splineSeries objects*/
    QSplineSeries *filtered_signal = new QSplineSeries();
    QSplineSeries *filtered_signal_cascade = new QSplineSeries();
    filtered_signal->setName("filtered signal");
    filtered_signal_cascade->setName("filtered signal cascade");
    /*Create two filter objects, the second one will be the cascade of the first*/
    filter<float> f(270.0F);
    filter<float> f_cascade(f.get_result());
    for(int i=0;i<100;i++)
    {
        /*Input the sample data into the first filter*/
        f.new_sample(b,sample_data[i]);
        /*append the result from the first filter*/
        filtered_signal -> append(i, f.get_result());
        /*The result from the first filter is the input to the second filter*/
        f_cascade.new_sample(b1,f.get_result());
        /*append the result from the cascade filter*/
        filtered_signal_cascade -> append(i, f_cascade.get_result());

    }

    /*Filtered Signal integer filter order 1, create a new SplineSeries object*/
    QSplineSeries *signal_filterorder1 = new QSplineSeries();
    signal_filterorder1->setName("signal signal_filterorder1");
    /*Create a new integer filter order 1 object*/
    filter_order1<> f1(270U);
    for(int i=0;i<100;i++)
    {
        /*Input the sample data into the integer filter*/
        f1.new_sample<9,7>(sample_data[i]);
        /*append the result from the integer filter*/
        signal_filterorder1->append(i,f1.get_result());
    }

    /*Filtered Signal integer filter order 5*/
    QSplineSeries *signal_filterorder5 = new QSplineSeries();
    signal_filterorder5->setName("signal signal_filterorder5");
    QSplineSeries *cascade_filterorder5 = new QSplineSeries();
    cascade_filterorder5->setName("signal cascade_filterorder5");
    /*Create two integer filter order 5 objects, the second one will be the cascade of the first*/
    filter_order_5<> f5(270U);
    filter_order_5<> f5_1(f5.get_result());
    for(int i=0;i<100;i++)
    {
        /*Input the sample data into the first filter*/
        f5.new_sample<1,1,2,2,1,1>(sample_data[i]);
        /*append the result from the first filter*/
        signal_filterorder5->append(i,f5.get_result());
        /*The result from the first filter is the input to the second filter*/
        f5_1.new_sample<1,1,2,2,1,1>(f5.get_result());
        /*The result from the first filter is the input to the second filter*/
        cascade_filterorder5->append(i,f5_1.get_result());
    }

    //![3]
    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(signal);
    //chart->addSeries(filtered_signal);
    chart->addSeries(filtered_signal_cascade);
    //chart->addSeries(signal_filterorder1);
    //chart->addSeries(signal_filterorder5);
    chart->addSeries(cascade_filterorder5);
    chart->setTitle("Blue: Normal Signal. Green:Floating point cascade filter order 17. Orange: Integer cascade filter order 5");
    chart->createDefaultAxes();
    chart->axes(Qt::Vertical).first()->setRange(0, 450);
    //![3]

    //![4]
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    //![4]

    //![5]
    QMainWindow window;
    window.setCentralWidget(chartView);
    window.resize(400, 300);
    window.show();
    //![5]

    return a.exec();
}
