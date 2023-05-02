#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

enum FLAG{
    BEOP, OP
};

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private slots:

    void on_lineEditResult_textChanged(const QString &arg1);

    void numKeyClick();
    void op();

    void pushButtonOpPlus();
    void pushButtonOpMin();
    void pushButtonOpMult();
    void pushButtonOpDiv();
    void pushButtonOpLsh();
    void pushButtonOpRsh();
    void pushButtonOpNot();
    void pushButtonOpXor();
    void pushButtonOpAnd();
    void pushButtonOpOr();
    void pushButtonOpMod();

    void on_pushButtonOpCalculate_clicked();

    void on_radioButtonHex_toggled(bool checked);

    void on_radioButtonDec_toggled(bool checked);

    void on_radioButtonOct_toggled(bool checked);

    void on_radioButtonBin_toggled(bool checked);

    void on_pushButtonOpCE_clicked();

    void on_pushButtonOpClean_clicked();

    void on_pushButtonOpBs_clicked();

    void binChanged(bool);

private:
    Ui::Widget *ui;

    long long result;
    long long beOperated;
    long long operate;
    QString Operator;
    int flag;       //用于判断当前是在操作数阶段还是在被操作数阶段
    QString binArray;
};
#endif // WIDGET_H





















