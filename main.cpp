#include <iostream>
#include <fstream>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <iomanip>
using namespace std;

int Menu(int k);

COORD Position;
int i,j,k,l,a[5][5],b[18][14],score=0,t,nr_sur,nr_tri,gamemode,cont;
char ci,ch;
clock_t inc;
ifstream f("score.txt");

struct nod
{
	char nume[5];
	int scor;
} scoretable[2][7];

void SetScreen(int x, int y, int color)
{
	Position.X=x; Position.Y=y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),Position);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),color);
}

void DrawMenuGraphic()
{
	for (i=1; i<=80; i+=5)
		for (j=2; j<=20; j+=3)
		{
			if (i%2) SetScreen(i,j,176);
			else SetScreen(i,j+=2,176);
			if (i%3) SetScreen(i,j,240);
			cout<<" ";
		}
}

void WriteScore()
{
	ofstream g("scorecopy.txt");
	g<<nr_sur<<"\n";
	for (i=1; i<=nr_sur; i++) g<<scoretable[1][i].nume<<"\n"<<scoretable[1][i].scor<<"\n";
	g<<nr_tri<<"\n";
	for (i=1; i<=nr_tri; i++) g<<scoretable[2][i].nume<<"\n"<<scoretable[2][i].scor<<"\n";
	f.close(); g.close();
	remove("score.txt"); rename("scorecopy.txt", "score.txt");
}

void MoveTilesAnimation()
{
	for (i=4; i>=1; i--)
		for (j=1; j<=4; j++)
			a[i][j]=a[i-1][j];
	a[1][rand()%4+1]=1;
	for (i=14; i>=3; i--)
	{
		for (j=2; j<=12; j++)
			if (i==14 || i==10 || i==6) b[i][j]=b[i-2][j];
			else b[i][j]=b[i-1][j];
		if (i==14 || i==10 || i==6) i--;
	}
}

bool CheckTile()
{
	if ((ch=='a' && a[4][1]==1) || (ch=='s' && a[4][2]==1) || (ch=='d' && a[4][3]==1) || (ch=='f' && a[4][4]==1)) MoveTilesAnimation();
	else return 0;
	if (++score%10==0 && gamemode==1) t+=5;
	ch=50;
	return 1;
}

void DrawClock()
{
	SetScreen(50,10,224);
	cout<<"Remaining time: "<<setw(7)<<t-((clock()-inc)/(double)CLOCKS_PER_SEC);
}

void DrawTiles()
{
	for (i=1; i<=4; i++)
		for (j=1; j<=4; j++)
			for (k=3*j-1; k<=3*j; k++)
				for (l=4*i-2; l<=4*i; l++)
					if (a[i][j]==1) b[l][k]=2;
					else b[l][k]=240;
	for (i=1; i<=17; i++)
		for (j=1; j<=13; j++)
		{
			SetScreen(j+24,i+2,b[i][j]); cout<<" ";
		}
	SetScreen(50,6,224); cout<<"Score: "<<score;
}

void InitGame()
{
	SetScreen(0,0,0); system("cls");
	f>>nr_sur; scoretable[1][0].scor=999;
	for (i=1; i<=nr_sur; i++)
	{
		f>>scoretable[1][i].nume>>scoretable[1][i].scor;
		if (gamemode==1)
		{
			SetScreen(5,5,224); cout<<"-Survival-";
			SetScreen(5,6+i,224);
			cout<<i<<". "<<scoretable[1][i].nume<<" "<<setw(3)<<scoretable[1][i].scor;
		}
	}
	f>>nr_tri; scoretable[2][0].scor=999;
	for (i=1; i<=nr_tri; i++)
	{
		f>>scoretable[2][i].nume>>scoretable[2][i].scor;
		if (gamemode==2)
		{
			SetScreen(4,5,224); cout<<"-Time trial-";
			SetScreen(5,6+i,224);
			cout<<i<<". "<<scoretable[2][i].nume<<" "<<setw(3)<<scoretable[2][i].scor;
		}
	}
	SetScreen(0,0,15);
	cout<<"                           -BlackTile-\n";
	cout<<"                      -Game made by Adryon-";
	SetScreen(0,24,224); cout<<"Copyright 2014. Game made by Adryon. All rights reserved.";
	if (gamemode==1 || gamemode==2)
	{
		if (gamemode==1) t=10;
		else if (gamemode==2) t=30;
		SetScreen(5,4,224); cout<<"HIGH SCORE";
		SetScreen(5,6,224); cout<<"          ";
	}
	for (i=1; i<=4; i++) a[i][rand()%4+1]=1;
	for (i=1; i<=17; i++)
		for (j=1; j<=13; j+=3)
			b[i][j]=176;
	for (i=1; i<=13; i++)
		for (j=1; j<=17; j+=4)
			if (j==13 || j==17) b[j][i]=224;
			else b[j][i]=176;
	srand(time(0));
	inc=clock();
	ch=50;
}

void Help()
{
	system("cls");
	SetScreen(10,4,240); cout<<"Jocul este impartit in doua moduri: Survival si Trial Attack.";
	SetScreen(0,6,240); cout<<"In modul Survival, trebuie sa rezisti cat mai mult posibil, apasarile corecte prelungindu-ti astfel timpul de joc.";
	SetScreen(0,9,240); cout<<"In modul Time Trial, trebuie sa apesi cat mai corect in timpul alocat.";
	SetScreen(10,11,240); cout<<"Tastele folosite in timpul jocului sunt 'a' 's' 'd' 'f'"; _getch(); exit(0);
}

int Menu(int k)
{
	SetScreen(0,24,224); cout<<"Copyright 2014. Game made by Adryon. All rights reserved.";
	SetScreen(26,4,240); cout<<"    BlackTile      ";
	SetScreen(26,7,224); cout<<" Survival.         ";
	SetScreen(26,8,224); cout<<" Time trial.       ";
	SetScreen(26,9,224); cout<<" Help.             ";
	SetScreen(26,10,224); cout<<" Exit.             ";
	SetScreen(26,6+k,224); cout<<"-";
	ci=_getch();
	if (ci==-32)
	{
		ch=_getch();
		if (ch==72)
			if (k>1) Menu(--k);
			else Menu(4);
		else if (ch==80)
			if (k<4) Menu(++k);
			else Menu(1);
	}
	else if (ci==13 && k==1) gamemode=1;
	else if (ci==13 && k==2) gamemode=2;
	else if (ci==13 && k==3) Help();
	else if (ci==13 && k==4) exit(0);
}

void PostGame()
{
	if (gamemode==1){ cont=nr_sur; SetScreen(18,13,176); cout<<"Game Over! Press any key to continue."; _getch();}
	else if (gamemode==2){ cont=nr_tri; SetScreen(18,13,176); cout<<"Time's up! Press any key to continue."; _getch();}
	for (i=cont+1; i>=1; i--)
		if (score>=scoretable[gamemode][i].scor && score<scoretable[gamemode][i-1].scor && score>0)
		{
			for (j=cont+1; j>=i; j--)
			{
				scoretable[gamemode][j+1].scor=scoretable[gamemode][j].scor;
				strcpy(scoretable[gamemode][j+1].nume,scoretable[gamemode][j].nume);
			}
			scoretable[gamemode][i].scor=score;
			if (i!=6){ SetScreen(18,20,176); cout<<"Please insert your name: "; cin.get(scoretable[gamemode][i].nume,4); strupr(scoretable[gamemode][i].nume);}
			if (cont<5) cont++;
			break;
		}
	WriteScore();
}

int main()
{
	DrawMenuGraphic();
	Menu(1);
	InitGame();
	while (1)
	{
		DrawTiles();
		do
		{
			if (_kbhit()) ch=_getch();
			if (t-((clock()-inc)/CLOCKS_PER_SEC)<=0) break;
			DrawClock();
		}while (!strchr("asdf",ch));
		if (!CheckTile()) break;
	}
	PostGame();
}
