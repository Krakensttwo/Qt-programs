//Michal Sobczak, bezier curve
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
            wsk[4*j] = 255;   // ustawiamy składową ,,niebieską'' na 0
            wsk[4*j+1] = 255; // ustawiamy składową ,,zieloną'' na 0
            wsk[4*j+2] = 255; // ustawiamy składową ,,czerwoną'' na 0
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

void MainWindow::rysujDuzyPunkt(int x, int y) {
    for(int i=-3;i<4;i++) {
        for(int j=-3;j<4;j++) {
            drawPixel(x+i,y+j,0,0,255);
        }
    }
}
void MainWindow::rysujBeziera(int ktory,unsigned char red, unsigned char green, unsigned char blue) {
    for(int i=0;i<20;i++) {
        float t=static_cast<float>(i)/20;

        xB0=(1-t)*(1-t)*(1-t)*tabPunktX[ktory-4]+3*(1-t)*(1-t)*t*tabPunktX[ktory-3]+
            3*(1-t)*t*t*tabPunktX[ktory-2]+t*t*t*tabPunktX[ktory-1];
        yB0=(1-t)*(1-t)*(1-t)*tabPunktY[ktory-4]+3*(1-t)*(1-t)*t*tabPunktY[ktory-3]+
            3*(1-t)*t*t*tabPunktY[ktory-2]+t*t*t*tabPunktY[ktory-1];

        xB1=(1-(t+0.05))*(1-(t+0.05))*(1-(t+0.05))*tabPunktX[ktory-4]+3*(1-(t+0.05))*(1-(t+0.05))*(t+0.05)*tabPunktX[ktory-3]+
            3*(1-(t+0.05))*(t+0.05)*(t+0.05)*tabPunktX[ktory-2]+(t+0.05)*(t+0.05)*(t+0.05)*tabPunktX[ktory-1];
        yB1=(1-(t+0.05))*(1-(t+0.05))*(1-(t+0.05))*tabPunktY[ktory-4]+3*(1-(t+0.05))*(1-(t+0.05))*(t+0.05)*tabPunktY[ktory-3]+
            3*(1-(t+0.05))*(t+0.05)*(t+0.05)*tabPunktY[ktory-2]+(t+0.05)*(t+0.05)*(t+0.05)*tabPunktY[ktory-1];

        qDebug()<<xB0 << " " <<yB0<< " " <<xB1<< " " <<yB1;
        rysujOdcinek(static_cast<int>(round(xB0)),static_cast<int>(round(yB0)),static_cast<int>(round(xB1)),static_cast<int>(round(yB1)),red,green,blue);
    }

}
//*****************************************************************************************************
void MainWindow::rysujOdcinek(int x0, int y0, int x1, int y1, unsigned char red, unsigned char green, unsigned char blue) {
    int k;
    double a;
    double b;
    double c;
    //jezeli jest pionowa linia
    if(x0==x1) {
        if(y0>y1) {
            k=y0;
            y0=y1;
            y1=k;
        }
        for(int i=y0;i<=y1;i++) {//rysowanie pionowej lini
            drawPixel(x0,i,red,green,blue);
        }
    } else {
        //qDebug()<<"rys odc x0: " << x0 <<" y0: "<< y0 << " x1: "<< x1 << " y1: " <<y1;
        a=(static_cast<double>(y1)-static_cast<double>(y0))/(static_cast<double>(x1)-static_cast<double>(x0));
        b=static_cast<double>(y0)-a*static_cast<double>(x0);
        //qDebug()<<a << " " <<b;
        c=a;
        if(c>1 || c<-1) {
            //qDebug()<<"rys pion";
            if(y0>y1) {
                k=y0;
                y0=y1;
                y1=k;
            }
            for(int y=y0;y<=y1;y++) {
                double x=static_cast<double>((y-b)/a);
                //qDebug()<<x<<" "<<y;
                drawPixel(static_cast<int>(round(x)),y,red,green,blue);
            }
        } else {
            //qDebug()<<"rys poziom";
            if(x0>x1) {
                k=x0;
                x0=x1;
                x1=k;
            }
            for(int x=x0;x<=x1;x++) {
                double y=static_cast<double>(a*x+b);
                //qDebug()<<x<<" "<<y;
                drawPixel(x,static_cast<int>(round(y)),red,green,blue);
            }
        }

    }
    update();
}


// Metoda wywoływana po wciśnięciu przycisku myszy
// Inne metody związane z obsługą myszy to
// void MainWindow::mouseReleaseEvent(QMouseEvent *event)
// wywoływana po puszczeniu przycisku myszy i
// void MainWindow::mouseMoveEvent(QMouseEvent *event)
// wywoływana po każdym ruchu myszy
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    qDebug()<<odcinekLubPunkt;
    int x,y;
    // Pobieramy współrzędne punktu kliknięcia
    x = event->pos().x();
    y = event->pos().y();

    // Współrzędne obliczane są względem głównego okna programu
    // aby uzyskać współrzędne względem obszaru rysowania (ramki) musimy je przesunąć
    // tak aby punkt (0,0) pokrywał się z lewym górnym naroznikiem obszaru rysowania
    x = x - startX;
    y = y - startY;

    if(odcinekLubPunkt==1) {
        if(event->button() == Qt::LeftButton)
        {
            qDebug() << x << y;
            rysujDuzyPunkt(x,y);
            tabPunktX[iloscPunktow]=x;
            tabPunktY[iloscPunktow]=y;
            iloscPunktow++;
            ktoryPunkt++;
            if(ktoryPunkt==4) {
                rysujBeziera(iloscPunktow,0,0,0);
                ktoryPunkt=1;
            }

        }
        update();
    } else if(odcinekLubPunkt==2) {
        int k=0;
        int i;
        ///sprawdzenie czy trafiono
        for(i=0;i<iloscPunktow;i++) {
            if(x>=tabPunktX[i]-3 && x<=tabPunktX[i]+3 && y>=tabPunktY[i]-3 && y<=tabPunktY[i]+3) {
                trafiony=i;
                break;
            }
        }
    } else if(odcinekLubPunkt==3) {
        int k=0;
        int i;
        ///sprawdzenie czy trafiono
        for(i=0;i<iloscPunktow;i++) {
            if(x>=tabPunktX[i]-3 && x<=tabPunktX[i]+3 && y>=tabPunktY[i]-3 && y<=tabPunktY[i]+3) {
                k=-1;
                break;
            }
        }
        ///jezeli trafiono
        if(k<0) {
            ktoryPunkt=0;
            for(i;i<iloscPunktow;i++) {
                tabPunktX[i]=tabPunktX[i+1];
                tabPunktY[i]=tabPunktY[i+1];
            }
            tabPunktX[i+1]=0;
            tabPunktY[i+1]=0;
            iloscPunktow--;
            clean();
            //qDebug() << "ilosc "<<iloscPunktow;
            for(int j=0;j<iloscPunktow;j++) {
                qDebug() << "ktory "<< ktoryPunkt;
                rysujDuzyPunkt(tabPunktX[j],tabPunktY[j]);
                ktoryPunkt++;
                if(ktoryPunkt==4) {
                    //qDebug() << "ktory4 "<< j;
                    rysujBeziera(j+1,0,0,0);
                    ktoryPunkt=1;
                }
            }
            update();
        }
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    int x,y;
    //qDebug()<<odcinekLubPunkt;
    if(odcinekLubPunkt==2) {
        // Pobieramy współrzędne punktu puszczenia
        x = event->pos().x();
        y = event->pos().y();
        // Współrzędne obliczane są względem głównego okna programu
        // aby uzyskać współrzędne względem obszaru rysowania (ramki) musimy je przesunąć
        // tak aby punkt (0,0) pokrywał się z lewym górnym naroznikiem obszaru rysowania
        x = x - startX;
        y = y - startY;
        if(trafiony>=0) {
            ktoryPunkt=0;
            tabPunktX[trafiony]=x;
            tabPunktY[trafiony]=y;
            clean();
            //qDebug() << "ilosc "<<iloscPunktow;
            for(int j=0;j<iloscPunktow;j++) {
                qDebug() << "ktory "<< ktoryPunkt;
                rysujDuzyPunkt(tabPunktX[j],tabPunktY[j]);
                ktoryPunkt++;
                if(ktoryPunkt==4) {
                    //qDebug() << "ktory4 "<< j;
                    rysujBeziera(j+1,0,0,0);
                    ktoryPunkt=1;
                }
            }
            update();
        }

    }
}

//*****************************************************************************************************

// Metoda wywoływana po nacisnięciu przycisku cleanButton (,,Czyść'')
void MainWindow::on_cleanButton_clicked()
{
    clean();
    for(int i=0;i<iloscPunktow;i++) {
        tabPunktX[i]=0;
        tabPunktY[i]=0;
    }
    iloscPunktow=0;
    ktoryPunkt=0;
    trafiony=-1;

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
//****************************************************************************************************


//****************************************************************************************************
void MainWindow::on_radioButton_2_clicked()
{
    //dodanie
    odcinekLubPunkt=1;
}


void MainWindow::on_radioButton_clicked()
{
    //przeniesienie
    odcinekLubPunkt=2;
}

void MainWindow::on_radioButton_3_clicked()
{
    //usuwanie
    odcinekLubPunkt=3;
}
