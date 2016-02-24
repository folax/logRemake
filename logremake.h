#ifndef LOGREMAKE_H
#define LOGREMAKE_H

#include <QDialog>
#include <QSettings>
#include <QProxyStyle>

class QPushButton;
class QVBoxLayout;
class QStringList;
class QLabel;

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

protected:
    void closeEvent(QCloseEvent *);
    void resizeEvent(QResizeEvent *);

private:
    QSettings m_settings;

    QPushButton *m_pBtnLoadFile;
    QPushButton *m_pBtnDestination;
    QPushButton *m_pBtnConvertFile;
    QPushButton *m_pBtnClose;

    QVBoxLayout *m_pMainLayout;
    QVBoxLayout *m_pBtnLayout;

    QString filePath;
    QString m_strLastOpenPath;
    QString m_strPathToSaveFile;

    QLabel *m_pLblCaption;
    QLabel *m_pLblFileName;
    QLabel *m_pLblFileDestination;
    QLabel *m_pLblStatus;

    QVector<QPair<int, int>> cords;
};

class newStyle : public QProxyStyle
{
    Q_OBJECT

public:
    newStyle();

    void polish(QPalette& pal)Q_DECL_OVERRIDE;
};

#endif // LOGREMAKE_H
