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
    //wszystkie obrazy pochodza ze strony wallpaper abyss
    obrazy[0] = new QImage(":/249186");
    obrazy[1] = new QImage(":/692334");
    obrazy[2] = new QImage(":/1168402");
    obrazy[3] = new QImage(":/Szkielet");
    obrazy[4] = new QImage(":/Wrak");

    clean();
    czysc(img);
    update();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete img;
    for(int i=0;i<5;i++) {
        delete obrazy[i];
    }
    delete [] *obrazy;
}

//*****************************************************************************************************

// Funkcja ,,odrysowująca'' okno aplikacji. Wywoływana automatycznie podczas uruchamiania aplikacji
// i w innych sytuacjach, w których zachodzi konieczność ponownego narysowania okna.
// Można też wymusić jej wywołanie za pomocą funkcji update()
void MainWindow::paintEvent(QPaintEvent*)
{
    // Obiekt przy pomocy, którego możemy rysować po elementach interfejsu graficznego aplikacji
    QPainter p(this);
    rysujObraz();
    // Metoda drawImage wyświetla zawartość obiektu *img klasy QImage w głównym oknie aplikacji
    // startX i startY to współrzędne (względem głównego okna) lewego górnego narożnika
    // wyświetlanego obiektu
    p.drawImage(startX, startY, *img);
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

// Funkcja zamalowująca na biało wszystkie piksele obrazu *img
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
    for(int i=0;i<5;i++) {
        alpha_wartosci[i]=0.0;
        aktywny[i]=0;
        tryb[i]=0;
    }
    ui->checkBox->setChecked(false);
    ui->checkBox_2->setChecked(false);
    ui->checkBox_3->setChecked(false);
    ui->checkBox_4->setChecked(false);
    ui->checkBox_5->setChecked(false);

    ui->horizontalSlider->setValue(0);
    ui->horizontalSlider_2->setValue(0);
    ui->horizontalSlider_3->setValue(0);
    ui->horizontalSlider_4->setValue(0);
    ui->horizontalSlider_5->setValue(0);

    ui->comboBox->setCurrentIndex(0);
    ui->comboBox_2->setCurrentIndex(0);
    ui->comboBox_3->setCurrentIndex(0);
    ui->comboBox_4->setCurrentIndex(0);
    ui->comboBox_5->setCurrentIndex(0);

    czysc(img);
    update();
}

//*****************************************************************************************************

void MainWindow::czysc(QImage *picture) {
    unsigned char *ptr;
    ptr = picture->bits();
    for(int i=0;i<height;i++) {
        for(int j=0;j<width;j++) {
            ptr[width*4*i + 4*j +2] =255;
            ptr[width*4*i + 4*j +1] =255;
            ptr[width*4*i + 4*j] =255;
        }
    }
}
//*****************************************************************************************************
void MainWindow::rysujObraz() {
    QImage *temp_image = new QImage(width,height,QImage::Format_RGB32);
    czysc(temp_image);
    int t=0;
    for(int k=0;k<5;k++) {
        if(aktywny[k]) {
            blend(obrazy[k],temp_image,alpha_wartosci[k],tryb[k]);
            *temp_image = img->copy(0,0,width,height);
            t++;
        }
    }
    if(t==0) {
        czysc(img);
    }
    delete temp_image;
}
//*****************************************************************************************************

void MainWindow::blend(QImage *picture, QImage *background,double alpha,int mode) {
    unsigned char *ptr1, *ptr2, *main_ptr;
    main_ptr = img->bits();
    ptr1 = picture->bits();
    ptr2 = background->bits();
    int pom=0;
    int r,g,b;
    if(mode==0) {//normal
        for(int i=0; i<height;i++) {
            for(int j=0;j<width;j++) {
                pom=width*4*i+4*j;
                main_ptr[pom+2]= alpha*ptr1[pom+2]+(1.0-alpha)*ptr2[pom+2];
                main_ptr[pom+1]= alpha*ptr1[pom+1]+(1.0-alpha)*ptr2[pom+1];
                main_ptr[pom]= alpha*ptr1[pom]+(1.0-alpha)*ptr2[pom];
            }
        }
    } else if(mode==1) {//multiply
        for(int i=0; i<height;i++) {
            for(int j=0;j<width;j++) {
                pom=width*4*i+4*j;
                r=(ptr1[pom+2]*ptr2[pom+2]) >> 8;
                g=(ptr1[pom+1]*ptr2[pom+1]) >> 8;
                b=(ptr1[pom]*ptr2[pom]) >> 8;
                main_ptr[pom+2]= alpha*r+(1.0-alpha)*ptr2[pom+2];
                main_ptr[pom+1]= alpha*g+(1.0-alpha)*ptr2[pom+1];
                main_ptr[pom]= alpha*b+(1.0-alpha)*ptr2[pom];
            }
        }

    } else if(mode==2) { //darken
        for(int i=0; i<height;i++) {
            for(int j=0;j<width;j++) {
                pom=width*4*i+4*j;
                if(ptr1[pom+2]>ptr2[pom+2]) {
                    r=ptr2[pom+2];
                } else {
                    r=ptr1[pom+2];
                }
                if(ptr1[pom+1]>ptr2[pom+1]) {
                    g=ptr2[pom+1];
                } else {
                    g=ptr1[pom+1];
                }
                if(ptr1[pom]>ptr2[pom]) {
                    b=ptr2[pom];
                } else {
                    b=ptr1[pom];
                }
                main_ptr[pom+2]= alpha*r+(1.0-alpha)*ptr2[pom+2];
                main_ptr[pom+1]= alpha*g+(1.0-alpha)*ptr2[pom+1];
                main_ptr[pom]= alpha*b+(1.0-alpha)*ptr2[pom];
            }
        }

    } else if(mode==3) {//lighten
        for(int i=0; i<height;i++) {
            for(int j=0;j<width;j++) {
                pom=width*4*i+4*j;
                if(ptr1[pom+2]<ptr2[pom+2]) {
                    r=ptr2[pom+2];
                } else {
                    r=ptr1[pom+2];
                }
                if(ptr1[pom+1]<ptr2[pom+1]) {
                    g=ptr2[pom+1];
                } else {
                    g=ptr1[pom+1];
                }
                if(ptr1[pom]<ptr2[pom]) {
                    b=ptr2[pom];
                } else {
                    b=ptr1[pom];
                }
                main_ptr[pom+2]= alpha*r+(1.0-alpha)*ptr2[pom+2];
                main_ptr[pom+1]= alpha*g+(1.0-alpha)*ptr2[pom+1];
                main_ptr[pom]= alpha*b+(1.0-alpha)*ptr2[pom];
            }
        }
    } else if(mode==4) {//average
        for(int i=0; i<height;i++) {
            for(int j=0;j<width;j++) {
                pom=width*4*i+4*j;
                r=(ptr1[pom+2]+ptr2[pom+2])/2;
                g=(ptr1[pom+1]+ptr2[pom+1])/2;
                b=(ptr1[pom]+ptr2[pom])/2;
                main_ptr[pom+2]= alpha*r+(1.0-alpha)*ptr2[pom+2];
                main_ptr[pom+1]= alpha*g+(1.0-alpha)*ptr2[pom+1];
                main_ptr[pom]= alpha*b+(1.0-alpha)*ptr2[pom];
            }
        }
    }
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


void MainWindow::on_checkBox_stateChanged(int arg1)
{
    if(arg1==Qt::Checked) {
        aktywny[4]=1;
        alpha_wartosci[4] = ui->horizontalSlider->value()/100.0;
    } else {
        aktywny[4]=0;
        alpha_wartosci[4] = 0;
    }
    rysujObraz();
    update();
}

void MainWindow::on_checkBox_2_stateChanged(int arg1)
{
    if(arg1==Qt::Checked) {
        aktywny[3]=1;
        alpha_wartosci[3] = ui->horizontalSlider_2->value()/100.0;
    } else {
        aktywny[3]=0;
        alpha_wartosci[3] = 0;
    }
    rysujObraz();
    update();
}

void MainWindow::on_checkBox_4_stateChanged(int arg1)
{
    if(arg1==Qt::Checked) {
        aktywny[2]=1;
        alpha_wartosci[2] = ui->horizontalSlider_3->value()/100.0;
    } else {
        aktywny[2]=0;
        alpha_wartosci[2] = 0;
    }
    rysujObraz();
    update();
}

void MainWindow::on_checkBox_3_stateChanged(int arg1)
{
    if(arg1==Qt::Checked) {
        aktywny[1]=1;
        alpha_wartosci[1] = ui->horizontalSlider_4->value()/100.0;
    } else {
        aktywny[1]=0;
        alpha_wartosci[1] = 0;
    }
    rysujObraz();
    update();
}

void MainWindow::on_checkBox_5_stateChanged(int arg1)
{
    if(arg1==Qt::Checked) {
        aktywny[0]=1;
        alpha_wartosci[0] = ui->horizontalSlider_5->value()/100.0;
    } else {
        aktywny[0]=0;
        alpha_wartosci[0] = 0;
    }
    rysujObraz();
    update();
}
//*****************************************************************************************************

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    tryb[4]=index;
    rysujObraz();
    update();
}

void MainWindow::on_comboBox_2_currentIndexChanged(int index)
{
    tryb[3]=index;
    rysujObraz();
    update();
}

void MainWindow::on_comboBox_4_currentIndexChanged(int index)
{
    tryb[2]=index;
    rysujObraz();
    update();
}

void MainWindow::on_comboBox_3_currentIndexChanged(int index)
{
    tryb[1]=index;
    rysujObraz();
    update();
}

void MainWindow::on_comboBox_5_currentIndexChanged(int index)
{
    tryb[0]=index;
    rysujObraz();
    update();
}
//*****************************************************************************************************

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    alpha_wartosci[4]=static_cast<double>(value)/100.0;
    rysujObraz();
    update();
}

void MainWindow::on_horizontalSlider_2_valueChanged(int value)
{
    alpha_wartosci[3]=static_cast<double>(value)/100.0;
    rysujObraz();
    update();
}

void MainWindow::on_horizontalSlider_3_valueChanged(int value)
{
    alpha_wartosci[2]=static_cast<double>(value)/100.0;
    rysujObraz();
    update();
}

void MainWindow::on_horizontalSlider_4_valueChanged(int value)
{
    alpha_wartosci[1]=static_cast<double>(value)/100.0;
    rysujObraz();
    update();
}

void MainWindow::on_horizontalSlider_5_valueChanged(int value)
{
    alpha_wartosci[0]=static_cast<double>(value)/100.0;
    rysujObraz();
    update();
}
