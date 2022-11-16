//Michal Sobczak, bezier curve
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

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

    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

    void on_radioButton_3_clicked();

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

    int odcinekLubPunkt=0;
    int xOdc0, yOdc0, xOdc1, yOdc1;

    // zamalowuje piksel (x,y) na kolor (red,green,blue), domyślnie na biało
    void drawPixel(int x, int y, unsigned char red = 255, unsigned char green = 255, unsigned char blue = 255);
    void rysujDuzyPunkt(int x,int y);
    int tabPunktX[500];
    int tabPunktY[500];
    int iloscPunktow=0;
    int ktoryPunkt=0;
    void rysujBeziera(int ktory,unsigned char red, unsigned char green, unsigned char blue);
    float xB0,yB0,xB1,yB1;
    void rysujOdcinek(int x0, int y0, int x1, int y1,unsigned char red, unsigned char green, unsigned char blue);

    int trafiony=0;
};
#endif // MAINWINDOW_H
