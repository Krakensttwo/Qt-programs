//Michal Sobczak, alpha blending
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
        // Jeżeli wciśnięto lewy przycisk to zamalowujemy piksel na czarno
        if(event->button() == Qt::LeftButton)
        {
            drawPixel(x,y,0,0,0);
        }
        // a w przeciwnym wypadku na czerwono
        else
        {
            drawPixel(x,y,255,0,0);
        }
        update();
    } else {
        xOdc0=x;
        yOdc0=y;
        if(odcinekLubPunkt==5) {
            unsigned char *wsk;
            wsk = img->scanLine(y);
            ffred=wsk[4*x+2];
            ffgreen=wsk[4*x+1];
            ffblue=wsk[4*x];
            qDebug() << ffred;
            qDebug() << ffgreen;
            qDebug() << ffblue;
            rysujFloodfill(x,y);
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
        xOdc1=x;
        yOdc1=y;
        rysujOdcinek(xOdc0,yOdc0,xOdc1,yOdc1);
    } else if(odcinekLubPunkt==3) {
        x = event->pos().x();
        y = event->pos().y();
        x = x - startX;
        y = y - startY;
        xOdc1=x;
        yOdc1=y;
        rysujOkrag(xOdc0,yOdc0,xOdc1,yOdc1);
    } else if(odcinekLubPunkt==4) {
        x = event->pos().x();
        y = event->pos().y();
        x = x - startX;
        y = y - startY;
        xOdc1=x;
        yOdc1=y;
        rysujElipse(xOdc0,yOdc0,xOdc1,yOdc1);
    }

}

//*****************************************************************************************************


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

void MainWindow::rysujOdcinek(int x0, int y0, int x1, int y1) {
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
            drawPixel(x0,i,0,0,0);
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
                drawPixel(static_cast<int>(round(x)),y,0,0,0);
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
                drawPixel(x,static_cast<int>(round(y)),0,0,0);
            }
        }

    }
    update();
}
//*****************************************************************************************************
void MainWindow::drawOsiemPixel(int x, int y, int x1, int y1) {
    qDebug()  << x1 << y1 << x << y;
    drawPixel(x1+x,y1-y,255,0,0);
    drawPixel(x1+x,y1+y,255,0,0);
    drawPixel(x1-x,y1+y,255,0,0);
    drawPixel(x1-x,y1-y,255,0,0);
    drawPixel(x1+y,y1+x,255,0,0);
    drawPixel(x1+y,y1-x,255,0,0);
    drawPixel(x1-y,y1+x,255,0,0);
    drawPixel(x1-y,y1-x,255,0,0);
}

void MainWindow::rysujOkrag(int x0, int y0, int x1, int y1) {
    rOkrag=static_cast<double>(sqrt((x1-x0)*(x1-x0)+(y1-y0)*(y1-y0)));
    qDebug()<<"rys okrag x0: " << x0 <<" y0: "<< y0 << " x1: "<< x1 << " y1: " <<y1;
    for(int x=x0;x<=x0+static_cast<int>(rOkrag/sqrt(2));x++) {
        int y=static_cast<int>(sqrt(rOkrag*rOkrag-(x-x0)*(x-x0)));
        drawOsiemPixel(x-x0,y,x0,y0);
    }
    update();
}
//*****************************************************************************************************
void MainWindow::drawCzteryPixel(int x, int y, int x1, int y1) {
    qDebug()  << x1 << y1 << x << y;
    drawPixel(x1+x,y1-y,0,255,0);
    drawPixel(x1+x,y1+y,0,255,0);
    drawPixel(x1-x,y1+y,0,255,0);
    drawPixel(x1-x,y1-y,0,255,0);
}

void MainWindow::rysujElipse(int x0, int y0, int x1, int y1) {
    for(double t=0.0;t<=2*3.14;t=t+0.01) {

        int x=abs(x1-x0)*cos(t);
        int y=abs(y1-y0)*sin(t);

        drawCzteryPixel(x,y,x0,y0);
    }
    if(wpisany==1) {
        for(double t=0.0;t<=2*3.14;t=t+2*3.14/iloscWierzcholkow) {
            int x=abs(x1-x0)*cos(t);
            int y=abs(y1-y0)*sin(t);
            qDebug() << wpisany;
                if(bbb==0) {
                    xOdc0=x0-x;
                    xEli=x0-x;
                    yOdc0=y0-y;
                    yEli=y0-y;
                    bbb++;
                } else if(bbb!=iloscWierzcholkow){
                    xOdc1=x0-x;
                    yOdc1=y0-y;
                    rysujOdcinek(xOdc0,yOdc0,xOdc1,yOdc1);
                    //drawPixel(x1+x,y1-y,0,255,0);
                    xOdc0=xOdc1;
                    yOdc0=yOdc1;
                    bbb++;
                } else {
                    rysujOdcinek(xOdc0,yOdc0,xEli,yEli);
                    //drawPixel(x1+x,y1-y,0,255,0);

                }
            }
        }
    bbb=0;
    update();
}
//*****************************************************************************************************
void MainWindow::rysujFloodfill(int x, int y) {
    QStack<ffpunkt> stack;
    ffpunkt st;
    st.fx =x;
    st.fy =y;
    stack.push(st);
    int w,e;
    while(!stack.isEmpty()) {
        unsigned char *wsk;
        unsigned char *wsk1;
        ffpunkt pomp = stack.pop();
        wsk = img->scanLine(pomp.fy);
        if(pomp.fx>=0 && pomp.fy>=0 && pomp.fx<width && pomp.fy<height) {
            if(ffred==wsk[4*pomp.fx+2] && ffgreen==wsk[4*pomp.fx+1] && ffblue==wsk[4*pomp.fx]) {
                ///qDebug() << "tak";
                w=pomp.fx;
                e=pomp.fx;
                while(ffred==wsk[4*w+2] && ffgreen==wsk[4*w+1] && ffblue==wsk[4*w] && w>=0) {
                    w--;
                }
                while(ffred==wsk[4*e+2] && ffgreen==wsk[4*e+1] && ffblue==wsk[4*e] && e<width) {
                    e++;
                }
                for(int i=w+1;i<e;i++) {
                    drawPixel(i,pomp.fy,155,110,214);
                }
                wsk  = img->scanLine(pomp.fy+1);
                wsk1 = img->scanLine(pomp.fy-1);
                for(int i=w+1;i<e;i++) {
                    if(ffred==wsk[4*i+2] && ffgreen==wsk[4*i+1] && ffblue==wsk[4*i]){
                        ffpunkt nowy = {i,pomp.fy+1};
                        stack.push(nowy);
                    }
                    if(ffred==wsk1[4*i+2] && ffgreen==wsk1[4*i+1] && ffblue==wsk1[4*i]){
                        ffpunkt nowy = {i,pomp.fy-1};
                        stack.push(nowy);
                    }
                }
            }
        }
    }
    update();
}



//*****************************************************************************************************
void MainWindow::on_radioButton_clicked()
{
    odcinekLubPunkt=2;
}

void MainWindow::on_radioButton_2_clicked()
{
    odcinekLubPunkt=1;
}

void MainWindow::on_radioButton_3_clicked()
{
    odcinekLubPunkt=3;
}

void MainWindow::on_radioButton_4_clicked()
{
    odcinekLubPunkt=4;
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    ui->lineEdit->setText(QString::number(value));
    iloscWierzcholkow=value;
}

void MainWindow::on_checkBox_stateChanged(int arg1)
{
    if(arg1) {
        wpisany=1;
    } else {
        wpisany=0;
    }
}

void MainWindow::on_radioButton_5_clicked()
{
     odcinekLubPunkt=5;
}
