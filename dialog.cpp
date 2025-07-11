
#include "dialog.h"
#include "ui_dialog.h"

int i=0,max_skor=0;
int ceza=0;
int kesilen=0;
QTimer *timer1,*timer2,*timer3,*timer4,*timer5,*timer6,*timer7;
QList<int> konumlarListesi;
QList<int> skorlarListesi;
Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);

    QFile konumlar(":/new/prefix2/documents/konumlar.txt");
    if(!konumlar.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(this,"hata",konumlar.errorString());
        return;
    }
    QTextStream my_stream(&konumlar);

    while(!my_stream.atEnd())
    {
        QString satir = my_stream.readLine();
        QStringList konummlar = satir.split(" ");
        int x = konummlar.at(0).toInt();
        int y = konummlar.at(1).toInt();
        konumlarListesi.append(x);
        konumlarListesi.append(y);

    }
    konumlar.close();


    timer1=new QTimer(this);
    timer1->start(1000);
    connect(timer1,SIGNAL(timeout()),this,SLOT(sureAzalt()));
    timer2=new QTimer(this);
    timer2->start(500);
    connect(timer2,SIGNAL(timeout()),this,SLOT(karpuzolustur()));
    timer3=new QTimer(this);
    timer3->start(50);
    connect(timer3,SIGNAL(timeout()),this,SLOT(hareketettir()));
    timer4=new QTimer(this);
    timer4->start(1500);
    connect(timer4,SIGNAL(timeout()),this,SLOT(karpuzsil()));
    timer5=new QTimer(this);
    timer5->start(3000);
    connect(timer5,SIGNAL(timeout()),this,SLOT(dinamitolustur()));
    timer6=new QTimer(this);
    timer6->start(50);
    connect(timer6,SIGNAL(timeout()),this,SLOT(dinamithareketettir()));
    timer7=new QTimer(this);
    timer7->start(3000);
    connect(timer7,SIGNAL(timeout()),this,SLOT(patlamakaldir()));



}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::dosyayayaz()
{
    QFile dosya(":/new/prefix2/documents/skorlar.txt");

    if (!dosya.open(QIODevice::WriteOnly | QIODevice::Append)) {
        QMessageBox::information(this,"Hata","Skor dosyaya kaydedilirken küçücük bir hata oluşmuş olabilir :'(\n" + dosya.errorString());
        return;
    }

    QTextStream stream(&dosya);
    stream << QString::number(kesilen-ceza);

    dosya.close();

}

int sure = 30;
void Dialog::sureAzalt()
{
    ui->sure->setText("<font color='blue' size='7'>"+QString::number(sure)+"</font>");
    if(sure==0){

        QFile skorlar(":/new/prefix2/documents/skorlar.txt");
        if(!skorlar.open(QIODevice::ReadOnly))
        {
            QMessageBox::critical(this,"hata",skorlar.errorString());
            return;
        }
        QTextStream my_stream2(&skorlar);

        while (!my_stream2.atEnd())
        {
            QString line = my_stream2.readLine();
            int skor = line.toInt();
            skorlarListesi.append(skor);
        }
        skorlar.close();
        for(int a=0; a < skorlarListesi.size();a++)
        {
            if(skorlarListesi[a]>max_skor)
            {
                max_skor=skorlarListesi[a];
            }
        }

        if((kesilen-ceza)>max_skor)
        {
            QMessageBox::information(this,"Bilgi!","Oyun Bitti! Tebrikler Maksimum Puan Sizde!\nToplam Puan: " + QString::number(kesilen-ceza) +"\nKesilen Karpuz Sayısı: "+QString::number(kesilen) +"\nKaçırılan Karpuz Sayısı: " + QString::number(geciciListe.size())+"\nPatlayan Dinamit Sayısı: " + QString::number(ceza/2) +"\nMaksimum Skor: "+QString::number(max_skor),"OK");
        }
        else
        {
            QMessageBox::information(this,"Bilgi!","Oyun Bitti! Maksimum Skoru Geçemediniz!\nToplam Puan: " + QString::number(kesilen-ceza) +"\nKesilen Karpuz Sayısı: "+ QString::number(kesilen) +"\nKaçırılan Karpuz Sayısı: " + QString::number(geciciListe.size())+"\nPatlayan Dinamit Sayısı: " + QString::number(ceza/2) +"\nMaksimum Skor: "+QString::number(max_skor),"OK");
        }

        dosyayayaz();
        timer1->stop();
        this->accept();

    }
    sure--;


}


void Dialog::karpuzolustur()
{

    QPushButton *karpuz=new QPushButton(this);
    QIcon *icon = new QIcon();
    icon->addPixmap(QPixmap(":/new/prefix1/images/1.png"),QIcon::Normal,QIcon::Off);
    karpuz->setIcon(*icon);
    karpuz->setIconSize(QSize(70,70));
    karpuz->setStyleSheet("background-color: rgba(255, 255, 255, 0); color: black;");
    karpuz->setGeometry(konumlarListesi[i],konumlarListesi[i+1],70,70);
    karpuz->show();
    KarpuzListesi.push_back(karpuz);
    ui->label_puan->setText("<font color='blue' size='7'>"+QString::number(kesilen-ceza)+"</font>");

    connect(karpuz, &QPushButton::clicked, [karpuz, this]() {
        KarpuzListesi.removeOne(karpuz);
        QIcon *icon2 = new QIcon();
        icon2->addPixmap(QPixmap(":/new/prefix1/images/2.png"), QIcon::Normal, QIcon::Off);
        karpuz->setIcon(*icon2);
        KesiliKarpuzlar.push_back(karpuz);
        karpuz->setStyleSheet("background-color: rgba(255, 255, 255, 0); color: black;");

        kesilen++;
        ui->label_kesilen_karpuz->setText("<font color='green' size='7'>"+QString::number(kesilen)+"</font>");

    });
    if(i+2 <= konumlarListesi.size()){
        i=i+2;
    }


}


void Dialog::hareketettir()
{
    for (int i=0; i<KarpuzListesi.size();++i)
    {
        if(!KesiliKarpuzlar.contains(KarpuzListesi[i])){
            KarpuzListesi[i]->setGeometry(KarpuzListesi[i]->x(),
                                          KarpuzListesi[i]->y()+6,
                                          KarpuzListesi[i]->width(),
                                          KarpuzListesi[i]->height());
            if(KarpuzListesi[i]->y()>750)
            {
                if(!geciciListe.contains(KarpuzListesi[i]))
                {
                    geciciListe.append(KarpuzListesi[i]);
                    ui->label_kacan_karpuz->setText("<font color='red' size='7'>"+QString::number(geciciListe.size())+"</font>");
                }
            }
        }
    }
}

void Dialog::karpuzsil()
{
    if(!KesiliKarpuzlar.isEmpty())
    {
        delete KesiliKarpuzlar[0];
        KesiliKarpuzlar.removeFirst();
    }

}

void Dialog::dinamitolustur()
{
    QPushButton *dinamit=new QPushButton(this);
    QIcon *icon = new QIcon();
    icon->addPixmap(QPixmap(":/new/prefix1/images/dinamitt.png"),QIcon::Normal,QIcon::Off);
    dinamit->setIcon(*icon);
    dinamit->setIconSize(QSize(70,70));
    dinamit->setStyleSheet("background-color: rgba(255, 255, 255, 0); color: black;");

    dinamit->setGeometry(rand()%1300,rand()%650+100,70,70);
    dinamit->show();
    DinamitListesi.push_back(dinamit);

    connect(dinamit, &QPushButton::clicked, [dinamit, this]() {
        DinamitListesi.removeOne(dinamit);
        QIcon *icon2 = new QIcon();
        icon2->addPixmap(QPixmap(":/new/prefix1/images/patlamma.png"), QIcon::Normal, QIcon::Off);
        dinamit->setIcon(*icon2);
        dinamit->setIconSize(QSize(100,100));
        PatlayanDinamitler.push_back(dinamit);
        dinamit->setStyleSheet("background-color: rgba(255, 255, 255, 0); color: black;");
        ceza=ceza+2;
    });


}

void Dialog::dinamithareketettir()
{
    for (int i=0; i<DinamitListesi.size();++i)
    {

        DinamitListesi[i]->setGeometry(DinamitListesi[i]->x(),
                                       DinamitListesi[i]->y()+10,
                                       DinamitListesi[i]->width(),
                                       DinamitListesi[i]->height());


    }

}

void Dialog::patlamakaldir()
{
    if(!PatlayanDinamitler.isEmpty())
    {
        delete PatlayanDinamitler[0];
        PatlayanDinamitler.removeFirst();
    }

}
