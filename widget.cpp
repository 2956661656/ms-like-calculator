#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <map>
#include <exception>

void hexConnect(Ui::Widget *ui){
    QObject::connect(ui->radioButtonHex, &QPushButton::toggled, ui->pushButtonNoA, &QPushButton::setEnabled);
    QObject::connect(ui->radioButtonHex, &QPushButton::toggled, ui->pushButtonNoB, &QPushButton::setEnabled);
    QObject::connect(ui->radioButtonHex, &QPushButton::toggled, ui->pushButtonNoC, &QPushButton::setEnabled);
    QObject::connect(ui->radioButtonHex, &QPushButton::toggled, ui->pushButtonNoD, &QPushButton::setEnabled);
    QObject::connect(ui->radioButtonHex, &QPushButton::toggled, ui->pushButtonNoE, &QPushButton::setEnabled);
    QObject::connect(ui->radioButtonHex, &QPushButton::toggled, ui->pushButtonNoF, &QPushButton::setEnabled);
    QObject::connect(ui->radioButtonHex, &QPushButton::toggled, ui->pushButtonNo1, &QPushButton::setEnabled);
    QObject::connect(ui->radioButtonHex, &QPushButton::toggled, ui->pushButtonNo2, &QPushButton::setEnabled);
    QObject::connect(ui->radioButtonHex, &QPushButton::toggled, ui->pushButtonNo3, &QPushButton::setEnabled);
    QObject::connect(ui->radioButtonHex, &QPushButton::toggled, ui->pushButtonNo4, &QPushButton::setEnabled);
    QObject::connect(ui->radioButtonHex, &QPushButton::toggled, ui->pushButtonNo5, &QPushButton::setEnabled);
    QObject::connect(ui->radioButtonHex, &QPushButton::toggled, ui->pushButtonNo6, &QPushButton::setEnabled);
    QObject::connect(ui->radioButtonHex, &QPushButton::toggled, ui->pushButtonNo7, &QPushButton::setEnabled);
    QObject::connect(ui->radioButtonHex, &QPushButton::toggled, ui->pushButtonNo8, &QPushButton::setEnabled);
    QObject::connect(ui->radioButtonHex, &QPushButton::toggled, ui->pushButtonNo9, &QPushButton::setEnabled);
    QObject::connect(ui->radioButtonHex, &QPushButton::toggled, ui->pushButtonNo0, &QPushButton::setEnabled);
}
void decConnect(Ui::Widget *ui){
    QObject::connect(ui->radioButtonDec, &QPushButton::toggled, ui->pushButtonNo1, &QPushButton::setEnabled);
    QObject::connect(ui->radioButtonDec, &QPushButton::toggled, ui->pushButtonNo2, &QPushButton::setEnabled);
    QObject::connect(ui->radioButtonDec, &QPushButton::toggled, ui->pushButtonNo3, &QPushButton::setEnabled);
    QObject::connect(ui->radioButtonDec, &QPushButton::toggled, ui->pushButtonNo4, &QPushButton::setEnabled);
    QObject::connect(ui->radioButtonDec, &QPushButton::toggled, ui->pushButtonNo5, &QPushButton::setEnabled);
    QObject::connect(ui->radioButtonDec, &QPushButton::toggled, ui->pushButtonNo6, &QPushButton::setEnabled);
    QObject::connect(ui->radioButtonDec, &QPushButton::toggled, ui->pushButtonNo7, &QPushButton::setEnabled);
    QObject::connect(ui->radioButtonDec, &QPushButton::toggled, ui->pushButtonNo8, &QPushButton::setEnabled);
    QObject::connect(ui->radioButtonDec, &QPushButton::toggled, ui->pushButtonNo9, &QPushButton::setEnabled);
    QObject::connect(ui->radioButtonDec, &QPushButton::toggled, ui->pushButtonNo0, &QPushButton::setEnabled);
}

void OctConnect(Ui::Widget *ui){
    QObject::connect(ui->radioButtonOct, &QPushButton::toggled, ui->pushButtonNo1, &QPushButton::setEnabled);
    QObject::connect(ui->radioButtonOct, &QPushButton::toggled, ui->pushButtonNo2, &QPushButton::setEnabled);
    QObject::connect(ui->radioButtonOct, &QPushButton::toggled, ui->pushButtonNo3, &QPushButton::setEnabled);
    QObject::connect(ui->radioButtonOct, &QPushButton::toggled, ui->pushButtonNo4, &QPushButton::setEnabled);
    QObject::connect(ui->radioButtonOct, &QPushButton::toggled, ui->pushButtonNo5, &QPushButton::setEnabled);
    QObject::connect(ui->radioButtonOct, &QPushButton::toggled, ui->pushButtonNo6, &QPushButton::setEnabled);
    QObject::connect(ui->radioButtonOct, &QPushButton::toggled, ui->pushButtonNo7, &QPushButton::setEnabled);
    QObject::connect(ui->radioButtonOct, &QPushButton::toggled, ui->pushButtonNo0, &QPushButton::setEnabled);
}

void numKeyConnect(Ui::Widget *ui){
}

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget), flag(FLAG::BEOP)
{
    ui->setupUi(this);
    this->setWindowTitle("Calculator");
    ui->radioButtonDec->setChecked(true);

    ui->lineEditResult->setAlignment(Qt::AlignRight);
//    ui->lineEditResult->setEnabled(false);
    ui->pushButtonNonDot->setDisabled(true);
//    ui->horizontalLayoutSwitch		//跨两列

    ui->pushButtonNoA->setEnabled(false);
    ui->pushButtonNoB->setEnabled(false);
    ui->pushButtonNoC->setEnabled(false);
    ui->pushButtonNoD->setEnabled(false);
    ui->pushButtonNoE->setEnabled(false);
    ui->pushButtonNoF->setEnabled(false);

    //当十六进制启用时，启用全部按钮
    hexConnect(ui);

    //当十进制启用时，启用数字按钮
    decConnect(ui);

    //当八进制启用时，启用0～7按钮
    OctConnect(ui);

    //当二进制启用时，只启用0和1按钮
    connect(ui->radioButtonBin, &QRadioButton::toggled, ui->pushButtonNo0, &QPushButton::setEnabled);
    connect(ui->radioButtonBin, &QRadioButton::toggled, ui->pushButtonNo1, &QPushButton::setEnabled);

    QObject::connect(ui->pushButtonNo1, &QPushButton::clicked, this, &Widget::numKeyClick);
    QObject::connect(ui->pushButtonNo2, &QPushButton::clicked, this, &Widget::numKeyClick);
    QObject::connect(ui->pushButtonNo3, &QPushButton::clicked, this, &Widget::numKeyClick);
    QObject::connect(ui->pushButtonNo4, &QPushButton::clicked, this, &Widget::numKeyClick);
    QObject::connect(ui->pushButtonNo5, &QPushButton::clicked, this, &Widget::numKeyClick);
    QObject::connect(ui->pushButtonNo6, &QPushButton::clicked, this, &Widget::numKeyClick);
    QObject::connect(ui->pushButtonNo7, &QPushButton::clicked, this, &Widget::numKeyClick);
    QObject::connect(ui->pushButtonNo8, &QPushButton::clicked, this, &Widget::numKeyClick);
    QObject::connect(ui->pushButtonNo9, &QPushButton::clicked, this, &Widget::numKeyClick);
    QObject::connect(ui->pushButtonNo0, &QPushButton::clicked, this, &Widget::numKeyClick);
    QObject::connect(ui->pushButtonNoA, &QPushButton::clicked, this, &Widget::numKeyClick);
    QObject::connect(ui->pushButtonNoB, &QPushButton::clicked, this, &Widget::numKeyClick);
    QObject::connect(ui->pushButtonNoC, &QPushButton::clicked, this, &Widget::numKeyClick);
    QObject::connect(ui->pushButtonNoD, &QPushButton::clicked, this, &Widget::numKeyClick);
    QObject::connect(ui->pushButtonNoE, &QPushButton::clicked, this, &Widget::numKeyClick);
    QObject::connect(ui->pushButtonNoF, &QPushButton::clicked, this, &Widget::numKeyClick);
//    numKeyConnect(ui);

    connect(ui->pushButtonOpPlus, &QPushButton::clicked, this, &Widget::op);
    connect(ui->pushButtonOpMin, &QPushButton::clicked, this, &Widget::op);
    connect(ui->pushButtonOpMult, &QPushButton::clicked, this, &Widget::op);
    connect(ui->pushButtonOpDiv, &QPushButton::clicked, this, &Widget::op);
    connect(ui->pushButtonOpLsh, &QPushButton::clicked, this, &Widget::op);
    connect(ui->pushButtonOpRsh, &QPushButton::clicked, this, &Widget::op);
    connect(ui->pushButtonOpNot, &QPushButton::clicked, this, &Widget::op);
    connect(ui->pushButtonOpXor, &QPushButton::clicked, this, &Widget::op);
    connect(ui->pushButtonOpAnd, &QPushButton::clicked, this, &Widget::op);
    connect(ui->pushButtonOpOr, &QPushButton::clicked, this, &Widget::op);
    connect(ui->pushButtonOpMod, &QPushButton::clicked, this, &Widget::op);
}

Widget::~Widget()
{
    delete ui;
}

//每个数字键执行前都要检查一遍Result是否为0
//每个数字键执行时都需要做一件相同的事：向result追加与函数名对应的字
void Widget::numKeyClick(){
    if("0" == ui->lineEditResult->text()){
        ui->lineEditResult->clear();
    }
    auto senderName = sender()->objectName();
    ui->lineEditResult->insert(QString(senderName.back()));

}

void Widget::on_lineEditResult_textChanged(const QString &arg1)
{
    if(arg1 == "0") return;
    //如果result中出现了A-F,禁用其他进制按钮
    //如果result中出现了A-F,8，9,禁用8进制和2进制按钮
    //如果result中出现了A-F,2-9,禁用2进制按钮
    if(arg1.contains(QRegExp("[A-F]"))){
        ui->radioButtonBin->setEnabled(false);
        ui->radioButtonDec->setEnabled(false);
        ui->radioButtonOct->setEnabled(false);
    }else{
        ui->radioButtonBin->setEnabled(true);
        ui->radioButtonDec->setEnabled(true);
        ui->radioButtonOct->setEnabled(true);
    }
    if(arg1.contains(QRegExp("[A-F89]"))){
        ui->radioButtonBin->setEnabled(false);
        ui->radioButtonOct->setEnabled(false);
    }else{
        ui->radioButtonBin->setEnabled(true);
        ui->radioButtonOct->setEnabled(true);
    }
    if(arg1.contains(QRegExp("[A-F2-9]"))){
        ui->radioButtonBin->setEnabled(false);
    }else{
        ui->radioButtonBin->setEnabled(true);
    }
    //更新各个进制label的内容
    bool ok = false;
    if(ui->radioButtonBin->isChecked()){
        if(flag == FLAG::BEOP)
            this->beOperated = arg1.toLongLong(&ok, 2);
        if(flag == FLAG::OP)
            this->operate = arg1.toLongLong(&ok, 2);
    }else if(ui->radioButtonDec->isChecked()){
        if(flag == FLAG::BEOP)
            this->beOperated = arg1.toLongLong(&ok, 10);
        if(flag == FLAG::OP)
            this->operate = arg1.toLongLong(&ok, 10);
    }else if(ui->radioButtonHex->isChecked()){
        if(flag == FLAG::BEOP)
            this->beOperated = arg1.toLongLong(&ok, 16);
        if(flag == FLAG::OP)
            this->operate = arg1.toLongLong(&ok, 16);
    }else if(ui->radioButtonOct->isChecked()){
        if(flag == FLAG::BEOP)
            this->beOperated = arg1.toLongLong(&ok, 8);
        if(flag == FLAG::OP)
            this->operate = arg1.toLongLong(&ok, 8);
    }
    qDebug() << ok << "\t" << this->beOperated;
    if(ok){
        if(flag == FLAG::BEOP){
            ui->labelShowBin->setText(QString::number(this->beOperated, 2));
            ui->labelShowHex->setText(QString::number(this->beOperated, 16).toUpper());
            ui->labelShowDec->setText(QString::number(this->beOperated));
            ui->labelShowOct->setText(QString::number(this->beOperated, 8));
        }
        if(flag == FLAG::OP){
            ui->labelShowBin->setText(QString::number(this->operate, 2));
            ui->labelShowHex->setText(QString::number(this->operate, 16).toUpper());
            ui->labelShowDec->setText(QString::number(this->operate));
            ui->labelShowOct->setText(QString::number(this->operate, 8));
        }
    }/*else{
        throw std::runtime_error("进制转换异常：转换前>>" + std::to_string(result) + "\t转换结果：" + std::to_string(ok));
    }*/
}

std::map<QString, QString> nameToSymbol{
    {"pushButtonOpPlus", "+"}
    ,{"pushButtonOpMin", "-"}
    ,{"pushButtonOpMult", "*"}
    ,{"pushButtonOpDiv", "/"}
    ,{"pushButtonOpLsh", "<<"}
    ,{"pushButtonOpRsh", ">>"}
    ,{"pushButtonOpNot", "!"}
    ,{"pushButtonOpXor", "^"}
    ,{"pushButtonOpAnd", "&"}
    ,{"pushButtonOpOr", "|"}
    ,{"pushButtonOpMod", "%"}
};

//每个操作Op点击后都需要将result清空并在result前一行增加一个label用于记录被操作数
void Widget::op(){
    //如果flag为op,调用一次计算操作
    if(flag == FLAG::OP)
        on_pushButtonOpCalculate_clicked();
    //将lineEditResult里的值保存到beOperated       TODO 计算操作实现之后删除此操作 FOR重复
    bool ok = false;
    if(ui->radioButtonBin->isChecked()){
            this->beOperated = ui->lineEditResult->text().toLongLong(&ok, 2);
    }else if(ui->radioButtonDec->isChecked()){
            this->beOperated = ui->lineEditResult->text().toLongLong(&ok, 10);
    }else if(ui->radioButtonHex->isChecked()){
            this->beOperated = ui->lineEditResult->text().toLongLong(&ok, 16);
    }else if(ui->radioButtonOct->isChecked()){
            this->beOperated = ui->lineEditResult->text().toLongLong(&ok, 8);
    }
    //将lineEditResult清空
    ui->lineEditResult->setText(QString("0"));
    ui->labelShowBin->setText(QString("0"));
    ui->labelShowDec->setText(QString("0"));
    ui->labelShowHex->setText(QString("0"));
    ui->labelShowOct->setText(QString("0"));
    //增加一个用于显示被操作数的组件，并设置文本：beOperated

    //将operator设置为与sender()->name()相同的符号
    auto senderName = sender()->objectName();
    this->Operator = senderName;
//    if(nameToSymbol.find(senderName) != nameToSymbol.end()){
//        this->Operator = nameToSymbol[senderName];
//    }else{
//        qDebug() << "未找到与信号发送者相同的符号";
//    }

    //修改标志位
    this->flag = FLAG::OP;
}

//处理逻辑
//将计算结果同时保存到result和beOperated
void Widget::pushButtonOpPlus()
{
    this->result = this->beOperated + this->operate;
    this->beOperated = result;
}

void Widget::pushButtonOpMin()
{
    this->result = this->beOperated - this->operate;
    this->beOperated = result;
}

void Widget::pushButtonOpMult()
{
    this->result = this->beOperated * this->operate;
    this->beOperated = result;
}

void Widget::pushButtonOpDiv()
{
    this->result = this->beOperated / this->operate;
    this->beOperated = result;
}

void Widget::pushButtonOpLsh()
{

}

void Widget::pushButtonOpRsh()
{

}

void Widget::pushButtonOpNot()
{

}

void Widget::pushButtonOpXor()
{

}

void Widget::pushButtonOpAnd()
{

}

void Widget::pushButtonOpOr()
{

}

void Widget::pushButtonOpMod()
{

}

//如果是主动点击，将结果显示在lineEditResult，并修改标志位为BEOP —— 主动调用时，调用者的名字为calculate
//如果是被动调用，只 计算并保存
void Widget::on_pushButtonOpCalculate_clicked()
{
    auto senderName = sender()->objectName();
    qDebug() << senderName;
    //    auto symbol = nameToSymbol[senderName];
    //调用与信号发送方名字相同的函数
    QMetaObject::invokeMethod(this, Operator.toUtf8().data());
    if(senderName == "pushButtonOpCalculate"){
        //如果是主动调用
        if(ui->radioButtonBin->isChecked()){
            ui->lineEditResult->setText(QString::number(result, 2));
        }else if(ui->radioButtonDec->isChecked()){
            ui->lineEditResult->setText(QString::number(result, 10));
        }else if(ui->radioButtonHex->isChecked()){
            ui->lineEditResult->setText(QString::number(result, 16));
        }else if(ui->radioButtonOct->isChecked()){
            ui->lineEditResult->setText(QString::number(result, 8));
        }
//        flag = FLAG::BEOP;    //不应在此修改标志位，应该点击C/CE之后修改
    }

    //将计算结果同时保存到result和beOperated-->由各个操作完成
}
















void Widget::on_radioButtonHex_toggled(bool checked)
{
    ui->lineEditResult->setText(ui->labelShowHex->text());
}


void Widget::on_radioButtonDec_toggled(bool checked)
{
    ui->lineEditResult->setText(ui->labelShowDec->text());
}


void Widget::on_radioButtonOct_toggled(bool checked)
{
    ui->lineEditResult->setText(ui->labelShowOct->text());
}


void Widget::on_radioButtonBin_toggled(bool checked)
{
    ui->lineEditResult->setText(ui->labelShowBin->text());
}

