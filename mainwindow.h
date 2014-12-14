#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTextStream>
#include <QStringList>
#include <QMessageBox>
#include <QFile>
#include <QList>
#include <cstdlib>
#include <ctime>
#include <QFileDialog>

namespace Ui {
class MainWindow;
}

class Word
{
public:
    QString word;
    int times;
    Word(QString w,int t):word(w),times(t){}
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    QList<Word> wordList;
    QList<Word> removedList;
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QLabel* status;
    void readFile(void);

private slots:
    void learn_start(void);
    void fromFile(void);
    void Exit(void);
    void saveResult(void);
};



#endif // MAINWINDOW_H
