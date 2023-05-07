#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <map>
#include <exception>
#include <QRegExp>
#include <QStandardItemModel>

#define ACCORDING_TO_BECHEAKED_LAB_TO_CHANGE(L,V) \
    if(ui->radioButtonBin->isChecked()){\
        ui->L->setText(QString::number(V, 2));\
    }else if(ui->radioButtonDec->isChecked()){\
        ui->L->setText(QString::number(V, 10));\
    }else if(ui->radioButtonHex->isChecked()){\
        ui->L->setText(QString::number(V, 16));\
    }else if(ui->radioButtonOct->isChecked()){\
        ui->L->setText(QString::number(V, 8));\
    }

bool isContinuousCalculation = false;       //不允许连续点击计算
bool isContinuousOp = false;		//不允许连续点击加减等操作

char hexs[] = {'A', 'B', 'C', 'D', 'E', 'F'};
int decs[] = {0,1,2,3,4,5,6,7,8,9};
int octs[] = {0,1,2,3,4,5,6,7};

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget), result(0), beOperated(0),operate(0), Operator(""), flag(FLAG::BEOP), binArray(64, '0')
{
    ui->setupUi(this);
    this->setWindowTitle("Calculator");
    ui->radioButtonDec->setChecked(true);

    QFont font("Arial", 26, QFont::Bold);
    ui->lineEditResult->setFont(font);

    ui->lineEditResult->setAlignment(Qt::AlignRight);
//    ui->lineEditResult->setEnabled(false);
    ui->pushButtonNonDot->setDisabled(true);

    ui->pushButtonNoA->setEnabled(false);
    ui->pushButtonNoB->setEnabled(false);
    ui->pushButtonNoC->setEnabled(false);
    ui->pushButtonNoD->setEnabled(false);
    ui->pushButtonNoE->setEnabled(false);
    ui->pushButtonNoF->setEnabled(false);

    ui->labelBeOp->setAlignment(Qt::AlignRight);
    ui->labelShowOp->setVisible(false);
    ui->labelBeOp->setVisible(false);
    ui->labelOp->setVisible(false);

    //当十六进制启用时，启用全部按钮
    for(int i = 0; i < sizeof(hexs)/sizeof(char); ++i){
        QPushButton * btHex = this->findChild<QPushButton*>("pushButtonNo" + QString(hexs[i]));
        QObject::connect(ui->radioButtonHex, &QPushButton::toggled, btHex, &QPushButton::setEnabled);
    }
    for(int i = 0; i < sizeof(decs)/sizeof(int); ++i){
        QPushButton * btNum = this->findChild<QPushButton*>("pushButtonNo" + QString::number(decs[i]));
        QObject::connect(ui->radioButtonHex, &QPushButton::toggled, btNum, &QPushButton::setEnabled);
    }

    //当十进制启用时，启用数字按钮
    for(int i = 0; i < sizeof(decs)/sizeof(int); ++i){
        QPushButton * btNum = this->findChild<QPushButton*>("pushButtonNo" + QString::number(decs[i]));
        QObject::connect(ui->radioButtonDec, &QPushButton::toggled, btNum, &QPushButton::setEnabled);
    }

    //当八进制启用时，启用0～7按钮
    for(int i = 0; i < sizeof(octs)/sizeof(int); ++i){
        QPushButton * btOct = this->findChild<QPushButton*>("pushButtonNo" + QString::number(octs[i]));
        QObject::connect(ui->radioButtonOct, &QPushButton::toggled, btOct, &QPushButton::setEnabled);
    }

    //当二进制启用时，只启用0和1按钮
    connect(ui->radioButtonBin, &QRadioButton::toggled, ui->pushButtonNo0, &QPushButton::setEnabled);
    connect(ui->radioButtonBin, &QRadioButton::toggled, ui->pushButtonNo1, &QPushButton::setEnabled);

    //向lineEditResult追加数字
    for(int i = 0; i < sizeof(hexs)/sizeof(char); ++i){
        QPushButton * btHex = this->findChild<QPushButton*>("pushButtonNo" + QString(hexs[i]));
        QObject::connect(btHex, &QPushButton::clicked, this, &Widget::numKeyClick);
    }
    for(int i = 0; i < sizeof(decs)/sizeof(int); ++i){
        QPushButton * btNum = this->findChild<QPushButton*>("pushButtonNo" + QString::number(decs[i]));
        QObject::connect(btNum, &QPushButton::clicked, this, &Widget::numKeyClick);
    }

    //各个操作都会执行大致相同的事情，直接连接到同一个槽函数，增加可扩展性
    connect(ui->pushButtonOpPlus, &QPushButton::clicked, this, &Widget::op);
    connect(ui->pushButtonOpMin, &QPushButton::clicked, this, &Widget::op);
    connect(ui->pushButtonOpMult, &QPushButton::clicked, this, &Widget::op);
    connect(ui->pushButtonOpDiv, &QPushButton::clicked, this, &Widget::op);
    connect(ui->pushButtonOpLsh, &QPushButton::clicked, this, &Widget::op);
    connect(ui->pushButtonOpRsh, &QPushButton::clicked, this, &Widget::op);
    connect(ui->pushButtonOpNot, &QPushButton::clicked, this, &Widget::pushButtonOpNot);		//特殊操作，直接修改lineEditResult的文本
    connect(ui->pushButtonOpXor, &QPushButton::clicked, this, &Widget::op);
    connect(ui->pushButtonOpAnd, &QPushButton::clicked, this, &Widget::op);
    connect(ui->pushButtonOpOr, &QPushButton::clicked, this, &Widget::op);
    connect(ui->pushButtonOpMod, &QPushButton::clicked, this, &Widget::op);

    //连接所有bin按钮到binChanged
    for(int i = 0; i < 64; ++i){
        QPushButton *btObject = nullptr;
        if(i < 10)
            btObject = this->findChild<QPushButton*>("pushButtonBin0"+ QString::number(i));
        else
            btObject = this->findChild<QPushButton*>("pushButtonBin"+ QString::number(i));
        if(btObject){
            connect(btObject, &QPushButton::clicked, this, &Widget::binChanged);
        }
    }

    //切换到二进制键盘时将阶段改为操作阶段
    connect(ui->tabWidget, &QTabWidget::currentChanged, [this](int index){
        if(index == 0){
        /*并且*/if (ui->lineEditResult->text() != "0" || !ui->lineEditResult->text().isEmpty())
                this->flag = FLAG::BEOP;
        }else if(index == 1){
            ui->labelBeOp->setVisible(false);
            ui->labelShowOp->setVisible(false);
            ui->labelOp->setVisible(false);
            this->flag = FLAG::OP;
            //将每个1的位置的按钮都check,binArray也会自动变为正确数值
            auto strBinArr = ui->labelShowBin->text();
            auto strBinArrSize = strBinArr.size();
            for(int i = 0; i < strBinArrSize; ++i){
                QPushButton *btObject = nullptr;
                if(i < 10)
                    btObject = this->findChild<QPushButton*>("pushButtonBin0"+ QString::number(i));
                else
                    btObject = this->findChild<QPushButton*>("pushButtonBin"+ QString::number(i));
                if(btObject){
                    if(strBinArr[strBinArrSize - i - 1] == "1"){
                        emit btObject->clicked(true);
                        btObject->setChecked(true);
                    }else{
                        emit btObject->clicked(false);
                        btObject->setChecked(false);
                    }
                }
            }
            //将lineEditResult最大1之前的都归0,原因：当之前点过比目前最长的1还要长时，那些1无法归0,因为上面的循环判断不到
            for(int i = strBinArrSize; i < 64; ++i){
                QPushButton *btObject = nullptr;
                if(i < 10)
                    btObject = this->findChild<QPushButton*>("pushButtonBin0"+ QString::number(i));
                else
                    btObject = this->findChild<QPushButton*>("pushButtonBin"+ QString::number(i));
                if(btObject){
                    emit btObject->clicked(false);      //直接触发 false click
                    btObject->setChecked(false);
                }
            }
            //倒着替换
//            auto strBinArrEndIt = strBinArr.end();
//            auto binArrayEndIt = binArray.end();
//            while(strBinArr.begin() != strBinArrEndIt){
//                --strBinArrEndIt;
//                --binArrayEndIt;
//                *binArrayEndIt = *strBinArrEndIt;
//            }
        }
    });
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
    ui->lineEditResult->insert(QString(senderName.back().toUpper()));

    isContinuousCalculation = false;
    isContinuousOp = false;
}

//CHECK TODO UPDATE 改变就更新操作数会增加cpu计算量
//应该在点击操作后更新被操作数，在点击计算后更新被操作数和结果
void Widget::on_lineEditResult_textChanged(const QString &txt)
{
    if(txt == "0" && flag == FLAG::BEOP) return;
    //如果result中出现了A-F,禁用其他进制按钮
    //如果result中出现了A-F,8，9,禁用8进制和2进制按钮
    //如果result中出现了A-F,2-9,禁用2进制按钮
    if(txt.contains(QRegExp("[A-F]"))){
        ui->radioButtonBin->setEnabled(false);
        ui->radioButtonDec->setEnabled(false);
        ui->radioButtonOct->setEnabled(false);
    }else{
        ui->radioButtonBin->setEnabled(true);
        ui->radioButtonDec->setEnabled(true);
        ui->radioButtonOct->setEnabled(true);
    }
    if(txt.contains(QRegExp("[A-F89]"))){
        ui->radioButtonBin->setEnabled(false);
        ui->radioButtonOct->setEnabled(false);
    }else{
        ui->radioButtonBin->setEnabled(true);
        ui->radioButtonOct->setEnabled(true);
    }
    if(txt.contains(QRegExp("[A-F2-9]"))){
        ui->radioButtonBin->setEnabled(false);
    }else{
        ui->radioButtonBin->setEnabled(true);
    }

    //更新各个标签
    bool ok = false;
    long long numShow = txt.toLongLong();
    if(ui->radioButtonBin->isChecked()){
            numShow = txt.toLongLong(&ok, 2);
    }else if(ui->radioButtonDec->isChecked()){
            numShow = txt.toLongLong(&ok, 10);
    }else if(ui->radioButtonHex->isChecked()){
            numShow = txt.toLongLong(&ok, 16);
    }else if(ui->radioButtonOct->isChecked()){
            numShow = txt.toLongLong(&ok, 8);
    }
    qDebug() << ui->labelShowBin->text();

    ui->labelShowBin->setText(QString::number(numShow, 2));
    ui->labelShowHex->setText(QString::number(numShow, 16).toUpper());
    ui->labelShowDec->setText(QString::number(numShow));
    ui->labelShowOct->setText(QString::number(numShow, 8));

//    for(int i = strBinArrSize + 1; i < 64; ++i){
        //将每一位不相关的都置为零
//        this->binArray.replace(i, 1, '0');
//    }

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
    auto senderName = sender()->objectName();
    if(isContinuousOp && senderName == Operator) return;
    //如果flag为op,调用一次计算操作
    if(flag == FLAG::OP){
        on_pushButtonOpCalculate_clicked();
        //op阶段，标签应为不可见
        ui->labelOp->setVisible(false);
    }
    //将lineEditResult里的值保存到beOperated       TODO 计算操作实现之后删除此操作 FOR重复 *?
    updateBeOpNum(ui->lineEditResult->text());
//    bool ok = false;
//    if(ui->radioButtonBin->isChecked()){
//            this->beOperated = ui->lineEditResult->text().toLongLong(&ok, 2);
//    }else if(ui->radioButtonDec->isChecked()){
//            this->beOperated = ui->lineEditResult->text().toLongLong(&ok, 10);
//    }else if(ui->radioButtonHex->isChecked()){
//            this->beOperated = ui->lineEditResult->text().toLongLong(&ok, 16);
//    }else if(ui->radioButtonOct->isChecked()){
//            this->beOperated = ui->lineEditResult->text().toLongLong(&ok, 8);
//    }
    //将lineEditResult清空
//    if(!isContinuousOp){
        ui->lineEditResult->blockSignals(true);
        ui->lineEditResult->setText(QString("0"));
        ui->labelShowBin->setText(QString("0"));
        ui->labelShowDec->setText(QString("0"));
        ui->labelShowHex->setText(QString("0"));
        ui->labelShowOct->setText(QString("0"));
        ui->lineEditResult->blockSignals(false);
//    }
    //增加一个用于显示被操作数的组件，并设置文本：beOperated
    ui->labelBeOp->show();
    ACCORDING_TO_BECHEAKED_LAB_TO_CHANGE(labelBeOp, beOperated);

    //将operator设置为与sender()->name()相同的符号   TODO 在点击计算后再更新
    this->Operator = senderName;
    ui->labelShowOp->show();
    if(nameToSymbol.find(senderName) != nameToSymbol.end()){
        ui->labelShowOp->setText(nameToSymbol[senderName]);
    }else{
        qDebug() << "未找到与信号发送者相同的符号";
    }

    //修改标志位
    isContinuousOp = true;
    this->flag = FLAG::OP;
    isContinuousCalculation = false;        //不是连续计算
}

//处理逻辑
//将计算结果同时保存到result和beOperated
void Widget::pushButtonOpPlus()
{
    this->result = this->beOperated + this->operate;
}

void Widget::pushButtonOpMin()
{
    this->result = this->beOperated - this->operate;
}

void Widget::pushButtonOpMult()
{
    this->result = this->beOperated * this->operate;
}

void Widget::pushButtonOpDiv()
{
    this->result = this->beOperated / this->operate;
}

void Widget::pushButtonOpLsh()
{
    this->result = this->beOperated << this->operate;
}

void Widget::pushButtonOpRsh()
{
    this->result = this->beOperated >> this->operate;
}

void Widget::pushButtonOpXor()
{
    this->result = this->beOperated ^ this->operate;
}

void Widget::pushButtonOpAnd()
{
    this->result = this->beOperated & this->operate;
}

void Widget::pushButtonOpOr()
{
    this->result = this->beOperated | this->operate;
}

void Widget::pushButtonOpMod()
{
    this->result = this->beOperated % this->operate;
}

void Widget::pushButtonOpNot()
{
    QRegExp rx("[01]");  // 定义一个正则表达式，用于匹配二进制字符
    int pos = 0;
    while ((pos = rx.indexIn(binArray, pos)) != -1) {
        QString s = rx.cap(0);
        QString replacement = s == "0" ? "1" : "0";
        binArray.replace(pos, 1, replacement);
        pos += rx.matchedLength();
    }
    //将binArray转换为十进制
    bool ok = false;
    long long dec = binArray.toLongLong(&ok, 2);
    //再将十进制转换为选中的进制
    if(ok){
        ACCORDING_TO_BECHEAKED_LAB_TO_CHANGE(lineEditResult, dec);
    }
}

//如果是主动点击，将结果显示在lineEditResult，并修改标志位为BEOP —— 主动调用时，调用者的名字为calculate
//如果是被动调用，只 计算并保存
void Widget::on_pushButtonOpCalculate_clicked()
{
    if(isContinuousCalculation) return;
    if(Operator.isEmpty()) return;
    //调用方法之前更新一次操作数
    updateOpNum(ui->lineEditResult->text());
    //显示操作数历史
    ui->labelOp->show();
    ACCORDING_TO_BECHEAKED_LAB_TO_CHANGE(labelOp, this->operate);
    //调用计算操作
    QMetaObject::invokeMethod(this, Operator.toUtf8().data());
    auto senderName = sender()->objectName();
    qDebug() << senderName;
    //    auto symbol = nameToSymbol[senderName];
    //调用与信号发送方名字相同的函数
    if(senderName == "pushButtonOpCalculate"){
        //如果是主动调用
        ACCORDING_TO_BECHEAKED_LAB_TO_CHANGE(lineEditResult, result);
//        flag = FLAG::BEOP;    //不应在此修改标志位，应该点击C/CE之后修改
    }
    //每次计算完成，都要保存记录到ListViewHistory
    static int record = 0;
    static QStandardItemModel *model = new QStandardItemModel(this);
    QStandardItem *item = new QStandardItem(QString::number(record++)
                                            + ".    "
                                            + ui->labelBeOp->text()
                                            + ui->labelShowOp->text()
                                            + ui->labelOp->text()
                                            + "="
                                            + ui->lineEditResult->text());
    model->insertRow(0, item);
    ui->listViewHistory->setModel(model);

    //将计算结果同时保存到result和beOperated-->由各个操作完成
    this->beOperated = result;

    isContinuousCalculation = true;     //不允许连续点击计算
    isContinuousOp = false;
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


void Widget::on_pushButtonOpCE_clicked()
{
    this->beOperated = 0;
    this->Operator = "";
    this->operate = 0;
    this->result = 0;
    this->flag = FLAG::BEOP;
    ui->lineEditResult->setText(QString("0"));
    ui->labelShowBin->setText(QString("0"));
    ui->labelShowDec->setText(QString("0"));
    ui->labelShowHex->setText(QString("0"));
    ui->labelShowOct->setText(QString("0"));
    ui->labelShowOp->setText("");
    ui->labelShowOp->setVisible(false);
    ui->labelBeOp->setText("");
    ui->labelBeOp->setVisible(false);
    ui->labelOp->setText("");
    ui->labelOp->setVisible(false);
}


void Widget::on_pushButtonOpClean_clicked()
{
//    this->Operator = "";
    ui->lineEditResult->setText(QString("0"));
    ui->labelShowBin->setText(QString("0"));
    ui->labelShowDec->setText(QString("0"));
    ui->labelShowHex->setText(QString("0"));
    ui->labelShowOct->setText(QString("0"));
    if(flag == FLAG::OP){
        this->operate = 0;
    }else if(flag == FLAG::BEOP){
        this->beOperated = 0;
    }
}


void Widget::on_pushButtonOpBs_clicked()
{
    auto value = ui->lineEditResult->text();
    value.remove(value.size()-1, 1);
    if(value.isEmpty()) value = "0";
    ui->lineEditResult->setText(value);
}

//设置0/1
//更新binArray
//更新lineEditResult
void Widget::binChanged(bool beChecked)
{
    QPushButton* btObj = (QPushButton*)sender();
    btObj->setText(beChecked ? "1" : "0");

    auto btIndex = sender()->objectName().right(2);
    this->binArray.replace(64 - btIndex.toInt() -1, 1, beChecked? '1': '0');       //{64减}相当于倒着修改binArray

    //将binArray转换为十进制
    bool ok = false;
    long long dec = binArray.toLongLong(&ok, 2);
    //再将十进制转换为选中的进制
    if(ok){
        ACCORDING_TO_BECHEAKED_LAB_TO_CHANGE(lineEditResult, dec);
    }
}

void Widget::updateOpNum(QString opNum)
{
    //更新各个进制label的内容
    bool ok = false;
    if(ui->radioButtonBin->isChecked()){
            this->operate = opNum.toLongLong(&ok, 2);
    }else if(ui->radioButtonDec->isChecked()){
            this->operate = opNum.toLongLong(&ok, 10);
    }else if(ui->radioButtonHex->isChecked()){
            this->operate = opNum.toLongLong(&ok, 16);
    }else if(ui->radioButtonOct->isChecked()){
            this->operate = opNum.toLongLong(&ok, 8);
    }
    qDebug() << ok << "\t" << this->beOperated;
    if(ok){
            binNum = QString::number(this->beOperated, 2);
            ui->labelShowBin->setText(binNum);
            ui->labelShowHex->setText(QString::number(this->beOperated, 16).toUpper());
            ui->labelShowDec->setText(QString::number(this->beOperated));
            ui->labelShowOct->setText(QString::number(this->beOperated, 8));
    }/*else{
        throw std::runtime_error("进制转换异常：转换前>>" + std::to_string(result) + "\t转换结果：" + std::to_string(ok));
    }*/
}

void Widget::updateBeOpNum(QString beOpNum)
{    //更新各个进制label的内容
    bool ok = false;
    if(ui->radioButtonBin->isChecked()){
            this->beOperated = beOpNum.toLongLong(&ok, 2);
    }else if(ui->radioButtonDec->isChecked()){
            this->beOperated = beOpNum.toLongLong(&ok, 10);
    }else if(ui->radioButtonHex->isChecked()){
            this->beOperated = beOpNum.toLongLong(&ok, 16);
    }else if(ui->radioButtonOct->isChecked()){
            this->beOperated = beOpNum.toLongLong(&ok, 8);
    }
    qDebug() << ok << "\t" << this->beOperated;
    if(ok){
            binNum = QString::number(this->operate, 2);
            ui->labelShowBin->setText(binNum);
            ui->labelShowHex->setText(QString::number(this->operate, 16).toUpper());
            ui->labelShowDec->setText(QString::number(this->operate));
            ui->labelShowOct->setText(QString::number(this->operate, 8));
    }/*else{
        throw std::runtime_error("进制转换异常：转换前>>" + std::to_string(result) + "\t转换结果：" + std::to_string(ok));
    }*/

}

void Widget::updateBinStr()
{
    //还需要修改binArray
    auto strBinArrSize = binNum.size();
    for(int i = 0; i < strBinArrSize; ++i)
        this->binArray.replace(64 - i -1, 1, binNum[strBinArrSize-i-1]);       //{64减}相当于倒着修改binArray
}

















