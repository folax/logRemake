#include <QDebug>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QFile>
#include <QPair>
#include <QApplication>
#include <QStyleFactory>
#include <QLabel>

#include "logremake.h"

logRemake::logRemake(QWidget *parent) : QDialog(parent), m_settings("Decay", "logRemake")
{
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint & Qt::WindowMinimized);
    setFixedHeight(200);
    resize(200, 181);
    m_strLastOpenPath = "c:\\";
    QApplication::setStyle(new newStyle);

    //variables
    m_strPathToSaveFile = "c:\\";

    //label
    m_pLblCaption = new QLabel(tr("HUATO log remaker"));
    m_pLblCaption->setAlignment(Qt::AlignCenter);
    m_pLblFileName = new QLabel(tr(":"));
    m_pLblFileDestination = new QLabel(tr(":"));
    m_pLblStatus = new QLabel(tr("Status: "));

    //buttons
    m_pBtnLoadFile = new QPushButton(tr("1) Load file"));
    m_pBtnDestination = new QPushButton(tr("2) Choose destination"));
    m_pBtnConvertFile = new QPushButton(tr("3) Convert file"));
    m_pBtnClose = new QPushButton("Close_Exit");
    m_pBtnConvertFile->setEnabled(false);
    m_pBtnDestination->setEnabled(false);

    //btn layout
    m_pBtnLayout = new QVBoxLayout();
    m_pBtnLayout->addWidget(m_pLblCaption);
    m_pBtnLayout->addWidget(m_pLblFileName);
    m_pBtnLayout->addWidget(m_pLblFileDestination);
    m_pBtnLayout->addWidget(m_pBtnLoadFile);
    m_pBtnLayout->addWidget(m_pBtnDestination);
    m_pBtnLayout->addWidget(m_pBtnConvertFile);
    m_pBtnLayout->addWidget(m_pBtnClose);
    m_pBtnLayout->addWidget(m_pLblStatus);

    //main layout
    m_pMainLayout = new QVBoxLayout(this);
    m_pMainLayout->addLayout(m_pBtnLayout);
    setLayout(m_pMainLayout);

    //connect
    connect(m_pBtnLoadFile, &QPushButton::clicked, this, &logRemake::loadFile);
    connect(m_pBtnDestination, &QPushButton::clicked, this, &logRemake::saveFileTo);
    connect(m_pBtnConvertFile, &QPushButton::clicked, this, &logRemake::readDataFromFile);
    connect(m_pBtnClose, &QPushButton::clicked, this, &logRemake::close);
    readSettings();
}

void logRemake::readSettings()
{
    m_settings.beginGroup("/logRemakeSettings");
    int nWidth = m_settings.value("/Width", width()).toInt();
    int nHeight = m_settings.value("/Height", height()).toInt();
    m_strPathToSaveFile = m_settings.value("/LastSaveToFile", "c:\\").toString();
    m_strLastOpenPath = m_settings.value("/LastOpenFilePath", "c:\\").toString();
    resize(nWidth, nHeight);
    m_settings.endGroup();
}

void logRemake::writeSettings()
{
    m_settings.beginGroup("/logRemakeSettings");
    m_settings.setValue("/Width", width());
    m_settings.setValue("/Height", height());
    m_settings.setValue("/LastSaveToFile", m_strPathToSaveFile + "/");
    m_settings.setValue("/LastOpenFilePath", QFileInfo(m_strFilePath).filePath());
    m_settings.endGroup();
}

void logRemake::loadFile()
{
    m_strFilePath = QFileDialog::getOpenFileName(this,
                                                 QObject::tr("Open LogPro file"),
                                                 m_strLastOpenPath,
                                                 QObject::tr("logp files (*.logp)"));
    if (!m_strFilePath.isEmpty()) {
        m_pBtnConvertFile->setEnabled(true);
        m_pBtnDestination->setEnabled(true);
        m_pLblFileName->setText("Source file: " + QFileInfo(m_strFilePath).fileName());
        m_pLblStatus->setText("Status: file loaded");
    }
    else
        return;
}

void logRemake::readDataFromFile()
{
    //read correct data from file
    QVector<QByteArray> correctData;
    QFile correctFile(QApplication::applicationDirPath() + "/data.logp");
    if(!correctFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "Can't open correct file!";
        return;
    }
    while(!correctFile.atEnd())
    {
        correctData.push_back(correctFile.readLine());
    }
    for (int i(0); i < correctData.size(); ++i)
    {
        if (correctData.at(i).contains("\n") || correctData.at(i).contains("\r"))
        {
            correctData[i].replace("\r", "");
            correctData[i].replace("\n", "");
        }

    }
    qDebug() << "Size of vector" << correctData.size();
    qDebug() << correctData;
    qDebug() << correctData.at(0).size();
    correctFile.close();

    //read input data from file
    QByteArray dataFromFile;
    QFile inputFile(m_strFilePath);
    if  (inputFile.open(QIODevice::ReadOnly))
    {

        dataFromFile = inputFile.readAll();

    }
    inputFile.close();

    //start workin with data
    QString buffer;
    QStringList data;
    int point1 = 0, point2 = 0;
    for (int i(0); i < (dataFromFile.size()); ++i) //  read for matches
    {
        if ((dataFromFile.at(i) == '2' && dataFromFile.at(i + 1) == '0'
             && dataFromFile.at(i + 2) == '1' && (i < dataFromFile.size()))
                && (dataFromFile.at(i + 3) == '4' || dataFromFile.at(i + 3) == '5'
                    || dataFromFile.at(i + 3) == '6' || dataFromFile.at(i + 3) == '7'
                    || dataFromFile.at(i + 3) == '8' || dataFromFile.at(i + 3) == '9'))
        {
            point1 = i + 1;
            for (int j(0); j < 14; ++j) // pass ahead by 15 characters
            {
                if(dataFromFile.at(i + j) == '0' || dataFromFile.at(i + j) == '1' || dataFromFile.at(i + j) == '2'
                        || dataFromFile.at(i + j) == '3' || dataFromFile.at(i + j) == '4' || dataFromFile.at(i + j) == '5'
                        || dataFromFile.at(i + j) == '6' || dataFromFile.at(i + j) == '7' || dataFromFile.at(i + j) == '8'
                        || dataFromFile.at(i + j) == '9'
                        )
                    buffer += dataFromFile.at(i + j);
                else
                    buffer.clear();

                if(buffer.size() == 14)
                {
                    point2 = i + 15;    //find end of date and save coords
                    data.push_back(buffer);
                    buffer.clear();
                    cords.push_back(qMakePair(point1, point2));
                }
            }

        }
    }

    qDebug() << "Data array size:" << data.size();
    for (auto i(0); i < data.size(); ++i)
        qDebug() << data.at(i) << " : " << cords.at(i);

    //have coords, now can edit data
    int fileNumber = 0;
    for (int m(0); m < cords.size(); ++m)
    {
        fileNumber = qrand() % (((correctData.size() - 1) + 1) - 0) + 0;
        dataFromFile.remove(cords.at(m).second - 1, 12);
        dataFromFile.insert(cords.at(m).second - 1, correctData.at(fileNumber));
    }

    //output file
    QFile outputFile(m_strPathToSaveFile + "/" + QFileInfo(m_strFilePath).fileName());
    if(!outputFile.open(QIODevice::WriteOnly))
        return;
    outputFile.write(dataFromFile);
    outputFile.close();
    m_pLblStatus->setText("Status: FINISHED");
    m_pBtnConvertFile->setEnabled(false);
    m_pBtnDestination->setEnabled(false);
    m_pLblFileDestination->setText("Destination: " + m_strPathToSaveFile + "/");
}

void logRemake::saveFileTo()
{
    m_strPathToSaveFile = QFileDialog::getExistingDirectory(this,
                                                            QObject::tr("Directory to save"),
                                                            m_strPathToSaveFile);
    m_pLblFileDestination->setText("Destination: " + m_strPathToSaveFile + "/");
}

void logRemake::closeEvent(QCloseEvent *)
{
    writeSettings();
}

void logRemake::resizeEvent(QResizeEvent *)
{
    qDebug() << "Height:" << height();
    qDebug() << "Width:" << width();
}

logRemake::~logRemake()
{

}

newStyle::newStyle() : QProxyStyle(QStyleFactory::create("Fusion"))
{

}

void newStyle::polish(QPalette &darkPalette)
{
    darkPalette.setColor(QPalette::Window, QColor(53,53,53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25,25,25)); //listWidget color
    darkPalette.setColor(QPalette::AlternateBase, QColor(53,53,53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, QColor(216, 224, 240));
    darkPalette.setColor(QPalette::Button, QColor(4, 90, 135));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);
}
