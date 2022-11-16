//Michal Sobczak, morphology
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
    nowyObraz="";
    // Tworzymy obiekt klasy QImage o wymiarach równych wymiarom ramki
    // Format_RGB32 to 32 bitowe RGB (z kanałem alfa ustawionym na wartość 255)
    img = new QImage(width, height, QImage::Format_RGB32);
    //https://wall.alphacoders.com/big.php?i=249186
    obraz = new QImage(":/249186");
    *img = obraz->copy(0,0,width,height);
    czarnoBialy(img);
    update();

    //qDebug() << startX;
    //qDebug() << startY;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete img;
    delete obraz;
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

void MainWindow::wczytaj(QString obraz1) {
    obraz = new QImage(obraz1);
    *img = obraz->copy(0,0,width,height);
    czarnoBialy(img);
    update();
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

// Funkcja zamalowująca na biało wszystkie piksele obrazu *img
void MainWindow::clean()
{
    if(!(zapisany.isEmpty())) {
        qDebug() << zapisany;
        obraz = new QImage(zapisany);
    } else {
        obraz = new QImage(":/249186");
    }
    *img = obraz->copy(0,0,width,height);
    czarnoBialy(img);
    update();
}

void MainWindow::czarnoBialy(QImage *img) {
    unsigned char *ptr;
    ptr = img->bits();
    int r,g,b;
    int szary;
    for(int i=0;i<height;i++) {
        for(int j=0;j<width;j++) {
            r= ptr[width*4*i + 4*j +2];
            g= ptr[width*4*i + 4*j +1];
            b= ptr[width*4*i + 4*j];
            szary = static_cast<int>(round(0.2126*r+0.7152*g+0.0722*b));
            if(szary<128) {
                szary=0;
            } else {
                szary=255;
            }
            ptr[width*4*i + 4*j +2]=szary;
            ptr[width*4*i + 4*j +1]=szary;
            ptr[width*4*i + 4*j]=szary;
        }
    }
}

void MainWindow::dylatacja(QImage *picture) {
    QImage *img_temp;
    unsigned char *ptr, *ptr2;
    int tlo = 255;
    int plan = 0;
    int kolor;
    img_temp = new QImage(width,height,QImage::Format_RGB32);
    *img_temp = img->copy(0,0,width,height);
    ptr = picture->bits();
    ptr2 = img_temp->bits();
    if(maska==0) {
        for(int i=0;i<height;i++) {
            for(int j=0;j<width;j++) {
                kolor = tlo;
                ///qDebug() << pom;
                if(width*4*(i-1)+4*j>=0 && width*4*(i-1)+4*j<width*height*4) {
                    if(ptr2[width*4*(i-1)+4*j]==plan) {
                        kolor=plan;
                    }
                }
                if(width*4*(i+1)+4*j>=0 && width*4*(i+1)+4*j<width*height*4) {
                    if(ptr2[width*4*(i+1)+4*j]==plan) {
                        kolor=plan;
                    }
                }
                if(width*4*i+4*(j+1)>=0 && width*4*i+4*(j+1)<width*height*4) {
                    if(ptr2[width*4*i+4*(j+1)]==plan) {
                        kolor=plan;
                    }
                }
                if(width*4*i+4*(j-1)>=0 && width*4*i+4*(j-1)<width*height*4) {
                    if(ptr2[width*4*i+4*(j-1)]==plan) {
                        kolor=plan;
                    }
                }

                ptr[width*4*i+4*j+2]=kolor;
                ptr[width*4*i+4*j+1]=kolor;
                ptr[width*4*i+4*j]=kolor;
            }
        }

    } else {
        for(int i=0;i<height;i++) {
            for(int j=0;j<width;j++) {
                kolor = tlo;
                for(int k=i-1;k<=i+1;k++) {
                    if(k>=0 && k<height) {
                        for(int l=j-1;l<=j+1;l++) {
                            if(l>=0 && l<width) {
                                if(ptr2[width*4*k + 4*l]==plan) {
                                    kolor=plan;
                                    break;
                                }
                            }
                        }
                    }
                }
                ptr[width*4*i+4*j+2]=kolor;
                ptr[width*4*i+4*j+1]=kolor;
                ptr[width*4*i+4*j]=kolor;
            }
        }
    }

    delete img_temp;
}

void MainWindow::erozja(QImage *picture) {
        QImage *img_temp;
        unsigned char *ptr, *ptr2;
        int tlo = 255;
        int plan = 0;
        int kolor;
        img_temp = new QImage(width,height,QImage::Format_RGB32);
        *img_temp = img->copy(0,0,width,height);
        ptr = picture->bits();
        ptr2 = img_temp->bits();
        if(maska==0) {
            for(int i=0;i<height;i++) {
                for(int j=0;j<width;j++) {
                    kolor = plan;
                    ///qDebug() << pom;
                    if(width*4*(i-1)+4*j>=0 && width*4*(i-1)+4*j<width*height*4) {
                        if(ptr2[width*4*(i-1)+4*j]==tlo) {
                            kolor=tlo;
                        }
                    }
                    if(width*4*(i+1)+4*j>=0 && width*4*(i+1)+4*j<width*height*4) {
                        if(ptr2[width*4*(i+1)+4*j]==tlo) {
                            kolor=tlo;
                        }
                    }
                    if(width*4*i+4*(j+1)>=0 && width*4*i+4*(j+1)<width*height*4) {
                        if(ptr2[width*4*i+4*(j+1)]==tlo) {
                            kolor=tlo;
                        }
                    }
                    if(width*4*i+4*(j-1)>=0 && width*4*i+4*(j-1)<width*height*4) {
                        if(ptr2[width*4*i+4*(j-1)]==tlo) {
                            kolor=tlo;
                        }
                    }

                    ptr[width*4*i+4*j+2]=kolor;
                    ptr[width*4*i+4*j+1]=kolor;
                    ptr[width*4*i+4*j]=kolor;
                }
            }

    } else {
        for(int i=0;i<height;i++) {
            for(int j=0;j<width;j++) {
                kolor = plan;
                for(int k=i-1;k<=i+1;k++) {
                    if(k>=0 && k<height) {
                        for(int l=j-1;l<=j+1;l++) {
                            if(l>=0 && l<width) {
                                if(ptr2[width*4*k + 4*l]==tlo) {
                                    kolor=tlo;
                                    break;
                                }
                            }
                        }
                    }
                }
                ptr[width*4*i+4*j+2]=kolor;
                ptr[width*4*i+4*j+1]=kolor;
                ptr[width*4*i+4*j]=kolor;
            }
        }
    }

    delete img_temp;
}

//*****************************************************************************************************

// Metoda wywoływana po nacisnięciu przycisku cleanButton (,,Czyść'')
void MainWindow::on_cleanButton_clicked()
{
    qDebug() << nowyObraz;
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

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    maska=index;
    qDebug()  << maska;
    update();
}

void MainWindow::on_pushButton_clicked()
{
    qDebug()  << "dylektacja";
    dylatacja(img);
    update();
}

void MainWindow::on_pushButton_2_clicked()
{
    erozja(img);
    update();
}

void MainWindow::on_pushButton_3_clicked()
{
    erozja(img);
    dylatacja(img);
    update();
}

void MainWindow::on_pushButton_4_clicked()
{
    dylatacja(img);
    erozja(img);
    update();
}

void MainWindow::on_pushButton_5_clicked()
{
    nowyObraz = QFileDialog::getOpenFileName(this,"Otwórz obraz", "..\\","Pliki graficzne (*.png *.jpg *.bmp)");
    qDebug() << nowyObraz;
    if(!(nowyObraz.isNull())) {
        zapisany=nowyObraz;
        wczytaj(nowyObraz);
        update();
    }
}
