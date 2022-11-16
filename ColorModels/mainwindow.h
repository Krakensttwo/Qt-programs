//Michal Sobczak, color models
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
    void on_horizontalSlider_sliderMoved(int position);

    void on_horizontalSlider_2_sliderMoved(int position);

    void on_horizontalSlider_3_sliderMoved(int position);

    void on_horizontalSlider_4_sliderMoved(int position);

    void on_horizontalSlider_5_sliderMoved(int position);

    void on_horizontalSlider_6_sliderMoved(int position);

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
    int startX, startY;

    // czyszczenie obrazu (zamalowanie na czarno)
    void clean();

    // zamalowuje piksel (x,y) na kolor (red,green,blue), domyślnie na czarno
    void drawPixel(int x, int y, unsigned char red = 255, unsigned char green = 255, unsigned char blue = 255);
    int ffred=0;
    int ffgreen=0;
    int ffblue=0;
    int jakikolor=0;

    int ffh=0;
    int ffs=0;
    int ffv=0;
    void rysujRGB();
    void rysujHSV();


};
#endif // MAINWINDOW_H
