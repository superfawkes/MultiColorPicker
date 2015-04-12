#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->btnAddLayer, SIGNAL(clicked()), SLOT(OnAddLayer()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OnAddLayer()
{
    ui->widget->setNumLayers(ui->widget->NumLayers() + 1);
}
