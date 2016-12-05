#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qpainter.h"
#include "qevent.h"
#include "qpoint.h"
#include "qmessagebox.h"

#include "Client.h"

#define SERVER_IP   "127.0.0.1"
#define PORT		3333

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	ui->mainToolBar->hide();
	ui->menuBar->hide();

	resize((CHESS_COLUMES + 1)*RECT_WIDTH  ,(CHESS_ROWS + 1)*RECT_HEIGHT);

	m_bIsBlackTurn = true;

	
    m_pClient = new Client(SERVER_IP, PORT);
	connect(m_pClient, SIGNAL(OtherUserMove(int,int)),this, SLOT(OtherUserMove(int,int)));
	
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *e)
{
	DrawChessboard();
	DrawItems();
	DrawItemWithMouse();

	update();
}

void MainWindow::DrawChessboard()
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::HighQualityAntialiasing, true); 
	painter.setBrush(Qt::darkYellow);
	painter.setPen(QPen(QColor(Qt::black),2));

	for(int i = 0;i<CHESS_COLUMES; i++)
	{
		for (int j = 0; j<CHESS_ROWS; j++)
		{
			painter.drawRect( (i+0.5)*RECT_WIDTH,(j+0.5)*RECT_HEIGHT,RECT_WIDTH,RECT_HEIGHT);
		}
	}
}

void MainWindow::DrawItems()
{
	QPainter painter(this);
	painter.setPen(QPen(QColor(Qt::transparent)));

	for (int i = 0; i<m_items.size(); i++)
	{
		Item item = m_items[i];
		if (item.m_bBlack)
		{
			painter.setBrush(Qt::black);
		}
		else
		{
			painter.setBrush(Qt::white);
		}
		DrawChessAtPoint(painter,item.m_pt);
	}
}

void MainWindow::DrawChessAtPoint(QPainter& painter,QPoint& pt)
{
	//painter.drawRect( (pt.x()+0.5)*RECT_WIDTH,(pt.y()+0.5)*RECT_HEIGHT,RECT_WIDTH,RECT_HEIGHT);

	QPoint ptCenter((pt.x()+0.5)*RECT_WIDTH,(pt.y()+0.5)*RECT_HEIGHT);
	painter.drawEllipse(ptCenter,RECT_WIDTH / 2,RECT_HEIGHT / 2);
}

void MainWindow::DrawItemWithMouse()
{
	QPainter painter(this);
	painter.setPen(QPen(QColor(Qt::transparent)));

	if (m_pClient->IsBlack())
	{
		painter.setBrush(Qt::black);
	}
	else
	{
		painter.setBrush(Qt::white);
	}

	painter.drawEllipse(mapFromGlobal(QCursor::pos()),RECT_WIDTH / 2,RECT_HEIGHT / 2);
	
}

void MainWindow::mousePressEvent(QMouseEvent * e)
{
	if (m_pClient->IsBlack())
	{
		if (!m_bIsBlackTurn)
		{
			return;
		}
	}
	else
	{
		if (m_bIsBlackTurn)
		{
			return;
		}
	}

	//QPoint ptCur = e->pos();
	QPoint pt;
	pt.setX( (e->pos().x() ) / RECT_WIDTH);
	pt.setY( (e->pos().y() ) / RECT_HEIGHT);

	for (int i = 0; i<m_items.size(); i++)
	{
		Item item = m_items[i];
		if (item.m_pt == pt)
		{
			//已有棋子
			return;
		}
	}
	m_pClient->QueryMove(pt.x(),pt.y());

	Item item(pt,m_bIsBlackTurn);
	m_items.append(item);

	int nLeft =			CountNearItem(item,QPoint(-1,0));
	int nLeftUp =		CountNearItem(item,QPoint(-1,-1));
	int nUp =			CountNearItem(item,QPoint(0,-1));
	int nRightUp =		CountNearItem(item,QPoint(1,-1));
	int nRight =		CountNearItem(item,QPoint(1,0));
	int nRightDown =	CountNearItem(item,QPoint(1,1));
	int nDown =			CountNearItem(item,QPoint(0,1));
	int nLeftDown =		CountNearItem(item,QPoint(-1,1));
	if ( (nLeft + nRight) >= 4 ||
		 (nLeftUp + nRightDown) >= 4 ||
		 (nUp + nDown) >= 4 ||
		 (nRightUp + nLeftDown) >= 4 )
	{
		if (m_bIsBlackTurn == m_pClient->IsBlack())
		{
			//赢的一方通知服务器自己赢了
			m_pClient->QuerySuccess();
		}

		QString str = m_bIsBlackTurn?"  Black WIN!!!  ":"  White WIN!!!  ";
		QMessageBox::information(NULL,  "  GAME OVER  ",str, QMessageBox::Yes , QMessageBox::Yes);
		m_items.clear();
		//NewGame();
		return;
	}


	m_bIsBlackTurn = !m_bIsBlackTurn;

	
}

int MainWindow::CountNearItem(Item item,QPoint ptDirection)
{
	int nCount = 0;
	item.m_pt += ptDirection;
	
	while (m_items.contains(item))
	{
		nCount++;
		item.m_pt += ptDirection;
	}
	return nCount;
}

void MainWindow::OtherUserMove( int x,int y )
{
	QPoint pt(x,y);
	Item item(pt,m_bIsBlackTurn);
	m_items.append(item);

	int nLeft =			CountNearItem(item,QPoint(-1,0));
	int nLeftUp =		CountNearItem(item,QPoint(-1,-1));
	int nUp =			CountNearItem(item,QPoint(0,-1));
	int nRightUp =		CountNearItem(item,QPoint(1,-1));
	int nRight =		CountNearItem(item,QPoint(1,0));
	int nRightDown =	CountNearItem(item,QPoint(1,1));
	int nDown =			CountNearItem(item,QPoint(0,1));
	int nLeftDown =		CountNearItem(item,QPoint(-1,1));
	if ( (nLeft + nRight) >= 4 ||
		(nLeftUp + nRightDown) >= 4 ||
		(nUp + nDown) >= 4 ||
		(nRightUp + nLeftDown) >= 4 )
	{
		QString str = m_bIsBlackTurn?"  Black WIN!!!  ":"  White WIN!!!  ";
		QMessageBox::information(NULL,  "  GAME OVER  ",str, QMessageBox::Yes , QMessageBox::Yes);
		m_items.clear();
		//NewGame();
		return;
	}


	m_bIsBlackTurn = !m_bIsBlackTurn;
	update();
}



//bool MainWindow::FindItem(Item item)
//{
//	bool bFind = false;
//	for (int i = 0; i<m_items.size(); i++)
//	{
//		Item item = m_items[i];
//		if (item.m_pt == pt)
//		{
//			bFind = true;
//			break;
//		}
//	}
//	return bFind;
//}
