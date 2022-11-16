//Michal Sobczak, morphing
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
    startX1 = ui->frame->x();
    startY1 = ui->frame->y();

    startX2 = ui->frame_2->x();
    startY2 = ui->frame_2->y();

    startX3 = ui->frame_3->x();
    startY3 = ui->frame_3->y();


    // Pobieramy wymiary ramki
    width = ui->frame->width();
    height = ui->frame->height();
    // Tworzymy obiekt klasy QImage o wymiarach równych wymiarom ramki
    // Format_RGB32 to 32 bitowe RGB (z kanałem alfa ustawionym na wartość 255)
    img = new QImage(width, height, QImage::Format_RGB32);
    obraz = new QImage(width, height, QImage::Format_RGB32);
    wynikowy = new QImage(width, height, QImage::Format_RGB32);
    //https://wall.alphacoders.com/big.php?i=1168402
    img->load(":/1168402");
    //https://www.hippopx.com/en/dinosaur-bones-rex-toy-grass-reptile-animal-117680
    obraz -> load(":/Szkielet.jpg");
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
    p.drawImage(startX1, startY1, *img);
    p.drawImage(startX2, startY2, *obraz);
    p.drawImage(startX3, startY3, *wynikowy);
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

    } else if(x>=520 && y>=0 && x<1040 && y<height) {
        x=x-8;
        wsk = obraz->scanLine(y);
        // zmieniamy składowe piksela x
        wsk[4*x] = blue;
        wsk[4*x+1] = green;
        wsk[4*x+2] = red;
    }

}


void MainWindow::rysujDuzyPunkt(int x, int y) {
    for(int i=-3;i<4;i++) {
        for(int j=-3;j<4;j++) {
            drawPixel(x+i,y+j,255,0,0);
        }
    }
}
void MainWindow::mousePressEvent(QMouseEvent *event)
{

    int x,y;
    // Pobieramy współrzędne punktu kliknięcia
    x = event->pos().x();
    y = event->pos().y();
    //qDebug() <<x <<y;
    // Współrzędne obliczane są względem głównego okna programu
    // aby uzyskać współrzędne względem obszaru rysowania (ramki) musimy je przesunąć
    // tak aby punkt (0,0) pokrywał się z lewym górnym naroznikiem obszaru rysowania

    x = x - startX1;
    y = y - startY1;
    //qDebug() <<x <<y;
    if(x>=520 && y>=0 && x<1040 && y<height && dwa.size()<3) {
        ffpunkt nowy={x,y};
        if(dwa.size()<1) {
            dwa.push_back(nowy);
            rysujDuzyPunkt(x,y);
        } else {
            ffpunkt pierwszy = dwa.at(0);
            dwa.push_back(nowy);
            rysujDuzyPunkt(x,y);
            if(dwa.size()>1 && dwa.size()<4) {
                ffpunkt poprzedni = dwa.at(dwa.size()-2);
                rysujOdcinek(poprzedni.x,poprzedni.y,x,y,255,0,0);
            }
            if(dwa.size()==3) {
                ffpunkt ostatni =dwa.at(dwa.size()-1);
                rysujOdcinek(ostatni.x,ostatni.y,pierwszy.x,pierwszy.y,255,0,0);
            }
        }
    }
    if(x>=0 && y>=0 && x<width && y<height && jeden.size()<3) {
        ffpunkt nowy={x,y};
        if(jeden.size()<1) {
            jeden.push_back(nowy);
            rysujDuzyPunkt(x,y);
        } else {
            ffpunkt pierwszy = jeden.at(0);
            jeden.push_back(nowy);
            rysujDuzyPunkt(x,y);
            if(jeden.size()>1 && jeden.size()<4) {
                ffpunkt poprzedni = jeden.at(jeden.size()-2);
                rysujOdcinek(poprzedni.x,poprzedni.y,x,y,255,0,0);
            }
            if(jeden.size()==3) {
                ffpunkt ostatni =jeden.at(jeden.size()-1);
                rysujOdcinek(ostatni.x,ostatni.y,pierwszy.x,pierwszy.y,255,0,0);
            }
        }
    }
    update();

}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    int x,y;
    x = event->pos().x();
    y = event->pos().y();
    x = x - startX1;
    y = y - startY1;

}

//*****************************************************************************************************

// Funkcja zamalowująca na biało wszystkie piksele obrazu *img
void MainWindow::clean()
{
    // deklaracja wskaźnika do poruszania się po pikselach obrazu
    unsigned char *wsk;

    // przechodzimy po wszystkich wierszach obrazu
    for(int i=0; i<height; i++)
    {
        // ustawiamy wskaźnik na początku i-tego wiersza
        wsk = wynikowy->scanLine(i);

        // przechodzimy po wszystkich piselach i-tego wiersza
        // i nadajemy im wartość (0,0,0) odpowiadającą kolorowi czarnemu
        for(int j=0; j<width; j++)
        {
            wsk[4*j] = 0;   // ustawiamy składową ,,niebieską'' na 255
            wsk[4*j+1] = 0; // ustawiamy składową ,,zieloną'' na 255
            wsk[4*j+2] = 0; // ustawiamy składową ,,czerwoną'' na 255
        }
    }
}


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

void MainWindow::animuj() {
    QImage *ptr1 = new QImage(width, height, QImage::Format_RGB32);
    QImage *ptr2 = new QImage(width, height, QImage::Format_RGB32);
    clean();
    if(!(zapisany1.isEmpty())) {
        ///qDebug() << zapisany1;
        QImage *temp;
        temp = new QImage(zapisany1);
        *ptr1 = temp->copy(0,0,width,height);
    } else {
        ptr1->load(":/1168402");
    }

    if(!(zapisany2.isEmpty())) {
        ///qDebug() << zapisany1;
        QImage *temp;
        temp = new QImage(zapisany2);
        *ptr2 = temp->copy(0,0,width,height);
    } else {
        ptr2 -> load(":/Szkielet.jpg");
    }
    unsigned char *wsk;
    unsigned char *wsk1;
    ffpunkt a=dwa.at(0);
    ffpunkt b=dwa.at(1);
    ffpunkt c=dwa.at(2);
    ffpunkt at=jeden.at(0);
    ffpunkt bt=jeden.at(1);
    ffpunkt ct=jeden.at(2);
    for(int n=0;n<=liczbaKlatek;n++) {
        qDebug() << n;
        ffpunkt aw;
        ffpunkt bw;
        ffpunkt cw;
        double pom=(1.0-(double)n/(double)liczbaKlatek)*(at.x+512)+(((double)n/(double)liczbaKlatek)*a.x);
        aw.x=round(pom);
        pom = (1.0-(double)n/(double)liczbaKlatek)*at.y+(((double)n/(double)liczbaKlatek)*a.y);
        aw.y=round(pom);
        pom = (1.0-(double)n/(double)liczbaKlatek)*(bt.x+512)+(((double)n/(double)liczbaKlatek)*b.x);
        bw.x=round(pom);
        pom = (1.0-(double)n/(double)liczbaKlatek)*bt.y+(((double)n/(double)liczbaKlatek)*b.y);
        bw.y=round(pom);
        pom = (1.0-(double)n/(double)liczbaKlatek)*(ct.x+512)+(((double)n/(double)liczbaKlatek)*c.x);
        cw.x=round(pom);
        pom = (1.0-(double)n/(double)liczbaKlatek)*ct.y+(((double)n/(double)liczbaKlatek)*c.y);
        cw.y=round(pom);
        /*
        if(n>110 && n<130) {
            qDebug() << at.x << aw.x << a.x;
            qDebug() << at.y << aw.y << a.y;
            qDebug() << bt.x << bw.x << b.x;
            qDebug() << bt.y << bw.y << b.y;
            qDebug() << ct.x << cw.x << c.x;
            qDebug() << ct.y << cw.y << c.y;
        }*/
        trzy.push_back(aw);
        trzy.push_back(bw);
        trzy.push_back(cw);
        int ymin=aw.y;
        int ymax=ymin;
        if(bw.y>ymax) {
            ymax=bw.y;
        }
        if(bw.y<ymin) {
            ymin=bw.y;
        }
        if(cw.y>ymax) {
            ymax=cw.y;
        }
        if(cw.y<ymin) {
            ymin=cw.y;
        }
        double wyznacznik = (bw.x-aw.x)*(cw.y-aw.y)-(bw.y-aw.y)*(cw.x-aw.x);

        ///qDebug() << wyznacznik;

        for(int i=ymin+1;i<ymax;i++) {
            int tabx[3];
            int k=0;
            for(int j=0;j<3;j++) {
                ffpunkt sc1=trzy.at(j);
                ffpunkt sc2;
                if(j==2) {
                    sc2=trzy.at(0);
                } else {
                    sc2=trzy.at(j+1);
                }
                //qDebug() << (sc2.y-sc1.y);
                if(!((sc1.y>i && sc2.y>=i) || (sc1.y<i && sc2.y<=i))) {
                    tabx[k]=sc1.x+(i-sc1.y)*(sc2.x-sc1.x)/(sc2.y-sc1.y);
                    k++;
                }
            }

            //sortowanie tablicy tabx
            for(int sb=0;sb<k;sb++) {
                for(int sv=1;sv<k-sb;sv++) {
                    if(tabx[sv-1]>tabx[sv]) {
                        int sn=tabx[sv-1];
                        tabx[sv-1]=tabx[sv];
                        tabx[sv]=sn;
                     }
                }
            }
            int r=0;
            int k1=0;
            double v;
            double w;
            double u;
            int r1=0,g1=0,b1=0;
            for(int j=520;j<1040;j++) {
                if(k1==k) {
                    break;
                }
                if(j==tabx[k1]) {
                    if(r==0) {
                        r=1;
                    } else {
                        r=0;
                    }
                }
                while(j==tabx[k1]) {
                    k1++;
                    if(k1==k) {
                        break;
                    }
                }
                if(r==1) {
                    v=(j-aw.x)*(cw.y-aw.y)-(i-aw.y)*(cw.x-aw.x);
                    w=(bw.x-aw.x)*(i-aw.y)-(bw.y-aw.y)*(j-aw.x);
                    v=v/wyznacznik;
                    w=w/wyznacznik;

                    u=1.0-v-w;
                    ///qDebug() << u+v+w;
                    double xt = u*at.x+v*bt.x+w*ct.x;
                    double xt1 = u*(a.x-512)+v*(b.x-512)+w*(c.x-512);
                    double yt = u*at.y+v*bt.y+w*ct.y;
                    double yt1 = u*a.y+v*b.y+w*c.y;
                    xt=round(xt);
                    yt=round(yt);
                    xt1=round(xt1);
                    yt1=round(yt1);
                    double pom;
                    if(yt1<=0 || xt1<=0 || yt<=0 || xt<=0) {
                        yt1=255;
                        xt1=255;
                        yt=255;
                        xt=255;
                    }

                    if(n>110 && n<130) {

                    }

                    wsk = ptr1->scanLine(yt);
                    wsk1 = ptr2->scanLine(yt1);
                    pom = (1.0-(double)n/(double)liczbaKlatek)*wsk[4*(int)xt+2] + ((double)n/(double)liczbaKlatek)*wsk1[4*(int)xt1+2];
                    r1 = round(pom);
                    pom = (1.0-(double)n/(double)liczbaKlatek)*wsk[4*(int)xt+1] + ((double)n/(double)liczbaKlatek)*wsk1[4*(int)xt1+1];
                    g1 = round(pom);
                    pom = (1.0-(double)n/(double)liczbaKlatek)*wsk[4*(int)xt] + ((double)n/(double)liczbaKlatek)*wsk1[4*(int)xt1];
                    b1 = round(pom);

                    wsk = wynikowy ->scanLine(i);
                    wsk[4*(j-8)+2]= r1;
                    wsk[4*(j-8)+1]= g1;
                    wsk[4*(j-8)]= b1;
                }
            }
        }
        repaint();
        delay(30);
        if(n!=liczbaKlatek) {
            clean();
        }
        while(trzy.size()) {
            trzy.pop_back();
        }

    }
}
//*****************************************************************************************************

// Metoda wywoływana po nacisnięciu przycisku cleanButton (,,Czyść'')
void MainWindow::on_cleanButton_clicked()
{
    clean();
    if(!(zapisany1.isEmpty())) {
        ///qDebug() << zapisany1;
        QImage *temp;
        temp = new QImage(zapisany1);
        *img = temp->copy(0,0,width,height);
    } else {
        img->load(":/1168402");
    }

    if(!(zapisany2.isEmpty())) {
        ///qDebug() << zapisany1;
        QImage *temp;
        temp = new QImage(zapisany2);
        *obraz = temp->copy(0,0,width,height);
    } else {
        obraz -> load(":/Szkielet.jpg");
    }

    while(dwa.size()) {
        dwa.pop_back();
    }
    while(jeden.size()) {
        jeden.pop_back();
    }
    while(trzy.size()) {
        trzy.pop_back();
    }
    // Po zmodyfikowaniu obiektu QImage odświeżamy okno aplikacji, aby zobaczyc zmiany
    repaint();
}

void MainWindow::wczytaj(QString obraz1, int jaki) {
    QImage *temp;
    if(jaki==0) {
        temp = new QImage(obraz1);
        *img = temp->copy(0,0,width,height);
    } else {
        temp = new QImage(obraz1);
        *obraz = temp->copy(0,0,width,height);
    }
   update();

}

void MainWindow::delay(int n) {
    QTime dieTime= QTime::currentTime().addMSecs(n);
    while(QTime::currentTime() < dieTime) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
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

void MainWindow::on_pushButton_clicked(){
    nowyObraz1 = QFileDialog::getOpenFileName(this,"Otwórz obraz", "..\\","Pliki graficzne (*.png *.jpg *.bmp)");
    qDebug() << nowyObraz1;
    if(!(nowyObraz1.isNull())) {
        zapisany1=nowyObraz1;
        wczytaj(nowyObraz1, 0);
        update();
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    nowyObraz2 = QFileDialog::getOpenFileName(this,"Otwórz obraz", "..\\","Pliki graficzne (*.png *.jpg *.bmp)");
    qDebug() << nowyObraz2;
    if(!(nowyObraz2.isNull())) {
        zapisany2=nowyObraz2;
        wczytaj(nowyObraz2, 1);
        update();
    }
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    liczbaKlatek=arg1;
}

void MainWindow::on_pushButton_3_clicked()
{
    if(jeden.size()==3 && dwa.size()==3) {
        animuj();
    }
}
