#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QString>

namespace {
QString NormalizeNumber(const QString &text) {
    if (text.isEmpty()) {
        return "0";
    }
    if (text.startsWith('.')) {
        return NormalizeNumber("0" + text);
    }
    if (text.startsWith('-')) {
        return "-" + NormalizeNumber(text.mid(1));
    }
    if (text.startsWith('0') && !text.startsWith("0.")) {
        int i = 0;
        while (i < text.size() && text[i] == '0') {
            ++i;
        }
        if (i == text.size()) return "0";
        return NormalizeNumber(text.mid(i));
    }
    return text;
}
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    SetupUI();
    SetText("0");
    l_memory->setText("");
    l_formula->setText("");
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::SetupUI() {
    QWidget* central = new QWidget(this);
    setCentralWidget(central);

    QGridLayout* layout = new QGridLayout(central);

    l_formula = new QLabel("", this);
    l_formula->setAlignment(Qt::AlignRight);
    l_formula->setObjectName("l_formula");

    l_memory = new QLabel("", this);
    l_memory->setObjectName("l_memory");

    l_result = new QLabel("0", this);
    l_result->setAlignment(Qt::AlignRight);
    l_result->setObjectName("l_result");

    layout->addWidget(l_formula, 0, 0, 1, 4);
    layout->addWidget(l_memory, 1, 0);
    layout->addWidget(l_result, 1, 1, 1, 3);

    QPushButton* btn_mc = new QPushButton("MC", this);
    QPushButton* btn_mr = new QPushButton("MR", this);
    QPushButton* btn_ms = new QPushButton("MS", this);
    QPushButton* btn_pow = new QPushButton("xʸ", this);

    layout->addWidget(btn_mc, 2, 0);
    layout->addWidget(btn_mr, 2, 1);
    layout->addWidget(btn_ms, 2, 2);
    layout->addWidget(btn_pow, 2, 3);

    QPushButton* btn_c = new QPushButton("C", this);
    QPushButton* btn_sign = new QPushButton("±", this);
    QPushButton* btn_div = new QPushButton("÷", this);

    layout->addWidget(btn_c, 3, 0, 1, 2);
    layout->addWidget(btn_sign, 3, 2);
    layout->addWidget(btn_div, 3, 3);

    QPushButton* btn_7 = new QPushButton("7", this);
    QPushButton* btn_8 = new QPushButton("8", this);
    QPushButton* btn_9 = new QPushButton("9", this);
    QPushButton* btn_mul = new QPushButton("×", this);

    layout->addWidget(btn_7, 4, 0);
    layout->addWidget(btn_8, 4, 1);
    layout->addWidget(btn_9, 4, 2);
    layout->addWidget(btn_mul, 4, 3);

    QPushButton* btn_4 = new QPushButton("4", this);
    QPushButton* btn_5 = new QPushButton("5", this);
    QPushButton* btn_6 = new QPushButton("6", this);
    QPushButton* btn_sub = new QPushButton("−", this);

    layout->addWidget(btn_4, 5, 0);
    layout->addWidget(btn_5, 5, 1);
    layout->addWidget(btn_6, 5, 2);
    layout->addWidget(btn_sub, 5, 3);

    QPushButton* btn_1 = new QPushButton("1", this);
    QPushButton* btn_2 = new QPushButton("2", this);
    QPushButton* btn_3 = new QPushButton("3", this);
    QPushButton* btn_add = new QPushButton("+", this);

    layout->addWidget(btn_1, 6, 0);
    layout->addWidget(btn_2, 6, 1);
    layout->addWidget(btn_3, 6, 2);
    layout->addWidget(btn_add, 6, 3);

    QPushButton* btn_dot = new QPushButton(".", this);
    QPushButton* btn_0 = new QPushButton("0", this);
    QPushButton* btn_back = new QPushButton("⌫", this);
    QPushButton* btn_eq = new QPushButton("=", this);

    layout->addWidget(btn_dot, 7, 0);
    layout->addWidget(btn_0, 7, 1);
    layout->addWidget(btn_back, 7, 2);
    layout->addWidget(btn_eq, 7, 3);

    connect(btn_0, &QPushButton::clicked, this, &MainWindow::OnDigitClicked);
    connect(btn_1, &QPushButton::clicked, this, &MainWindow::OnDigitClicked);
    connect(btn_2, &QPushButton::clicked, this, &MainWindow::OnDigitClicked);
    connect(btn_3, &QPushButton::clicked, this, &MainWindow::OnDigitClicked);
    connect(btn_4, &QPushButton::clicked, this, &MainWindow::OnDigitClicked);
    connect(btn_5, &QPushButton::clicked, this, &MainWindow::OnDigitClicked);
    connect(btn_6, &QPushButton::clicked, this, &MainWindow::OnDigitClicked);
    connect(btn_7, &QPushButton::clicked, this, &MainWindow::OnDigitClicked);
    connect(btn_8, &QPushButton::clicked, this, &MainWindow::OnDigitClicked);
    connect(btn_9, &QPushButton::clicked, this, &MainWindow::OnDigitClicked);

    connect(btn_dot, &QPushButton::clicked, this, &MainWindow::OnDotClicked);
    connect(btn_sign, &QPushButton::clicked, this, &MainWindow::OnSignClicked);
    connect(btn_back, &QPushButton::clicked, this, &MainWindow::OnBackspaceClicked);
    connect(btn_c, &QPushButton::clicked, this, &MainWindow::OnClearClicked);

    connect(btn_add, &QPushButton::clicked, this, &MainWindow::OnOperationClicked);
    connect(btn_sub, &QPushButton::clicked, this, &MainWindow::OnOperationClicked);
    connect(btn_mul, &QPushButton::clicked, this, &MainWindow::OnOperationClicked);
    connect(btn_div, &QPushButton::clicked, this, &MainWindow::OnOperationClicked);
    connect(btn_pow, &QPushButton::clicked, this, &MainWindow::OnOperationClicked);

    connect(btn_eq, &QPushButton::clicked, this, &MainWindow::OnEqualsClicked);

    connect(btn_mc, &QPushButton::clicked, this, &MainWindow::OnMCClicked);
    connect(btn_mr, &QPushButton::clicked, this, &MainWindow::OnMRClicked);
    connect(btn_ms, &QPushButton::clicked, this, &MainWindow::OnMSClicked);
}

void MainWindow::SetText(const QString& text) {
    input_number_ = NormalizeNumber(text);
    l_result->setText(input_number_);
    active_number_ = input_number_.toDouble();
}

void MainWindow::AddText(const QString& suffix) {
    SetText(input_number_ + suffix);
}

void MainWindow::OnDigitClicked() {
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (btn) {
        AddText(btn->text());
    }
}

void MainWindow::OnDotClicked() {
    if (!input_number_.contains('.')) {
        AddText(".");
    }
}

void MainWindow::OnSignClicked() {
    if (input_number_.startsWith("-")) {
        SetText(input_number_.mid(1));
    } else {
        SetText("-" + input_number_);
    }
}

void MainWindow::OnBackspaceClicked() {
    if (!input_number_.isEmpty()) {
        input_number_.chop(1);
        SetText(input_number_);
    }
}

void MainWindow::OnClearClicked() {
    current_operation_ = Operation::NO_OPERATION;
    l_formula->setText("");
    SetText("0");
}

QString MainWindow::OpToString(Operation op) const {
    switch(op) {
    case Operation::NO_OPERATION: return "";
    case Operation::ADDITION: return "+";
    case Operation::DIVISION: return "÷";
    case Operation::MULTIPLICATION: return "×";
    case Operation::SUBTRACTION: return "−";
    case Operation::POWER: return "^";
    }
    return "";
}

void MainWindow::SetOperation(Operation op) {
    if (current_operation_ == Operation::NO_OPERATION) {
        calculator_.Set(active_number_);
    }
    current_operation_ = op;
    l_formula->setText(QString::number(calculator_.GetNumber()) + " " + OpToString(op));
    input_number_.clear();
}

void MainWindow::OnOperationClicked() {
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    QString text = btn->text();
    if (text == "+") SetOperation(Operation::ADDITION);
    else if (text == "−") SetOperation(Operation::SUBTRACTION);
    else if (text == "×") SetOperation(Operation::MULTIPLICATION);
    else if (text == "÷") SetOperation(Operation::DIVISION);
    else if (text == "xʸ") SetOperation(Operation::POWER);
}

void MainWindow::OnEqualsClicked() {
    if (current_operation_ == Operation::NO_OPERATION) return;

    QString formula = QString::number(calculator_.GetNumber()) + " " +
                      OpToString(current_operation_) + " " +
                      QString::number(active_number_) + " =";
    l_formula->setText(formula);

    switch (current_operation_) {
    case Operation::ADDITION: calculator_.Add(active_number_); break;
    case Operation::SUBTRACTION: calculator_.Sub(active_number_); break;
    case Operation::MULTIPLICATION: calculator_.Mul(active_number_); break;
    case Operation::DIVISION: calculator_.Div(active_number_); break;
    case Operation::POWER: calculator_.Pow(active_number_); break;
    default: break;
    }

    active_number_ = calculator_.GetNumber();
    l_result->setText(QString::number(active_number_));
    input_number_.clear();
    current_operation_ = Operation::NO_OPERATION;
}

void MainWindow::OnMSClicked() {
    memory_ = active_number_;
    has_memory_ = true;
    l_memory->setText("M");
}

void MainWindow::OnMCClicked() {
    memory_ = 0.0;
    has_memory_ = false;
    l_memory->setText("");
}

void MainWindow::OnMRClicked() {
    if (has_memory_) {
        active_number_ = memory_;
        l_result->setText(QString::number(active_number_));
        input_number_.clear();
    }
}