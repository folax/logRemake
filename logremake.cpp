#include <QDebug>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFileDialog>

#include "logremake.h"


logRemake::logRemake(QWidget *parent) : QDialog(parent)
{
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint & Qt::WindowMinimized);
    resize(400, 400);

    //buttons
    m_pBtnLoadFile = new QPushButton("Load file");
    m_pBtnAnalizeFile = new QPushButton("Analize file");
    m_pBtnRemakeFile = new QPushButton("Remake file");
    m_pBtnAnalizeFile->setEnabled(false);
    m_pBtnRemakeFile->setEnabled(false);

    //btn layout
    m_pBtnLayout = new QVBoxLayout();
    m_pBtnLayout->addWidget(m_pBtnLoadFile);
    m_pBtnLayout->addWidget(m_pBtnAnalizeFile);
    m_pBtnLayout->addWidget(m_pBtnRemakeFile);

    //main layout
    m_pMainLayout = new QVBoxLayout(this);
    m_pMainLayout->addLayout(m_pBtnLayout);
    setLayout(m_pMainLayout);

    //connect
    connect(m_pBtnLoadFile, &QPushButton::clicked, this, &logRemake::loadFile);
}

void logRemake::loadFile()
{
    filesPath = QFileDialog::getOpenFileNames(this,
                                              QObject::tr("Open LogPro file"),
                                              "c:",
                                              QObject::tr("logp files (*.logp)"));
    if (!filesPath.isEmpty()) {
        m_pBtnAnalizeFile->setEnabled(true);
        m_pBtnRemakeFile->setEnabled(true);
    }
}

logRemake::~logRemake()
{

}

