#ifndef LOGREMAKE_H
#define LOGREMAKE_H

#include <QDialog>
#include <QSettings>
#include <QProxyStyle>

class QPushButton;
class QVBoxLayout;
class QStringList;
class QLabel;
class logParser;
class QCheckBox;


//logRamake class
class logRemake : public QDialog
{
    Q_OBJECT
public:
    explicit logRemake(QWidget *parent = 0);
    void readSettings();
    void writeSettings();
    ~logRemake();

public slots:
    void loadFile();
    void readDataFromFile();
    void saveFileTo();
    void parseDataFromLogs();

protected:
    void closeEvent(QCloseEvent *);
    void resizeEvent(QResizeEvent *);

private:
    QSettings m_settings;

    QPushButton *m_pBtnLoadFile;
    QPushButton *m_pBtnDestination;
    QPushButton *m_pBtnConvertFile;
    QPushButton *m_pBtnClose;
    QPushButton *m_pBtnParser;

    QVBoxLayout *m_pMainLayout;
    QVBoxLayout *m_pBtnLayout;

    QString m_strFilePath;
    QString m_strLastOpenPath;
    QString m_strPathToSaveFile;

    QLabel *m_pLblCaption;
    QLabel *m_pLblFileName;
    QLabel *m_pLblFileDestination;
    QLabel *m_pLblStatus;

    QVector<QPair<int, int>> cords;
    logParser *m_pLp;
};


//logParser class
class logParser : public QDialog
{
    Q_OBJECT
public:
    explicit logParser(QWidget *parent = 0);
    ~logParser();

public slots:
    void parseDataFromLogs();

private:
    QVBoxLayout *m_pMainLayout;
    QVBoxLayout *m_pItemsLayout;

    QLabel *m_pLblInfo;
    QLabel *m_pLblStatus;
    QPushButton *m_pBtnParseData;

    QString m_strStatus;
    QStringList m_filesList;
    QByteArray m_baDataToWrite;
};


//newStyle class
class newStyle : public QProxyStyle
{
    Q_OBJECT

public:
    newStyle();

    void polish(QPalette& pal)Q_DECL_OVERRIDE;
};

#endif // LOGREMAKE_H
