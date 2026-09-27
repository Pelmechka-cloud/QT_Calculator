#pragma once

#include "calculator.h"

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void OnDigitClicked();
    void OnDotClicked();
    void OnSignClicked();
    void OnBackspaceClicked();
    void OnClearClicked();

    void OnOperationClicked();
    void OnEqualsClicked();

    void OnMCClicked();
    void OnMRClicked();
    void OnMSClicked();

private:
    enum class Operation {
        NO_OPERATION,
        ADDITION,
        SUBTRACTION,
        MULTIPLICATION,
        DIVISION,
        POWER
    };

    void SetupUI();
    void SetText(const QString& text);
    void AddText(const QString& suffix);
    void SetOperation(Operation op);
    QString OpToString(Operation op) const;

    Ui::MainWindow* ui;

    QLabel* l_memory = nullptr;
    QLabel* l_result = nullptr;
    QLabel* l_formula = nullptr;

    QString input_number_;
    Number active_number_ = 0.0;
    Calculator calculator_;
    Operation current_operation_ = Operation::NO_OPERATION;

    Number memory_ = 0.0;
    bool has_memory_ = false;
};