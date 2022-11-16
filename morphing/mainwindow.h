//Michal Sobczak, morphing
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QStack>
#include <QVector>
#include <cmath>
#include <QFileDialog>
#include <math.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// Deklaracja głównej klasy naszego programu
// Jest ona podklasą klasy QMainWindow
// standardowej klasy Qt reprezentującej główne okno aplikacji
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

// Sloty to specjalne funkcje (metody) wywoływane
// po wystąpieniu pewnych zdarzeń np. po kliknięciu przycisku
private slots:
    void on_exitButton_clicked();
    void on_cleanButton_clicked();
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void rysujDuzyPunkt(int x, int y);
    void rysujOdcinek(int x0, int y0, int x1, int y1, unsigned char red, unsigned char green, unsigned char blue);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_spinBox_valueChanged(int arg1);

    void on_pushButton_3_clicked();

private:
    // ui to wskaźnik za pomocą którego mamy dostęp
    // do elementów interfejsu użytkownika
    Ui::MainWindow *ui;

    // Obiekt klasy QImage będzie przechowywał rysowany obraz
    QImage *img;

    // Pola przechowujące wymiary obrazu
    int width, height;

    // Współrzędne lewego górnego narożnika pola w którym wyświetlany będzie obraz
    // (względem głównego okna aplikacji)
    int startX1, startY1;
    int startX2, startY2;
    int startX3, startY3;
    int licznik1=0;
    int licznik2=0;

    // czyszczenie obrazu (zamalowanie na czarno)
    void clean();

    // zamalowuje piksel (x,y) na kolor (red,green,blue), domyślnie na czarno
    void drawPixel(int x, int y, unsigned char red = 255, unsigned char green = 255, unsigned char blue = 255);
    QImage *obraz;
    QImage *wynikowy;

    struct ffpunkt {
        int x;
        int y;
    };

    QVector<ffpunkt> jeden;
    QVector<ffpunkt> dwa;
    QVector<ffpunkt> trzy;
    void animuj();
    int liczbaKlatek;

    QString nowyObraz1="";
    QString zapisany1="";
    QString nowyObraz2="";
    QString zapisany2="";

    void wczytaj(QString obraz1,int jaki);
    void delay(int n);


};
#endif // MAINWINDOW_H
