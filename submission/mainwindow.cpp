#include "mainwindow.h"
#include "ui_mainwindow.h"

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
        QString rest = NormalizeNumber(text.mid(1));
        if (rest == "0") {
            return "0";
        }
        return "-" + rest;
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

    SetText("0");
    ui->l_memory->setText("");
    ui->l_formula->setText("");

    connect(ui->btn_0, &QPushButton::clicked, this, &MainWindow::OnDigitClicked);
    connect(ui->btn_1, &QPushButton::clicked, this, &MainWindow::OnDigitClicked);
    connect(ui->btn_2, &QPushButton::clicked, this, &MainWindow::OnDigitClicked);
    connect(ui->btn_3, &QPushButton::clicked, this, &MainWindow::OnDigitClicked);
    connect(ui->btn_4, &QPushButton::clicked, this, &MainWindow::OnDigitClicked);
    connect(ui->btn_5, &QPushButton::clicked, this, &MainWindow::OnDigitClicked);
    connect(ui->btn_6, &QPushButton::clicked, this, &MainWindow::OnDigitClicked);
    connect(ui->btn_7, &QPushButton::clicked, this, &MainWindow::OnDigitClicked);
    connect(ui->btn_8, &QPushButton::clicked, this, &MainWindow::OnDigitClicked);
    connect(ui->btn_9, &QPushButton::clicked, this, &MainWindow::OnDigitClicked);

    connect(ui->btn_dot,  &QPushButton::clicked, this, &MainWindow::OnDotClicked);
    connect(ui->btn_sign, &QPushButton::clicked, this, &MainWindow::OnSignClicked);
    connect(ui->btn_back, &QPushButton::clicked, this, &MainWindow::OnBackspaceClicked);
    connect(ui->btn_c,    &QPushButton::clicked, this, &MainWindow::OnClearClicked);
    connect(ui->btn_eq,   &QPushButton::clicked, this, &MainWindow::OnEqualsClicked);

    connect(ui->btn_mc, &QPushButton::clicked, this, &MainWindow::OnMCClicked);
    connect(ui->btn_mr, &QPushButton::clicked, this, &MainWindow::OnMRClicked);
    connect(ui->btn_ms, &QPushButton::clicked, this, &MainWindow::OnMSClicked);

    connect(ui->btn_add, &QPushButton::clicked, this, &MainWindow::OnAddClicked);
    connect(ui->btn_sub, &QPushButton::clicked, this, &MainWindow::OnSubClicked);
    connect(ui->btn_mul, &QPushButton::clicked, this, &MainWindow::OnMulClicked);
    connect(ui->btn_div, &QPushButton::clicked, this, &MainWindow::OnDivClicked);
    connect(ui->btn_pow, &QPushButton::clicked, this, &MainWindow::OnPowClicked);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::SetText(const QString& text) {
    input_number_ = NormalizeNumber(text);
    ui->l_result->setText(input_number_);
    active_number_ = input_number_.toDouble();
}

void MainWindow::AddText(const QString& suffix) {
    if (current_operation_ == Operation::NO_OPERATION) {
        ui->l_formula->setText("");
    }
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
    if (input_number_.isEmpty()) {
        return;
    }
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
    last_operation_ = Operation::NO_OPERATION;
    last_operand_ = 0.0;
    ui->l_formula->setText("");
    SetText("0");
}

QString MainWindow::OpToString(Operation op) const {
    switch (op) {
    case Operation::NO_OPERATION:    return "";
    case Operation::ADDITION:        return "+";
    case Operation::DIVISION:        return "÷";
    case Operation::MULTIPLICATION:  return "×";
    case Operation::SUBTRACTION:     return "−";
    case Operation::POWER:           return "^";
    }
    return "";
}

void MainWindow::SetOperation(Operation op) {
    if (current_operation_ == Operation::NO_OPERATION) {
        calculator_.Set(active_number_);
        input_number_.clear();
    }
    current_operation_ = op;
    last_operation_ = Operation::NO_OPERATION;
    last_operand_ = 0.0;
    ui->l_formula->setText(QString::number(calculator_.GetNumber()) + " " + OpToString(op));
}

void MainWindow::OnAddClicked() {
    SetOperation(Operation::ADDITION);
}

void MainWindow::OnSubClicked() {
    SetOperation(Operation::SUBTRACTION);
}

void MainWindow::OnMulClicked() {
    SetOperation(Operation::MULTIPLICATION);
}

void MainWindow::OnDivClicked() {
    SetOperation(Operation::DIVISION);
}

void MainWindow::OnPowClicked() {
    SetOperation(Operation::POWER);
}

void MainWindow::OnEqualsClicked() {
    Operation op;
    Number operand;

    if (current_operation_ == Operation::NO_OPERATION) {

        if (last_operation_ == Operation::NO_OPERATION) {
            return;
        }
        op = last_operation_;
        operand = last_operand_;
        calculator_.Set(active_number_);
    } else {
        op = current_operation_;
        operand = active_number_;
        last_operation_ = op;
        last_operand_ = operand;
    }

    QString formula = QString::number(calculator_.GetNumber()) + " " +
                      OpToString(op) + " " +
                      QString::number(operand) + " =";
    ui->l_formula->setText(formula);

    switch (op) {
    case Operation::ADDITION:       calculator_.Add(operand); break;
    case Operation::SUBTRACTION:    calculator_.Sub(operand); break;
    case Operation::MULTIPLICATION: calculator_.Mul(operand); break;
    case Operation::DIVISION:       calculator_.Div(operand); break;
    case Operation::POWER:          calculator_.Pow(operand); break;
    default: break;
    }

    active_number_ = calculator_.GetNumber();
    ui->l_result->setText(QString::number(active_number_));
    input_number_.clear();
    current_operation_ = Operation::NO_OPERATION;
}

void MainWindow::OnMSClicked() {
    memory_ = active_number_;
    has_memory_ = true;
    ui->l_memory->setText("M");
}

void MainWindow::OnMCClicked() {
    memory_ = 0.0;
    has_memory_ = false;
    ui->l_memory->setText("");
}

void MainWindow::OnMRClicked() {
    if (!has_memory_) return;

    if (current_operation_ == Operation::NO_OPERATION) {
        ui->l_formula->setText("");
    }

    SetText(QString::number(memory_));
}
