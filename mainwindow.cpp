#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    srand((unsigned int)time(NULL));
    ui->setupUi(this);
    ui->plainTextEdit->setReadOnly(true);
    ui->plainTextEdit_2->setReadOnly(true);

    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(learn_start()));
    connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(fromFile()));
    connect(ui->pushButton_4,SIGNAL(clicked()),this,SLOT(Exit()));
    connect(ui->pushButton_3,SIGNAL(clicked()),this,SLOT(saveResult()));

    readFile();
}

void MainWindow::readFile()
{
    QFile file("wordlist.txt");
    if(!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        QMessageBox::information(this,"提示","无法打开wordlist.txt");
    }
    else
    {
        QTextStream stream(&file);
        QString line_in = "",wordlist_str = "",removedlist_str="";
        int line_count = 0;
        while( !stream.atEnd())
        {
            line_in = stream.readLine();
            if(line_in[0] == '#'||line_in == "")continue;
            QStringList tmp = line_in.split(" ");
            Word t = Word(tmp[0],tmp[1].toInt());
            wordList.append(t);
            line_count ++;
        }

        for(int i = 0;i < wordList.length();i++)
        {
            if(wordList[i].word[0] == '$')
            {
                removedlist_str = removedlist_str + wordList[i].word.remove(0,1) + "\n";
                removedList.append(wordList[i]);
                wordList.removeAt(i);
                i--;
            }
            else
                wordlist_str = wordlist_str + wordList[i].word + "\n";
        }
        ui->plainTextEdit->setPlainText(wordlist_str);
        ui->plainTextEdit_2->setPlainText(removedlist_str);
        QString line_count_str = QString::number(line_count);
        status = new QLabel("已打开wordlist.txt，共" + line_count_str + "条记录");
        ui->statusBar->addWidget(status);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::learn_start(void)
{
    if(wordList.length() == 0)
    {
        QMessageBox::information(this,"恭喜","所有单词已完成");
        QString wordlist_str = "",removedlist_str = "";
        for(int i = 0;i < wordList.length();i++)
        {
            wordlist_str = wordlist_str + wordList[i].word + "\n";
        }
        for(int i = 0;i < removedList.length();i++)
        {
            removedlist_str = removedlist_str + removedList[i].word + "\n";
        }
        ui->plainTextEdit->setPlainText(wordlist_str);
        ui->plainTextEdit_2->setPlainText(removedlist_str);
        status->setText("列表已刷新");
        saveResult();
        return;
    }
    QMessageBox msg;
    msg.setWindowTitle("单词");
    int index = rand()%wordList.length();
    msg.setText(wordList[index].word);
    QPushButton *yesButton = msg.addButton("勾", QMessageBox::AcceptRole);
    QPushButton *noButton = msg.addButton("叉", QMessageBox::RejectRole);
    QPushButton *exitButton = msg.addButton("退出", QMessageBox::RejectRole);
    msg.setDefaultButton(yesButton);
    msg.exec();
    if(msg.clickedButton() == yesButton)
    {
        status->setText(wordList[index].word + "还剩" + QString::number(--wordList[index].times) + "次");
        if(wordList[index].times <= 0)
        {
            removedList.append(wordList[index]);
            wordList.removeAt(index);
        }
        learn_start();
    }
    else if(msg.clickedButton() == noButton)
    {
        status->setText(wordList[index].word + "还剩" + QString::number(wordList[index].times) + "次");
        learn_start();
    }
    else if(msg.clickedButton() == exitButton)
    {
        QString wordlist_str = "",removedlist_str = "";
        for(int i = 0;i < wordList.length();i++)
        {
            wordlist_str = wordlist_str + wordList[i].word + "\n";
        }
        for(int i = 0;i < removedList.length();i++)
        {
            removedlist_str = removedlist_str + removedList[i].word + "\n";
        }
        ui->plainTextEdit->setPlainText(wordlist_str);
        ui->plainTextEdit_2->setPlainText(removedlist_str);
        status->setText("列表已刷新");
        saveResult();
    }
}

void MainWindow::fromFile(void)
{
    QMessageBox msg;
    msg.setWindowTitle("提示");
    msg.setText("此功能将会选择一个txt文件来生成本程序所需的文件格式，确定选择，取消退出。\n\n注意：请确保txt文件中每一行一个单词。");
    QPushButton *yesButton = msg.addButton("确定", QMessageBox::AcceptRole);
    msg.addButton("取消", QMessageBox::RejectRole);
    msg.setDefaultButton(yesButton);
    msg.setIcon(QMessageBox::Warning);
    msg.exec();

    if(msg.clickedButton() == yesButton)
    {
        QString path = QFileDialog::getOpenFileName(this, tr("打开单词文件"), ".", tr("文本文件(*.txt)"));
        if(path.length() == 0)
        {
            QMessageBox::information(NULL, tr("打开单词文件"), tr("未选择文件！"));
        }
        else
        {
            QFile file(path);
            if(!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
                QMessageBox::information(this,"提示",QString("无法打开") + path);
            }
            else
            {
                QTextStream stream(&file);
                QString line_in = "",wordlist_str = "",removedlist_str="";
                int line_count = 0;
                while( !stream.atEnd())
                {
                    line_in = stream.readLine();
                    if(line_in[0] == '#')continue;
                    line_in.insert(line_in.length()," 10\n");
                    wordlist_str += line_in;
                    line_count ++;
                }
                QFile file("wordlist.txt");
                file.open(QIODevice::ReadWrite | QIODevice::Text);
                file.resize(0);
                QTextStream stream1(&file);
                stream1<<wordlist_str;
                QMessageBox::information(NULL, tr("提示"), tr("已写入worlist.txt\n请重新打开程序"));
                close();
            }
        }
    }
}

void MainWindow::Exit()
{
    saveResult();
    close();
}

void MainWindow::saveResult()
{
    QString save_str = "";
    for(int i = 0;i < wordList.length();i++)
        save_str = save_str + wordList[i].word + " " + QString::number(wordList[i].times) + "\n";
    for(int i = 0;i < removedList.length();i++)
        save_str = save_str + "$" + removedList[i].word + " " + QString::number(removedList[i].times) + "\n";

    QFile file("wordlist.txt");
    file.open(QIODevice::ReadWrite | QIODevice::Text);
    file.resize(0);
    QTextStream stream1(&file);
    stream1<<save_str;
    QMessageBox::information(NULL, tr("提示"), tr("已保存结果！"));
}
