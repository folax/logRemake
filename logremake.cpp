#include <QDebug>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QFile>



#include "logremake.h"


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
    QFile file(filesPath.at(0));
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream stream(&file);
        QString str;
        QString buffer;
        QStringList data;
        int cnt = 0;    //data starting from second line
        while (!stream.atEnd()) {
            str = stream.readLine();
            if (cnt == 0) {
                for (int i(0); i < (str.size()); ++i) //получаем строку
                {
                    if (str.at(i) == 'A' && (i < str.size())) //проверяем если начинается с А
                    {
                        for (int j(0); j < 15; ++j) // проходим вперёд на 15 символов
                        {
                            if (str.at(i + j).isDigit())
                            {
                                buffer += str.at(i + j);
                            }
                            else
                            {
                                buffer.clear();
                            }
                        }
                        if(buffer.size() == 14)
                        {
                            data.push_back(buffer);
                            buffer.clear();
                        }

                    }
                }
            }
            qDebug() << data;
            qDebug() << "Size:" << data.size();
            //cnt++;
        }
    }
    file.close();
}


void logRemake::closeEvent(QCloseEvent *)
{
    writeSettings();
}

logRemake::~logRemake()
{

}

