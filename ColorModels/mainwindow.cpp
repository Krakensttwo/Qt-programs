//Michal Sobczak, color models
#include "mainwindow.h"
#include "ui_mainwindow.h"

// Konstruktor
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Pobieramy współrzędne (względem głównego okna) lewego, górnego narożnika
    // ramki, w której wyświetlany będzie obraz
    startX = ui->frame->x();
    startY = ui->frame->y();

    // Pobieramy wymiary ramki
    width = ui->frame->width();
    height = ui->frame->height();

    // Tworzymy obiekt klasy QImage o wymiarach równych wymiarom ramki
    // Format_RGB32 to 32 bitowe RGB (z kanałem alfa ustawionym na wartość 255)
    img = new QImage(width, height, QImage::Format_RGB32);
    clean();
    update();

    //qDebug() << startX;
    //qDebug() << startY;
}

MainWindow::~MainWindow()
{
    delete ui;
}

//*****************************************************************************************************

// Funkcja ,,odrysowująca'' okno aplikacji. Wywoływana automatycznie podczas uruchamiania aplikacji
// i w innych sytuacjach, w których zachodzi konieczność ponownego narysowania okna.
// Można też wymusić jej wywołanie za pomocą funkcji update()
void MainWindow::paintEvent(QPaintEvent*)
{
    // Obiekt przy pomocy, którego możemy rysować po elementach interfejsu graficznego aplikacji
    QPainter p(this);

    // Metoda drawImage wyświetla zawartość obiektu *img klasy QImage w głównym oknie aplikacji
    // startX i startY to współrzędne (względem głównego okna) lewego górnego narożnika
    // wyświetlanego obiektu
    p.drawImage(startX, startY, *img);
}
//*****************************************************************************************************



// Funkcja zamalowująca na czarno wszystkie piksele obrazu *img
void MainWindow::clean()
{
    // deklaracja wskaźnika do poruszania się po pikselach obrazu
    unsigned char *wsk;

    // przechodzimy po wszystkich wierszach obrazu
    for(int i=0; i<height; i++)
    {
        // ustawiamy wskaźnik na początku i-tego wiersza
        wsk = img->scanLine(i);

        // przechodzimy po wszystkich piselach i-tego wiersza
        // i nadajemy im wartość (0,0,0) odpowiadającą kolorowi czarnemu
        for(int j=0; j<width; j++)
        {
            wsk[4*j] = 255;   // ustawiamy składową ,,niebieską'' na 255
            wsk[4*j+1] = 255; // ustawiamy składową ,,zieloną'' na 255
            wsk[4*j+2] = 255; // ustawiamy składową ,,czerwoną'' na 255
        }
    }
}
//*****************************************************************************************************

// zamalowuje piksel (x,y) na kolor (red,green,blue), domyślnie na biało
void MainWindow::drawPixel(int x, int y, unsigned char red, unsigned char green, unsigned char blue)
{
    unsigned char *wsk;

    // sprawdzamy czy (x,y) leży w granicach rysunku
    if(x>=0 && y>=0 && x<width && y<height)
    {
        // ustawiamy wskaźnik na początku linii y
        wsk = img->scanLine(y);

        // zmieniamy składowe piksela x
        wsk[4*x] = blue;
        wsk[4*x+1] = green;
        wsk[4*x+2] = red;
    }

}
//*****************************************************************************************************



// Metoda wywoływana po wciśnięciu przycisku myszy
// Inne metody związane z obsługą myszy to
// void MainWindow::mouseReleaseEvent(QMouseEvent *event)
// wywoływana po puszczeniu przycisku myszy i
// void MainWindow::mouseMoveEvent(QMouseEvent *event)
// wywoływana po każdym ruchu myszy
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    //qDebug()<<odcinekLubPunkt;
    int x,y;
    // Pobieramy współrzędne punktu kliknięcia
    x = event->pos().x();
    y = event->pos().y();

    // Współrzędne obliczane są względem głównego okna programu
    // aby uzyskać współrzędne względem obszaru rysowania (ramki) musimy je przesunąć
    // tak aby punkt (0,0) pokrywał się z lewym górnym naroznikiem obszaru rysowania
    x = x - startX;
    y = y - startY;
}
void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    int x,y;
    x = event->pos().x();
    y = event->pos().y();

    // Współrzędne obliczane są względem głównego okna programu
    // aby uzyskać współrzędne względem obszaru rysowania (ramki) musimy je przesunąć
    // tak aby punkt (0,0) pokrywał się z lewym górnym naroznikiem obszaru rysowania
    x = x - startX;
    y = y - startY;
}

//*****************************************************************************************************

// Metoda wywoływana po nacisnięciu przycisku cleanButton (,,Czyść'')
void MainWindow::on_cleanButton_clicked()
{
    clean();

    // Po zmodyfikowaniu obiektu QImage odświeżamy okno aplikacji, aby zobaczyc zmiany
    update();
}
//*****************************************************************************************************

// Metoda wywoływana po nacisnięciu przycisku exitButton (,,Wyjście'')
void MainWindow::on_exitButton_clicked()
{
    // qApp to globalny wskaźnik na obiekt aplikacji
    // metoda quit() powoduje zakończenie działania aplikacji z kodem wyjścia 0 (sukces)
    qApp->quit();
}

//*****************************************************************************************************

void MainWindow::rysujRGB() {
    if(jakikolor==1) {
        for(int i=0;i<height;i=i+2) {
            for(int j=0;j<width;j=j+2) {
                drawPixel(j,i,ffred,255-i/2,j/2);
                drawPixel(j,i+1,ffred,255-i/2,j/2);
                drawPixel(j+1,i,ffred,255-i/2,j/2);
                drawPixel(j+1,i+1,ffred,255-i/2,j/2);
            }
        }
    } else if(jakikolor==2) {
        for(int i=0;i<height;i=i+2) {
            for(int j=0;j<width;j=j+2) {
                drawPixel(i,j,i/2,ffgreen,255-j/2);
                drawPixel(i,j+1,i/2,ffgreen,255-j/2);
                drawPixel(i+1,j,i/2,ffgreen,255-j/2);
                drawPixel(i+1,j+1,i/2,ffgreen,255-j/2);
            }
        }
    } else if(jakikolor==3) {
        for(int i=0;i<height;i=i+2) {
            for(int j=0;j<width;j=j+2) {
                drawPixel(i,j,255-j/2,i/2,ffblue);
                drawPixel(i,j+1,255-j/2,i/2,ffblue);
                drawPixel(i+1,j,255-j/2,i/2,ffblue);
                drawPixel(i+1,j+1,255-j/2,i/2,ffblue);
            }
        }
    }
    update();
}
void MainWindow::rysujHSV() {
    if(jakikolor==1) {
        double r,g,b;
        double hp=static_cast<double>(ffh)/static_cast<double>(60);
        double pom = fmod(hp,static_cast<double>(2))-static_cast<double>(1);
        if(pom<0) {
            pom=-1*pom;
        }
        for(int i=0;i<height;i++) {
            for(int j=0;j<width;j++) {
                double c=(static_cast<double>(i)/static_cast<double>(height))*(static_cast<double>(j)/static_cast<double>(width));
                double x=c*(1-pom);
                double m=static_cast<double>(i)/512-c;
                if(hp>=0 && hp<=1) {
                    r=c+m;
                    g=x+m;
                    b=0+m;

                } else if(hp>1 && hp<=2) {
                    r=x+m;
                    g=c+m;
                    b=0+m;

                } else if(hp>2 && hp<=3) {
                    r=0+m;
                    g=c+m;
                    b=x+m;

                } else if(hp>3 && hp<=4) {
                    r=0+m;
                    g=x+m;
                    b=c+m;

                } else if(hp>4 && hp<=5) {
                    r=x+m;
                    g=0+m;
                    b=c+m;
                } else if(hp>5 && hp<=6) {
                    r=c+m;
                    g=0+m;
                    b=x+m;
                } else {
                    r=0+m;
                    g=0+m;
                    b=0+m;

                }
                drawPixel(i,512-j,r*255,g*255,b*255);
            }
        }
    } else if(jakikolor==2) {
        double r,g,b;
        for(int i=0;i<height;i++) {
            for(int j=0;j<width;j++) {
                double h=static_cast<double>(j)*static_cast<double>(360)/static_cast<double>(512);
                double hp=static_cast<double>(h)/static_cast<double>(60);
                double pom = fmod(hp,static_cast<double>(2))-static_cast<double>(1);
                if(pom<0) {
                    pom=-1*pom;
                }
                double c=(static_cast<double>(ffs)/static_cast<double>(100))*(static_cast<double>(i)/static_cast<double>(width));
                double x=c*(1-pom);
                double m=static_cast<double>(i)/512-c;
                if(hp>=0 && hp<=1) {
                    r=c+m;
                    g=x+m;
                    b=0+m;

                } else if(hp>1 && hp<=2) {
                    r=x+m;
                    g=c+m;
                    b=0+m;

                } else if(hp>2 && hp<=3) {
                    r=0+m;
                    g=c+m;
                    b=x+m;

                } else if(hp>3 && hp<=4) {
                    r=0+m;
                    g=x+m;
                    b=c+m;

                } else if(hp>4 && hp<=5) {
                    r=x+m;
                    g=0+m;
                    b=c+m;
                } else if(hp>5 && hp<=6) {
                    r=c+m;
                    g=0+m;
                    b=x+m;
                } else {
                    r=0+m;
                    g=0+m;
                    b=0+m;

                }
                drawPixel(512-j,512-i,r*255,g*255,b*255);
            }

        }

    } else if(jakikolor==3) {
        double r,g,b;
        for(int i=0;i<height;i++) {
            for(int j=0;j<width;j++) {
                double h=static_cast<double>(j)*static_cast<double>(360)/static_cast<double>(512);
                double hp=static_cast<double>(h)/static_cast<double>(60);
                double pom = fmod(hp,static_cast<double>(2))-static_cast<double>(1);
                if(pom<0) {
                    pom=-1*pom;
                }
                double c=(static_cast<double>(ffv)/static_cast<double>(100))*(static_cast<double>(i)/static_cast<double>(width));
                double x=c*(1-pom);
                double m=static_cast<double>(ffv)/static_cast<double>(100)-c;
                if(hp>=0 && hp<=1) {
                    r=c+m;
                    g=x+m;
                    b=0+m;

                } else if(hp>1 && hp<=2) {
                    r=x+m;
                    g=c+m;
                    b=0+m;

                } else if(hp>2 && hp<=3) {
                    r=0+m;
                    g=c+m;
                    b=x+m;

                } else if(hp>3 && hp<=4) {
                    r=0+m;
                    g=x+m;
                    b=c+m;

                } else if(hp>4 && hp<=5) {
                    r=x+m;
                    g=0+m;
                    b=c+m;
                } else if(hp>5 && hp<=6) {
                    r=c+m;
                    g=0+m;
                    b=x+m;
                } else {
                    r=0+m;
                    g=0+m;
                    b=0+m;

                }
                drawPixel(512-j,512-i,r*255,g*255,b*255);
            }
        }
    }
    update();
}
//*****************************************************************************************************

void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    ffred=position;
    jakikolor=1;
    rysujRGB();
}
void MainWindow::on_horizontalSlider_2_sliderMoved(int position)
{
    ffgreen=position;
    jakikolor=2;
    rysujRGB();
}

void MainWindow::on_horizontalSlider_3_sliderMoved(int position)
{
    ffblue=position;
    jakikolor=3;
    rysujRGB();
}

void MainWindow::on_horizontalSlider_4_sliderMoved(int position)
{
    ffh=position;
    jakikolor=1;
    rysujHSV();
}

void MainWindow::on_horizontalSlider_5_sliderMoved(int position)
{
    ffs=position;
    jakikolor=2;
    rysujHSV();
}

void MainWindow::on_horizontalSlider_6_sliderMoved(int position)
{
    ffv=position;
    jakikolor=3;
    rysujHSV();
}
