#ifndef LOGREMAKE_H
#define LOGREMAKE_H

#include <QDialog>

class QPushButton;
class QVBoxLayout;
class QStringList;

class logRemake : public QDialog
{
    Q_OBJECT
public:
    explicit logRemake(QWidget *parent = 0);

    ~logRemake();

public slots:
    void loadFile();

private:
    QPushButton *m_pBtnLoadFile;
    QPushButton *m_pBtnAnalizeFile;
    QPushButton *m_pBtnRemakeFile;
    QVBoxLayout *m_pMainLayout;
    QVBoxLayout *m_pBtnLayout;

    QStringList filesPath;

};

#endif // LOGREMAKE_H
