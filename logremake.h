#ifndef LOGREMAKE_H
#define LOGREMAKE_H

#include <QDialog>
#include <QSettings>

class QPushButton;
class QVBoxLayout;
class QStringList;

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

protected:
    void closeEvent(QCloseEvent *);

private:
    QSettings m_settings;

    QPushButton *m_pBtnLoadFile;
    QPushButton *m_pBtnReadFile;
    QPushButton *m_pBtnRemakeFile;
    QPushButton *m_pBtnClose;

    QVBoxLayout *m_pMainLayout;
    QVBoxLayout *m_pBtnLayout;

    QStringList filesPath;
    QString m_strLastOpenPath;

};

#endif // LOGREMAKE_H
