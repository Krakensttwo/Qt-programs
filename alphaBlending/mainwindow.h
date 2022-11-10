//Michal Sobczak, alpha blending
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


    void on_checkBox_stateChanged(int arg1);

    void on_checkBox_2_stateChanged(int arg1);

    void on_checkBox_4_stateChanged(int arg1);

    void on_checkBox_3_stateChanged(int arg1);

    void on_checkBox_5_stateChanged(int arg1);

    void on_comboBox_currentIndexChanged(int index);

    void on_comboBox_2_currentIndexChanged(int index);

    void on_comboBox_4_currentIndexChanged(int index);

    void on_comboBox_3_currentIndexChanged(int index);

    void on_comboBox_5_currentIndexChanged(int index);

    void on_horizontalSlider_valueChanged(int value);

    void on_horizontalSlider_2_valueChanged(int value);

    void on_horizontalSlider_3_valueChanged(int value);

    void on_horizontalSlider_4_valueChanged(int value);

    void on_horizontalSlider_5_valueChanged(int value);

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
    QImage *obrazy[5];
    double alpha_wartosci[5]= {0,0,0,0,0};
    int aktywny[5] = {0,0,0,0,0};
    int tryb[5] = {0,0,0,0,0};
    void rysujObraz();
    void blend(QImage *picture, QImage *background,double alpha,int mode);
    void czysc(QImage *picture);
};
#endif // MAINWINDOW_H
