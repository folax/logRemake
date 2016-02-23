#include <QDebug>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QFile>
#include <QPair>
#include <QApplication>

#include "logremake.h"


static QStringList temp_wet =
{
    "  јAffBж", "НМ°Aљ™BvЙ", "33ЇAffBсбШ", "  ґAНМB—»С", "ffІAНМBпМ",
    "33ЇA33BЫЬО", "33ЇAffB{¤Т", "  ґAffBЩЙ", "  ёAffBz¤µ", "  A   B0±ў",
    "НМAffB[›", "  A33B®БЋ", "33—AffB%yЏ", "ffљA  (BVЭБ", "33іAНМ&BXн",
    "НМёA33Bпн", "33»Aљ™BЮУ", "  АAffBфОъ", "НМњAљ™B-н¬", "33«AНМBВЇЕ",
    "ffЄAНМBШВ", "  °Aљ™B›№", "љ™±AffB ґ", "33ЇAffB>кё", "љ™ҐA  0Bэ ",
    "љ™ҐAff.B8Ны", "fffA  <BђN"
};

const char *word = "33ЇAffB>кё";

logRemake::logRemake(QWidget *parent) : QDialog(parent), m_settings("Decay", "logRemake")
{
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint & Qt::WindowMinimized);
    resize(400, 400);
    m_strLastOpenPath = "c:\\";

    //buttons
    m_pBtnLoadFile = new QPushButton("Load file");
    m_pBtnReadFile = new QPushButton("Read file");
    m_pBtnRemakeFile = new QPushButton("Remake file");
    m_pBtnClose = new QPushButton("Close");
    m_pBtnReadFile->setEnabled(false);
    m_pBtnRemakeFile->setEnabled(false);

    //btn layout
    m_pBtnLayout = new QVBoxLayout();
    m_pBtnLayout->addWidget(m_pBtnLoadFile);
    m_pBtnLayout->addWidget(m_pBtnReadFile);
    m_pBtnLayout->addWidget(m_pBtnRemakeFile);
    m_pBtnLayout->addWidget(m_pBtnClose);

    //main layout
    m_pMainLayout = new QVBoxLayout(this);
    m_pMainLayout->addLayout(m_pBtnLayout);
    setLayout(m_pMainLayout);

    //connect
    connect(m_pBtnLoadFile, &QPushButton::clicked, this, &logRemake::loadFile);
    connect(m_pBtnReadFile, &QPushButton::clicked, this, &logRemake::readDataFromFile);
    connect(m_pBtnClose, &QPushButton::clicked, this, &logRemake::close);
    readSettings();
}

void logRemake::readSettings()
{
    m_settings.beginGroup("/logRemakeSettings");
    int nWidth = m_settings.value("/Width", width()).toInt();
    int nHeight = m_settings.value("/Height", height()).toInt();
    m_strLastOpenPath = m_settings.value("/LastOpenFilePath", "c:\\").toString();
    resize(nWidth, nHeight);
    m_settings.endGroup();
}

void logRemake::writeSettings()
{
    m_settings.beginGroup("/logRemakeSettings");
    m_settings.setValue("/Width", width());
    m_settings.setValue("/Height", height());
    if (filesPath.size() > 0) {
        m_settings.setValue("/LastOpenFilePath", QFileInfo(filesPath.at(0)).filePath());
    }
    m_settings.endGroup();
}

void logRemake::loadFile()
{
    filesPath = QFileDialog::getOpenFileNames(this,
                                              QObject::tr("Open LogPro file"),
                                              m_strLastOpenPath,
                                              QObject::tr("logp files (*.logp)"));
    if (!filesPath.isEmpty()) {
        m_pBtnReadFile->setEnabled(true);
        m_pBtnRemakeFile->setEnabled(true);
    }
    else
        return;
}

void logRemake::readDataFromFile()
{
    //read correct data from file
    QByteArray correctData;
    QFile correctFile(QApplication::applicationDirPath() + "/data.logp");
    if(!correctFile.open(QIODevice::ReadOnly))
        qDebug() << "Can't open correct data file.";
    else
        correctData = correctFile.readAll();


    //read input data from file
    QByteArray dataFromFile;
    QFile inputFile(filesPath.at(0));
    if  (inputFile.open(QIODevice::ReadOnly))
    {

        dataFromFile = inputFile.readAll();

    }
    inputFile.close();


    //start workin with data
    QString str;
    QString buffer;
    QStringList data;
    int point1 = 0, point2 = 0;

    for (int i(0); i < (dataFromFile.size()); ++i) //  read for matches
    {
        if (dataFromFile.at(i) == '2'
                && dataFromFile.at(i + 1) == '0'
                && dataFromFile.at(i + 2) == '1'
                && dataFromFile.at(i + 3) == '4'
                && (i < dataFromFile.size()))
        {
            point1 = i + 1;
            for (int j(0); j < 14; ++j) // проходим вперёд на 15 символов
            {
                if(dataFromFile.at(i + j) == '0' || dataFromFile.at(i + j) == '1' || dataFromFile.at(i + j) == '2'
                        || dataFromFile.at(i + j) == '3' || dataFromFile.at(i + j) == '4' || dataFromFile.at(i + j) == '5'
                        || dataFromFile.at(i + j) == '6' || dataFromFile.at(i + j) == '7' || dataFromFile.at(i + j) == '8'
                        || dataFromFile.at(i + j) == '9'
                        )
                    buffer += dataFromFile.at(i + j);
                else
                    buffer.clear();
            }

            if(buffer.size() == 14)
            {
                point2 = i + 15;    //находим конец даты и сохраняем координаты
                data.push_back(buffer);
                buffer.clear();
                cords.push_back(qMakePair(point1, point2));
            }
        }
    }

    //есть координаты, можем менять данные!
    //int before = 0, after = 0, diff = 0;

    for (int m(0); m < cords.size(); ++m)
    {
        //before = cords.at(m + 1).first - cords.at(m).second;
        //after = QString("zzzzzzzzzz").size();
        dataFromFile.remove(cords.at(m).second - 1, 12);
        dataFromFile.insert(cords.at(m).second - 1, correctData);

        //diff = before - after;
        //        qDebug() << before;
//        for (int h = m + 1; h < cords.size(); ++h)
//        {
//            cords[h] = qMakePair(cords.at(h).first, cords.at(h).second);
//        }
    }

    //    qDebug() << data;
    //    qDebug() << "Size:" << data.size();
    //qDebug() << dataFromFile;
//    for (auto k : cords)
//        qDebug() << k;
    qDebug() << correctData;



    //output file
    QFile outputFile("Log.logp");
    if(!outputFile.open(QIODevice::WriteOnly))
        return;
    outputFile.write(dataFromFile);
    outputFile.close();

}


void logRemake::closeEvent(QCloseEvent *)
{
    writeSettings();
}

logRemake::~logRemake()
{

}

