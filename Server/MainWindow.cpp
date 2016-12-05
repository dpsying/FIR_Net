#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent,Qt::WindowFlags f)
    : QDialog(parent,f)
{
    setWindowTitle(QStringLiteral("TCP Server"));
	resize(500,600);

    m_ListWidget = new QListWidget;
   // PortLabel = new QLabel(QStringLiteral("端口："));
   // PortLineEdit = new QLineEdit;
   // CreateBtn = new QPushButton(QStringLiteral("创建对局"));

	mainLayout = new QGridLayout(this);
	mainLayout->addWidget(m_ListWidget,0,0,1,2);
   // mainLayout->addWidget(PortLabel,1,0);
   // mainLayout->addWidget(PortLineEdit,1,1);
   // mainLayout->addWidget(CreateBtn,1,0,1,2);
   // port=3333;
   // PortLineEdit->setText(QString::number(port));
    //connect(CreateBtn,SIGNAL(clicked()),this,SLOT(slotCreateServer()));

	server = new Server(this,3333);
	//connect(server,SIGNAL(sigUpdateText(QString,int)),this,SLOT(slotUpdateText(QString,int)));
	slotUpdateText(QStringLiteral("服务器开启，端口: 3333"));
}

MainWindow::~MainWindow()
{
    
}

//void MainWindow::slotCreateServer()
//{
//    server = new Server(this,3333);
//    connect(server,SIGNAL(sigUpdateText(QString,int)),this,SLOT(slotUpdateText(QString,int)));
//    CreateBtn->setEnabled(false);
//}

void MainWindow::slotUpdateText(QString msg)
{
    m_ListWidget->addItem(msg);
}
