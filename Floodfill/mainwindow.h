#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QStack>


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

    void on_radioButton_4_clicked();

    void on_horizontalSlider_valueChanged(int value);


    void on_checkBox_stateChanged(int arg1);

    void on_radioButton_5_clicked();

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

    void rysujOdcinek(int x0, int y0, int x1, int y1);
    int odcinekLubPunkt=0;
    int xOdc0, yOdc0, xOdc1, yOdc1;
    int xEli, yEli;

    // zamalowuje piksel (x,y) na kolor (red,green,blue), domyślnie na biało
    void drawPixel(int x, int y, unsigned char red = 255, unsigned char green = 255, unsigned char blue = 255);
    void drawOsiemPixel(int x, int y,int x0, int y0);
    void drawCzteryPixel(int x, int y,int x0, int y0);
    double rOkrag;
    void rysujOkrag(int x0, int y0, int x1, int y1);
    void rysujElipse(int x0, int y0, int x1, int y1);
    int wpisany=0;
    int iloscWierzcholkow=3;
    int bbb=0;
    void rysujFloodfill(int x, int y);
    int ffred;
    int ffgreen;
    int ffblue;

    struct ffpunkt {
        int fx;
        int fy;
    };
};
#endif // MAINWINDOW_H
