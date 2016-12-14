#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), m_LED_ON(false)
{
    ui->setupUi(this);

    /* Create UI */
    createPlotGroupBox();
    createComboGroupBox();
    createValueDisplayGroupBox();
    createMenuGroupBox();
    createStatusBar();

    /* Main Layout */
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(m_plotGroupBox);
    mainLayout->addWidget(m_valueDisplayGroupBox);
    mainLayout->addWidget(m_comboGroupBox);
    mainLayout->addWidget(m_menuGroupBox);

    /* Set central widget */
    ui->centralWidget->setLayout(mainLayout);
    ui->centralWidget->setMaximumSize(500,900);
    //ui->centralWidget->setMinimumSize(500,900);
    setWindowTitle(tr("Bluetooth LE UART Example"));

    /* Signal and Slots */
    /* Search Button */
    connect(m_pBSearch, SIGNAL(clicked()),&m_bleConnection, SLOT(startScan()));
    /* Connect Button */
    connect(m_pBConnect,SIGNAL(clicked()), this, SLOT(connectDevice()));
    /* LED Button */
    connect(m_pBLED, SIGNAL(clicked()),this,SLOT(toggleLED()));
    /* Send Data Button */
    connect(m_pBsend,SIGNAL(clicked()),this, SLOT(sendData()));
    /* Bleutooth States */
    connect(&m_bleConnection, SIGNAL(changedState(bluetoothleUART::bluetoothleState)),this,SLOT(changedState(bluetoothleUART::bluetoothleState)));
}

void MainWindow::changedState(bluetoothleUART::bluetoothleState state){

    qDebug() << state;

    switch(state){

        case bluetoothleUART::Scanning:
        {
            m_cbDevicesFound.clear();

            m_pBConnect->setEnabled(false);
            m_pBSearch->setEnabled(false);
            m_cbDevicesFound.setEnabled(false);

            m_spinner->start();
            showStatusBarMessage("Searching for low energy devices...");
            break;
        }


        case bluetoothleUART::ScanFinished:
        {
            m_bleConnection.getDeviceList(m_qlFoundDevices);

            if(!m_qlFoundDevices.empty()){

                for (int i = 0; i < m_qlFoundDevices.size(); i++)
                {
                   m_cbDevicesFound.addItem(m_qlFoundDevices.at(i));
                   qDebug() << m_cbDevicesFound.itemText(i);
                }

                /* Initialise Slot startConnect(int) -> button press m_pBConnect */
                connect(this, SIGNAL(connectToDevice(int)),&m_bleConnection,SLOT(startConnect(int)));

                m_pBConnect->setEnabled(true);
                m_pBSearch->setEnabled(true);
                m_cbDevicesFound.setEnabled(true);


                showStatusBarMessage("Please select BLE device");
                m_spinner->stop();

            }
            else
            {
                showStatusBarMessage("No Low Energy devices found");
                m_pBSearch->setEnabled(true);
                m_spinner->stop();
            }

            break;
        }

        case bluetoothleUART::Connecting:
        {
            m_pBConnect->setEnabled(false);
            m_pBSearch->setEnabled(false);
            m_cbDevicesFound.setEnabled(false);

            showStatusBarMessage("Connecting to device...");
            m_spinner->start();
            break;
        }
        case bluetoothleUART::Connected:
        {
            showStatusBarMessage("Device connected. Looking for service...");
            m_spinner->start();
            break;
        }
        case bluetoothleUART::ServiceFound:
        {
            showStatusBarMessage("Service found");
            break;
        }
        case bluetoothleUART::AcquireData:
        {
            m_pBLED ->setEnabled(true);
            m_pBsend->setEnabled(true);
            m_lineEdit->setEnabled(true);

            /* Initialise Slot DataHandler(QString) - gets new data */
            connect(&m_bleConnection, SIGNAL(newData(QString)), this, SLOT(DataHandler(QString)));
            showStatusBarMessage("Aquire data");
            break;
        }
        default:
            //nothing for now
        break;


    }


}

void MainWindow::DataHandler(const QString &s){

    /* We get a string "TempNNNNHumNNNNXYZ" (N: Number from 1 to 9)  */
    QRegularExpression re ("Temp(?<temp>-?\\d+)Hum(?<hum>\\d+)XYZ");
    QRegularExpressionMatch match = re.match(s);

    if (match.hasMatch())
    {
            QString matchedTempValue = match.captured("temp");
            m_fTemperature = (matchedTempValue.toDouble()/100);

            QString matchedHumValue = match.captured("hum");
            m_fHumidity = (matchedHumValue.toDouble()/100);

            qDebug() << "Temp: " << m_fTemperature << "\tHum: " << m_fHumidity;

            m_lTemperature->setText(QString::number(m_fTemperature));
            m_lHumidity->setText(QString::number(m_fHumidity));

            m_tempDisplayPlot->updatePlot(m_fTemperature);
            m_humDisplayPlot->updatePlot(m_fHumidity);
      }


}

void MainWindow::toggleLED(){

    /* If LED is off, send LED_ON to device and set LED flag to true */
    if (m_LED_ON == false){
    m_bleConnection.writeData("LED_ON");
    m_LED_ON = true;
    m_pBLED->setText("Turn OFF LED");
    return;
    }

    /* If LED is ON, send LED_OFF to device and set LED flag to false */
    if(m_LED_ON == true){
    m_bleConnection.writeData("LED_OFF");
    m_LED_ON = false;
    m_pBLED->setText("Turn ON LED");
    return;
    }
}

void MainWindow::sendData(){

    m_bleConnection.writeData((QString)m_lineEdit->text());
}

MainWindow::~MainWindow(){

    delete ui;
}


void MainWindow::showStatusBarMessage(const QString &text){

    m_status->setText(text);
}


void MainWindow::connectDevice(){

    emit connectToDevice(m_cbDevicesFound.currentIndex());
}


void MainWindow::createPlotGroupBox(){

    m_tempDisplayPlot = new displayplot("Temperature","degC");
    m_humDisplayPlot = new displayplot("Humidity","%");

    m_plotGroupBox = new QGroupBox(tr("Plots"));
    QVBoxLayout *layout = new QVBoxLayout;

    layout->addWidget(m_tempDisplayPlot);
    layout->addWidget(m_humDisplayPlot);

    m_plotGroupBox->setLayout(layout);
}

void MainWindow::createMenuGroupBox(){

    m_menuGroupBox = new QGroupBox(tr("Menu"));
    QGridLayout *layout = new QGridLayout;

    m_lineEdit = new QLineEdit(this);
    m_pBsend = new QPushButton(this);
    m_pBLED = new QPushButton(this);
    m_pBSearch = new QPushButton(this);
    m_pBConnect = new QPushButton(this);

    /* Init Buttons */
    m_pBLED->setText("Turn On LED");
    m_pBSearch->setText("Search");
    m_pBConnect->setText("Connect");
    m_pBsend->setText("Send");
    m_pBLED->setEnabled(false);
    m_pBConnect->setEnabled(false);
    m_pBsend->setEnabled(false);
    m_lineEdit->setEnabled(false);

    /* Layout */
    layout->addWidget(m_pBsend, 2,0,1,1);
    layout->addWidget(m_lineEdit, 2,1,1,3);
    layout->addWidget(m_pBLED,3,0,1,4);
    layout->addWidget(m_pBSearch,4,0,1,2);
    layout->addWidget(m_pBConnect,4,2,1,2);

    m_menuGroupBox->setLayout(layout);
}


void MainWindow::createComboGroupBox(){

   m_comboGroupBox = new QGroupBox(tr("Devices"));
   QVBoxLayout *layout = new QVBoxLayout;

   /* Layout */
   layout->addWidget( &m_cbDevicesFound);

   m_comboGroupBox->setLayout(layout);
}


void MainWindow::createValueDisplayGroupBox(){

    m_lTemperature = new QLabel;
    m_lHumidity = new QLabel;

    m_valueDisplayGroupBox = new QGroupBox(tr("Current Values"));
    QHBoxLayout *layout = new QHBoxLayout;

    /* Layout */
    layout->addWidget(new QLabel(tr("Temperatur:")));
    layout->addWidget(m_lTemperature,2);
    layout->addWidget(new QLabel(tr("Humidity:")));
    layout->addWidget(m_lHumidity,2);

    m_valueDisplayGroupBox->setLayout(layout);

}

void MainWindow::createStatusBar(){

    m_status = new QLabel(this);
    m_spinner = new WaitingSpinnerWidget(this,false,false);

    QHBoxLayout *layout = new QHBoxLayout();

    QWidget *statusBar = new QWidget();
    statusBar->setLayout(layout);

    /* Initialise spinner/busy indicator */
    m_spinner->setRoundness(70.0);
    m_spinner->setMinimumTrailOpacity(15.0);
    m_spinner->setTrailFadePercentage(70.0);
    m_spinner->setNumberOfLines(8);
    m_spinner->setLineLength(4);
    m_spinner->setLineWidth(2);
    m_spinner->setInnerRadius(2);
    m_spinner->setRevolutionsPerSecond(1);
    m_spinner->setColor(QColor(0, 0, 0));

    /* Layout */
    layout->addWidget(m_status);
    layout->addWidget(m_spinner);
    layout->setSpacing(4);

     /* Add Status Bar */
    ui->statusBar->addWidget(statusBar);

}
