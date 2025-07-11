
#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QTimer>
#include <QPushButton>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QList>
#include <QStringList>

QT_BEGIN_NAMESPACE
namespace Ui {
class Dialog;
}
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();
    QList<QPushButton*> KesiliKarpuzlar;
    QList<QPushButton*> PatlayanDinamitler;
    QList<QPushButton*> geciciListe;
    void dosyayayaz();


private slots:
    void sureAzalt();
    void karpuzolustur();
    void hareketettir();
    void karpuzsil();
    void dinamitolustur();
    void dinamithareketettir();
    void patlamakaldir();

private:
    Ui::Dialog *ui;
    QList<QPushButton*> KarpuzListesi;
    QList<QPushButton*> DinamitListesi;

};
#endif // DIALOG_H
