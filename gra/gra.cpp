#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_audio.h>
#include <allegro5\allegro_acodec.h>

enum KEYS { A, S, E, D, C, Q, R, W, T };
bool keys[9] = { false, false, false, false, false,false , false,false,false };

int licznikListy = 0;
char board[3][3];
int polozenie;//1-sciana, 2-rog, 3-rog daleki
int globx, globy;
int globi;

void resetBoard()
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			board[i][j] = ' ';
		}
	}
}

int checkFreeSpaces()
{
	int freeSpaces = 9;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (board[i][j] != ' ')
			{
				freeSpaces--;
			}
		}
	}
	return freeSpaces;
}

int checkWinner()
{
	//rzedy
	for (int i = 0; i < 3; i++)
	{
		if (board[i][0] == board[i][1] && board[i][0] == board[i][2] && board[i][0] != ' ')
		{
			if (board[i][0] == 'X')return 1;
			else return 2;
		}
	}
	//kolumny
	for (int i = 0; i < 3; i++)
	{
		if (board[0][i] == board[1][i] && board[0][i] == board[2][i] && board[0][i] != ' ')
		{
			if (board[0][i] == 'X')return 1;
			else return 2;
		}
	}
	//przekatne
	if (board[0][0] == board[1][1] && board[0][0] == board[2][2] && board[0][0] != ' ')
	{
		if (board[0][0] == 'X')return 1;
		else return 2;
	}
	if (board[0][2] == board[1][1] && board[0][2] == board[2][0] && board[0][2] != ' ')
	{
		if (board[0][2] == 'X')return 1;
		else return 2;
	}

	return 0;
}
//1-X, 2-O

int Priorytet()
{
	//sprawdza czy nie moze wygrac

	int licznik;

	//rzedy
	for (int i = 0; i < 3; i++)
	{
		licznik = 0;
		for (int j = 0; j < 3; j++)
		{
			if (board[i][j] == 'O') licznik++;
		}
		if (licznik == 2)
		{
			for (int j = 0; j < 3; j++)
			{
				if (board[i][j] == ' ') {
					board[i][j] = 'O';
					if (i == 0 && j == 0) return 1;
					if (i == 0 && j == 1) return 2;
					if (i == 0 && j == 2) return 3;
					if (i == 1 && j == 0) return 4;
					if (i == 1 && j == 1) return 5;
					if (i == 1 && j == 2) return 6;
					if (i == 2 && j == 0) return 7;
					if (i == 2 && j == 1) return 8;
					if (i == 2 && j == 2) return 9;
				}
			}
		}
	}
	//kolumny
	for (int i = 0; i < 3; i++)
	{
		licznik = 0;
		for (int j = 0; j < 3; j++)
		{
			if (board[j][i] == 'O') licznik++;
		}
		if (licznik == 2)
		{
			for (int j = 0; j < 3; j++)
			{
				if (board[j][i] == ' ') {
					board[j][i] = 'O';
					if (j == 0 && i == 0) return 1;
					if (j == 0 && i == 1) return 2;
					if (j == 0 && i == 2) return 3;
					if (j == 1 && i == 0) return 4;
					if (j == 1 && i == 1) return 5;
					if (j == 1 && i == 2) return 6;
					if (j == 2 && i == 0) return 7;
					if (j == 2 && i == 1) return 8;
					if (j == 2 && i == 2) return 9;
				}
			}
		}
	}
	//jedna przekatna
	licznik = 0;
	for (int j = 0; j < 3; j++)
	{
		if (board[j][j] == 'O') licznik++;
	}
	if (licznik == 2)
	{
		for (int j = 0; j < 3; j++)
		{
			if (board[j][j] == ' ') {
				board[j][j] = 'O';
				if (j == 0) return 1;
				if (j == 1) return 5;
				if (j == 2) return 9;
			}
		}
	}
	//druga przekatna
	licznik = 0;
	int a = 0;
	int b = 2;
	while (a < 3 && b >= 0)
	{
		if (board[a][b] == 'O') licznik++;
		a++;
		b--;
	}
	if (licznik == 2)
	{
		a = 0; b = 2;
		while (a < 3 && b >= 0)
		{
			if (board[a][b] == ' ') {
				board[a][b] = 'O';
				if (a == 0 && b == 2) return 3;
				if (a == 1 && b == 1) return 5;
				if (a == 2 && b == 0) return 7;
			}
			a++;
			b--;
		}
	}

	//sprawdza czy nie musi blokowac
	//rzedy
	for (int i = 0; i < 3; i++)
	{
		licznik = 0;
		for (int j = 0; j < 3; j++)
		{
			if (board[i][j] == 'X') licznik++;
		}
		if (licznik == 2)
		{
			for (int j = 0; j < 3; j++)
			{
				if (board[i][j] == ' ') {
					board[i][j] = 'O';
					if (i == 0 && j == 0) return 1;
					if (i == 0 && j == 1) return 2;
					if (i == 0 && j == 2) return 3;
					if (i == 1 && j == 0) return 4;
					if (i == 1 && j == 1) return 5;
					if (i == 1 && j == 2) return 6;
					if (i == 2 && j == 0) return 7;
					if (i == 2 && j == 1) return 8;
					if (i == 2 && j == 2) return 9;
				}
			}
		}
	}
	//kolumny
	for (int i = 0; i < 3; i++)
	{
		licznik = 0;
		for (int j = 0; j < 3; j++)
		{
			if (board[j][i] == 'X') licznik++;
		}
		if (licznik == 2)
		{
			for (int j = 0; j < 3; j++)
			{
				if (board[j][i] == ' ') {
					board[j][i] = 'O';
					if (j == 0 && i == 0) return 1;
					if (j == 0 && i == 1) return 2;
					if (j == 0 && i == 2) return 3;
					if (j == 1 && i == 0) return 4;
					if (j == 1 && i == 1) return 5;
					if (j == 1 && i == 2) return 6;
					if (j == 2 && i == 0) return 7;
					if (j == 2 && i == 1) return 8;
					if (j == 2 && i == 2) return 9;
				}
			}
		}
	}
	//jedna przekatna
	licznik = 0;
	for (int j = 0; j < 3; j++)
	{
		if (board[j][j] == 'X') licznik++;
	}
	if (licznik == 2)
	{
		for (int j = 0; j < 3; j++)
		{
			if (board[j][j] == ' ') {
				board[j][j] = 'O';
				if (j == 0) return 1;
				if (j == 1) return 5;
				if (j == 2) return 9;
			}
		}
	}
	//druga przekatna
	licznik = 0;
	a = 0;
	b = 2;
	while (a < 3 && b >= 0)
	{
		if (board[a][b] == 'X')licznik++;
		a++;
		b--;
	}
	if (licznik == 2)
	{
		a = 0; b = 2;
		while (a < 3 && b >= 0)
		{
			if (board[a][b] == ' ') {
				board[a][b] = 'O';
				if (a == 0 && b == 2) return 3;
				if (a == 1 && b == 1) return 5;
				if (a == 2 && b == 0) return 7;
			}
			a++;
			b--;
		}
	}

	return 0;
}

int computerEasyMove()
{
	srand(time(0));
	int x;
	int y;

	do
	{
		x = rand() % 3;
		y = rand() % 3;
	} while (board[x][y] != ' ');

	board[x][y] = 'O';

	if (x == 0 && y == 0) return 1;
	if (x == 0 && y == 1) return 2;
	if (x == 0 && y == 2) return 3;
	if (x == 1 && y == 0) return 4;
	if (x == 1 && y == 1) return 5;
	if (x == 1 && y == 2) return 6;
	if (x == 2 && y == 0) return 7;
	if (x == 2 && y == 1) return 8;
	if (x == 2 && y == 2) return 9;

}

int computerNormalMove()
{
	switch (Priorytet())
	{
	case 1: return 1;
	case 2: return 2;
	case 3: return 3;
	case 4: return 4;
	case 5: return 5;
	case 6: return 6;
	case 7: return 7;
	case 8: return 8;
	case 9: return 9;
	}

	srand(time(0));
	int x;
	int y;

	do
	{
		x = rand() % 3;
		y = rand() % 3;
	} while (board[x][y] != ' ');

	board[x][y] = 'O';

	if (x == 0 && y == 0) return 1;
	if (x == 0 && y == 1) return 2;
	if (x == 0 && y == 2) return 3;
	if (x == 1 && y == 0) return 4;
	if (x == 1 && y == 1) return 5;
	if (x == 1 && y == 2) return 6;
	if (x == 2 && y == 0) return 7;
	if (x == 2 && y == 1) return 8;
	if (x == 2 && y == 2) return 9;
}


int computerHardMove()
{
	switch (Priorytet())
	{
	case 1: return 1;
	case 2: return 2;
	case 3: return 3;
	case 4: return 4;
	case 5: return 5;
	case 6: return 6;
	case 7: return 7;
	case 8: return 8;
	case 9: return 9;
	}

	srand(time(0));
	int x;
	int y;

	switch (checkFreeSpaces())
	{
		//jak komputer zaczyna
	case 9:
	{
		switch (rand() % 4)
		{
		case 0: x = 0; y = 0; break;
		case 1: x = 0; y = 2; break;
		case 2: x = 2; y = 0; break;
		case 3: x = 2; y = 2; break;
		}

		board[x][y] = 'O';
		globx = x;
		globy = y;
		if (x == 0 && y == 0) return 1;
		if (x == 0 && y == 1) return 2;
		if (x == 0 && y == 2) return 3;
		if (x == 1 && y == 0) return 4;
		if (x == 1 && y == 1) return 5;
		if (x == 1 && y == 2) return 6;
		if (x == 2 && y == 0) return 7;
		if (x == 2 && y == 1) return 8;
		if (x == 2 && y == 2) return 9;
	}
	case 7:
	{
		//wczytaj x, y
		x = globx;
		y = globy;

		//sciana
		if (board[0][1] == 'X' || board[2][1] == 'X' || board[1][0] == 'X' || board[1][2] == 'X')
		{
			board[1][1] = 'O';
			polozenie = 1;
			return 5;
		}

		//rog blisko

		if ((x == 0 && y == 0) || (x == 2 && y == 2))
		{
			if (board[0][2] == 'X')
			{
				board[2][0] = 'O';
				polozenie = 2;
				return 7;
			}
			else if (board[2][0] == 'X')
			{
				board[0][2] = 'O';
				polozenie = 2;
				return 3;
			}
		}
		else if ((x == 0 && y == 2) || (x == 2 && y == 0))
		{
			if (board[0][0] == 'X')
			{
				board[2][2] = 'O';
				polozenie = 2;
				return 9;
			}
			else if (board[2][2] == 'X')
			{
				board[0][0] = 'O';
				polozenie = 2;
				return 1;
			}
		}

		//rog daleko

		if (board[x - 2][y - 2] == 'X' || board[x + 2][y + 2] == 'X' || board[x - 2][y + 2] == 'X' || board[x + 2][y - 2] == 'X')
		{
			polozenie = 3;
			for (int b = 0; b < 3; b += 2)
			{
				if (board[0][b] == ' ')
				{
					board[0][b] = 'O';
					if (b == 0) return 1;
					if (b == 2) return 3;
				}
			}
			break;
		}

		//srodek
		if (board[1][1] == 'X')
		{
			if (x == 0 && y == 0) {
				board[2][2] = 'O';
				return 9;
			}
			else if (x == 0 && y == 2) {
				board[2][0] = 'O';
				return 7;
			}
			else if (x == 2 && y == 0) {
				board[0][2] = 'O';
				return 3;
			}
			else if (x == 2 && y == 2) {
				board[0][0] = 'O';
				return 1;
			}
		}

		break;
	}
	case 5:
	{
		switch (polozenie)
		{
		case 1: //sciana
		{
			x = globx;
			y = globy;
			if (x == 2)
			{
				if (board[x - 1][y] == ' ') {
					board[x - 2][y] = 'O';
					if (y == 0) return 1;
					if (y == 1) return 2;
					if (y == 2) return 3;
				}
				else if (y == 0) {
					board[2][2] = 'O';
					return 9;
				}
				else {
					board[2][0] = 'O';
					return 7;
				}
			}
			else
			{
				if (board[x + 1][y] == ' ') {
					board[x + 2][y] = 'O';
					if (y == 0) return 7;
					if (y == 1) return 8;
					if (y == 2) return 9;
				}
				else if (y == 0) {
					board[0][2] = 'O';
					return 3;
				}
				else {
					board[0][0] = 'O';
					return 1;
				}
			}
			break;
		}
		case 2:
		{
			x = globx;
			y = globy;
			if (x == 0 && y == 0) {
				board[2][2] = 'O';
				return 9;
			}
			else if (x == 0 && y == 2) {
				board[2][0] = 'O';
				return 7;
			}
			else if (x == 2 && y == 0) {
				board[0][2] = 'O';
				return 3;
			}
			else if (x == 2 && y == 2) {
				board[0][0] = 'O';
				return 1;
			}

			break;
		}
		case 3:
		{
			for (int a = 0; a < 3; a += 2)
			{
				if (board[2][a] == ' ')
				{
					board[2][a] = 'O';
					if (a == 0) return 7;
					if (a == 2) return 9;
				}
			}
			break;
		}
		}
		break;
	}
	//jak gracz zaczyna
	case 8:
	{
		//srodek
		if (board[1][1] == 'X')
		{
			switch (rand() % 4)
			{
			case 0: x = 0; y = 0; break;
			case 1: x = 0; y = 2; break;
			case 2: x = 2; y = 0; break;
			case 3: x = 2; y = 2; break;
			}

			board[x][y] = 'O';
			globx = x;
			globy = y;
			polozenie = 1;
			if (x == 0 && y == 0) return 1;
			if (x == 0 && y == 1) return 2;
			if (x == 0 && y == 2) return 3;
			if (x == 1 && y == 0) return 4;
			if (x == 1 && y == 1) return 5;
			if (x == 1 && y == 2) return 6;
			if (x == 2 && y == 0) return 7;
			if (x == 2 && y == 1) return 8;
			if (x == 2 && y == 2) return 9;
		}
		else
		{
			board[1][1] = 'O';
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					if (board[i][j] == 'X')
					{
						globx = i;
						globy = j;
					}
				}
			}
			polozenie = 2;

		}
		return 5;
	}
	case 6:
	{
		switch (polozenie)
		{
		case 1:
		{
			x = globx;
			y = globy;
			if ((x == 0 && y == 0) || (x == 2 && y == 2))
			{
				board[2][0] = 'O';
				return 7;
			}
			if ((x == 0 && y == 2) || (x == 2 && y == 0))
			{
				board[2][2] = 'O';
				return 9;
			}

			break;
		}
		case 2:
		{
			x = globx;
			y = globy;
			if ((x == 0 && y == 0) || (x == 0 && y == 2) || (x == 2 && y == 0) || (x == 2 && y == 2))
			{
				//czy na rogu zaczal
				if ((x == 0 && y == 0) && board[2][2] == ' ')
				{
					board[2][2] = 'O';
					return 9;
				}
				if ((x == 0 && y == 2) && board[2][0] == ' ')
				{
					board[2][0] = 'O';
					return 7;
				}
				if ((x == 2 && y == 0) && board[0][2] == ' ')
				{
					board[0][2] = 'O';
					return 3;
				}
				if ((x == 2 && y == 2) && board[0][0] == ' ')
				{
					board[0][0] = 'O';
					return 1;
				}

				switch (rand() % 4)
				{
				case 0: board[0][1] = 'O'; return 2;
				case 1: board[1][0] = 'O'; return 4;
				case 2: board[1][2] = 'O'; return 6;
				case 3: board[2][1] = 'O'; return 8;
				}
				break;
			}

			//jesli pierwszy byl sciana
			if ((x == 1 && y == 0) || (x == 0 && y == 1) || (x == 1 && y == 2) || (x == 2 && y == 1))
			{
				if (x == 1 && y == 0)
				{
					board[0][0] = 'O';
					return 1;
				}
				if (x == 0 && y == 1)
				{
					board[0][2] = 'O';
					return 3;
				}
				if (x == 1 && y == 2)
				{
					board[2][2] = 'O';
					return 9;
				}
				if (x == 2 && y == 1)
				{
					board[2][0] = 'O';
					return 7;
				}
			}

			else
			{
				do
				{
					x = rand() % 3;
					y = rand() % 3;
				} while (board[x][y] != ' ');

				board[x][y] = 'O';
				if (x == 0 && y == 0) return 1;
				if (x == 0 && y == 1) return 2;
				if (x == 0 && y == 2) return 3;
				if (x == 1 && y == 0) return 4;
				if (x == 1 && y == 1) return 5;
				if (x == 1 && y == 2) return 6;
				if (x == 2 && y == 0) return 7;
				if (x == 2 && y == 1) return 8;
				if (x == 2 && y == 2) return 9;

			}

			break;
		}
		}
		break;
	}
	default:
	{
		do
		{
			x = rand() % 3;
			y = rand() % 3;
		} while (board[x][y] != ' ');

		board[x][y] = 'O';
		if (x == 0 && y == 0) return 1;
		if (x == 0 && y == 1) return 2;
		if (x == 0 && y == 2) return 3;
		if (x == 1 && y == 0) return 4;
		if (x == 1 && y == 1) return 5;
		if (x == 1 && y == 2) return 6;
		if (x == 2 && y == 0) return 7;
		if (x == 2 && y == 1) return 8;
		if (x == 2 && y == 2) return 9;
	}
	}
	return 0;
}


struct HistoriaGier
{
	char* rodzajKomp;
	char* zwyciestwo;
};

struct HistoriaGier ZapiszGre(int tryb, int wygrana)
{
	if (wygrana == 1)
	{
		switch (tryb)
		{
		case 3: {
			struct HistoriaGier gra;
			gra.rodzajKomp = _strdup("Dumb Computer");
			gra.zwyciestwo = _strdup("Win                                               \n");
			return gra; }
		case 4: {
			struct HistoriaGier gra;
			gra.rodzajKomp = _strdup("Normal Computer");
			gra.zwyciestwo = _strdup("Win                                                \n");
			return gra; }
		case 5: {
			struct HistoriaGier gra;
			gra.rodzajKomp = _strdup("Inteligent Computer");
			gra.zwyciestwo = _strdup("Win                                                  \n");
			return gra; }
		}
	}
	else if (wygrana == 2)
	{
		switch (tryb)
		{
		case 3: {
			struct HistoriaGier gra;
			gra.rodzajKomp = _strdup("Dumb Computer");
			gra.zwyciestwo = _strdup("Loss                                             \n");
			return gra;
			break; }
		case 4: {
			struct HistoriaGier gra;
			gra.rodzajKomp = _strdup("Normal Computer");
			gra.zwyciestwo = _strdup("Loss                                        \n");
			return gra;
			break; }
		case 5: {
			struct HistoriaGier gra;
			gra.rodzajKomp = _strdup("Inteligent Computer");
			gra.zwyciestwo = _strdup("Loss                                        \n");
			return gra;
			break; }
		}
	}
	else if (wygrana == 0)
	{
		switch (tryb)
		{
		case 3: {
			struct HistoriaGier gra;
			gra.rodzajKomp = _strdup("Dumb Computer");
			gra.zwyciestwo = _strdup("Draw                                   \n");
			return gra;
			break; }
		case 4: {
			struct HistoriaGier gra;
			gra.rodzajKomp = _strdup("Normal Computer");
			gra.zwyciestwo = _strdup("Draw                                  \n");
			return gra;
			break; }
		case 5: {
			struct HistoriaGier gra;
			gra.rodzajKomp = _strdup("Inteligent Computer");
			gra.zwyciestwo = _strdup("Draw                                  \n");
			return gra;
			break; }
		}
	}

}

void ZapiszGrePlik(int tryb, int wygrana)
{
	FILE* plik;
	errno_t err;
	if ((err = fopen_s(&plik, "match_history.txt", "a")) != 0) {
		fprintf(stderr, "cannot open file");
	}
	else {

		struct HistoriaGier gra = ZapiszGre(tryb, wygrana);
		if (licznikListy == 6) {
			fprintf(plik, "\n");
			licznikListy = 0;
		}
		fprintf(plik, "Game Vs %s - %s", gra.rodzajKomp, gra.zwyciestwo);
		licznikListy++;

		fclose(plik);
	}
}

void draw_X(int x, int y)
{
	al_draw_line(x - 90, y - 90, x + 90, y + 90, al_map_rgb(0, 0, 255), 7);
	al_draw_line(x - 90, y + 90, x + 90, y - 90, al_map_rgb(0, 0, 255), 7);
}


int main()
{
	srand(time(0));
	int wynik = 0;
	int w = 1400;
	int h = 1000;
	int x = 0;
	int y = 0;
	bool done = false;
	bool draw = true;
	const int FPS = 60;

	ALLEGRO_DISPLAY* display = NULL;
	ALLEGRO_EVENT_QUEUE* event_queue = NULL;
	ALLEGRO_TIMER* timer = NULL;
	ALLEGRO_BITMAP* image = NULL;
	ALLEGRO_BITMAP* menu = NULL;
	ALLEGRO_BITMAP* mod = NULL;
	ALLEGRO_BITMAP* player = NULL;
	ALLEGRO_BITMAP* com = NULL;
	ALLEGRO_SAMPLE* click = NULL;
	ALLEGRO_SAMPLE* win = NULL;
	ALLEGRO_SAMPLE* los = NULL;

	if (!al_init())
		return -1;

	display = al_create_display(w, h);

	if (!display)
		return -1;

	al_init_primitives_addon();
	al_install_keyboard();
	al_init_font_addon();
	al_init_ttf_addon();
	al_install_mouse();
	al_install_audio();
	al_init_acodec_addon();

	image = al_create_bitmap(1400, 1000);
	menu = al_create_bitmap(400, 1000);
	mod = al_create_bitmap(400, 1000);
	player = al_create_bitmap(1400, 1000);
	com = al_create_bitmap(1400, 1000);

	ALLEGRO_FONT* font80 = al_load_font("arial.ttf", 80, 0);
	ALLEGRO_FONT* font40 = al_load_font("arial.ttf", 40, 0);
	ALLEGRO_FONT* font30 = al_load_font("arial.ttf", 30, 0);


	al_reserve_samples(3);
	click = al_load_sample("click.wav");
	los = al_load_sample("los.wav");
	win = al_load_sample("win.wav");
	

	al_set_target_bitmap(image);

	al_draw_rectangle(400, 70, 1300, 970, al_map_rgb(255, 255, 255), 1);


	al_set_target_bitmap(player);
	al_draw_line(700, 70, 700, 970, al_map_rgb(255, 255, 255), 1);
	al_draw_line(1000, 70, 1000, 970, al_map_rgb(255, 255, 255), 1);
	al_draw_line(400, 370, 1300, 370, al_map_rgb(255, 255, 255), 1);
	al_draw_line(400, 670, 1300, 670, al_map_rgb(255, 255, 255), 1);
	al_draw_text(font40, al_map_rgb(255, 255, 255), 420, 10, 0, "PLAYER MODE");

	al_set_target_bitmap(com);
	al_draw_line(700, 70, 700, 970, al_map_rgb(255, 255, 255), 1);
	al_draw_line(1000, 70, 1000, 970, al_map_rgb(255, 255, 255), 1);
	al_draw_line(400, 370, 1300, 370, al_map_rgb(255, 255, 255), 1);
	al_draw_line(400, 670, 1300, 670, al_map_rgb(255, 255, 255), 1);
	al_draw_text(font40, al_map_rgb(255, 255, 255), 420, 10, 0, "COMPUTER MODE");


	al_set_target_bitmap(menu);
	al_draw_text(font80, al_map_rgb(255, 255, 255), 20, 20, 0, "MENU");
	al_draw_text(font30, al_map_rgb(255, 255, 255), 20, 110, 0, "Player vs Player - [a]");
	al_draw_text(font30, al_map_rgb(255, 255, 255), 20, 150, 0, "Play with computer - [s]");
	al_draw_text(font40, al_map_rgb(255, 255, 255), 20, 800, 0, "Match history - [q]");
	al_draw_text(font40, al_map_rgb(255, 255, 255), 20, 870, 0, "RETURN - [r]");


	al_set_target_bitmap(mod);

	al_draw_text(font30, al_map_rgb(255, 255, 255), 20, 190, 0, "Easy - [e]");
	al_draw_text(font30, al_map_rgb(255, 255, 255), 20, 230, 0, "Medium - [d]");
	al_draw_text(font30, al_map_rgb(255, 255, 255), 20, 270, 0, "Hard - [c]");


	al_set_target_bitmap(al_get_backbuffer(display));






	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0 / FPS);
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_mouse_event_source());

	al_start_timer(timer);

	int mode = 0; //1-gracz//2-komp//3-easy/4-normalny/5-hard
	int wzor = 2;
	int czysto = 0;
	int blok1 = 1;//blok a,s
	int blok2 = 1;//blok e,d,c
	int beg = 0;
	int ruch = 0;
	

	resetBoard();

	while (!done)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);


		al_draw_bitmap(menu, 10, 10, 0);
		al_draw_bitmap(image, 10, 10, 0);


		if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			done = true;
		}

		else if (ev.type == ALLEGRO_KEY_DOWN);
		{
			switch (ev.keyboard.keycode)
			{

			case ALLEGRO_KEY_ESCAPE: done = true; break;

			case ALLEGRO_KEY_A:
				if (blok1 != 0) {

					al_draw_bitmap(player, 10, 10, 0);
					mode = 1;
					blok1 = 0;
					wzor = rand() % 2;

				}

				break;
			case ALLEGRO_KEY_S:
				if (blok1 != 0) {
					al_draw_bitmap(mod, 10, 10, 0);
					mode = 2;
					blok1 = 0;
				}
				break;
			case ALLEGRO_KEY_Q:
				if (blok1 != 0) {
					int pusta = 0;
					int f = 0;
					int linijki = 0;
					int koniec = 0;
					FILE* plik;
					errno_t err;
					char tekst[100];
					if ((err = fopen_s(&plik, "match_history.txt", "r")) != 0) {
						pusta = 1;
					}
					else {
						while (!feof(plik))
						{
							if (fgets(tekst, 100, plik) == NULL) break;
							al_draw_text(font40, al_map_rgb(255, 255, 255), 550, 150 + f, 0, tekst);
							f += 130;
							linijki++;
							al_flip_display();

							if (linijki == 6)
								if (fgets(tekst, 100, plik) == NULL) koniec = 1;

							if (linijki == 6 && koniec == 0)
							{
								al_rest(1.3);
								al_draw_filled_rectangle(410, 80, 1295, 965, al_map_rgb(0, 0, 0));
								al_flip_display();
								f = 0;
								linijki = 0;
							}
						}
						fclose(plik);
					}
					if (pusta == 1)
						al_draw_text(font40, al_map_rgb(255, 255, 255), 650, 425, 0, "No games were played");
					mode = 2;
					blok1 = 0;
				}
				break;
			case ALLEGRO_KEY_R:


				al_clear_to_color(al_map_rgb(0, 0, 0));
				resetBoard();
				mode = 0;
				blok1 = 1;
				wzor = 2;
				czysto = 0;
				wynik = 0;
				beg = 0;
				ruch = 0;

				break;
			}
		}

		if (mode == 2)
		{
			if (ev.type == ALLEGRO_KEY_DOWN);
			{
				switch (ev.keyboard.keycode)
				{
				case ALLEGRO_KEY_E:

					if (blok2 != 0) {
						al_draw_bitmap(com, 10, 10, 0);
						mode = 3;
						blok2 = 1;
						wzor = rand() % 2;
					}

					break;
				case ALLEGRO_KEY_D:

					if (blok2 != 0) {
						al_draw_bitmap(com, 10, 10, 0);
						mode = 4;
						blok2 = 1;
						wzor = rand() % 2;
					}

					break;

				case ALLEGRO_KEY_C:

					if (blok2 != 0) {
						al_draw_bitmap(com, 10, 10, 0);
						mode = 5;
						blok2 = 1;
						wzor = rand() % 2;
					}
					break;


				}
			}
		}

		if (wzor == 0 && beg == 0) {
			if (mode == 1)
				al_draw_text(font40, al_map_rgb(255, 255, 255), 930, 20, 0, "O PLAYER BEGINS");
			else if (mode > 2) {
				al_draw_text(font40, al_map_rgb(255, 255, 255), 900, 20, 0, "COMPUTER BEGINS");
				ruch = 1;
			}
			beg = 1;
		}
		else if (wzor == 1 && beg == 0) {
			al_draw_text(font40, al_map_rgb(255, 255, 255), 930, 20, 0, "X PLAYER BEGINS");
			beg = 1;
		}


		if (mode == 1 || mode == 3 || mode == 4 || mode == 5) {

			if (mode > 2)
			{
				if (ruch == 1 && wynik == 0)
				{
					al_rest(0.3);
					switch (mode) {
					case 3:
					{
						switch (computerEasyMove()) {
						case 1: {
							x = 550;
							y = 220;
							break;
						}
						case 2:
						{
							x = 850;
							y = 220;
							break;
						}
						case 3:
						{
							x = 1150;
							y = 220;
							break;
						}
						case 4:
						{
							x = 550;
							y = 520;
							break;
						}
						case 5:
						{
							x = 850;
							y = 520;
							break;
						}
						case 6:
						{
							x = 1150;
							y = 520;
							break;
						}
						case 7:
						{
							x = 550;
							y = 820;
							break;
						}
						case 8:
						{
							x = 850;
							y = 820;
							break;
						}
						case 9:
						{
							x = 1150;
							y = 820;
							break;
						}
						}
						czysto = 1;
						wzor = 1;
						ruch = 0;
						wynik = checkWinner();
						break;
					}
					case 4:
					{
						switch (computerNormalMove()) {
						case 1: {
							x = 550;
							y = 220;
							break;
						}
						case 2:
						{
							x = 850;
							y = 220;
							break;
						}
						case 3:
						{
							x = 1150;
							y = 220;
							break;
						}
						case 4:
						{
							x = 550;
							y = 520;
							break;
						}
						case 5:
						{
							x = 850;
							y = 520;
							break;
						}
						case 6:
						{
							x = 1150;
							y = 520;
							break;
						}
						case 7:
						{
							x = 550;
							y = 820;
							break;
						}
						case 8:
						{
							x = 850;
							y = 820;
							break;
						}
						case 9:
						{
							x = 1150;
							y = 820;
							break;
						}
						}
						czysto = 1;
						wzor = 1;
						ruch = 0;
						wynik = checkWinner();
						break;
					}
					case 5:
					{
						switch (computerHardMove()) {
						case 1: {
							x = 550;
							y = 220;
							break;
						}
						case 2:
						{
							x = 850;
							y = 220;
							break;
						}
						case 3:
						{
							x = 1150;
							y = 220;
							break;
						}
						case 4:
						{
							x = 550;
							y = 520;
							break;
						}
						case 5:
						{
							x = 850;
							y = 520;
							break;
						}
						case 6:
						{
							x = 1150;
							y = 520;
							break;
						}
						case 7:
						{
							x = 550;
							y = 820;
							break;
						}
						case 8:
						{
							x = 850;
							y = 820;
							break;
						}
						case 9:
						{
							x = 1150;
							y = 820;
							break;
						}
						}
						czysto = 1;
						wzor = 1;
						ruch = 0;
						wynik = checkWinner();
						break;
					}

					}
				}
			}

			if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
			{

				if (ev.mouse.x > 400 && ev.mouse.x < 700 && ev.mouse.y > 70 && ev.mouse.y < 370)
				{
					al_play_sample(click, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
					if (board[0][0] == ' ') {
						x = 550;
						y = 220;
						czysto = 1;
						if (wzor == 0) {
							board[0][0] = 'O';
							wzor = 1;
						}
						else {
							board[0][0] = 'X';
							wzor = 0;
							ruch = 1;
						}
						wynik = checkWinner();

					}
				}
				if (ev.mouse.x > 700 && ev.mouse.x < 1000 && ev.mouse.y > 70 && ev.mouse.y < 370)
				{
					al_play_sample(click, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
					if (board[0][1] == ' ') {
						x = 850;
						y = 220;
						czysto = 1;
						if (wzor == 0) {
							board[0][1] = 'O';
							wzor = 1;
						}
						else {
							wzor = 0;
							board[0][1] = 'X';
							ruch = 1;
						}
						wynik = checkWinner();

					}
				}
				if (ev.mouse.x > 1000 && ev.mouse.x < 1300 && ev.mouse.y > 70 && ev.mouse.y < 370)
				{
					al_play_sample(click, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
					if (board[0][2] == ' ') {
						x = 1150;
						y = 220;
						czysto = 1;
						if (wzor == 0) {
							board[0][2] = 'O';
							wzor = 1;
						}
						else {
							board[0][2] = 'X';
							wzor = 0;
							ruch = 1;
						}
						wynik = checkWinner();

					}
				}
				if (ev.mouse.x > 400 && ev.mouse.x < 700 && ev.mouse.y > 370 && ev.mouse.y < 670)
				{
					al_play_sample(click, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
					if (board[1][0] == ' ') {
						x = 550;
						y = 520;
						czysto = 1;
						if (wzor == 0) {
							board[1][0] = 'O';
							wzor = 1;
						}
						else {
							board[1][0] = 'X';
							wzor = 0;
							ruch = 1;
						}
						wynik = checkWinner();

					}
				}
				if (ev.mouse.x > 700 && ev.mouse.x < 1000 && ev.mouse.y > 370 && ev.mouse.y < 670)
				{
					al_play_sample(click, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
					if (board[1][1] == ' ') {
						x = 850;
						y = 520;
						czysto = 1;
						if (wzor == 0) {
							board[1][1] = 'O';
							wzor = 1;
						}
						else {
							board[1][1] = 'X';
							wzor = 0;
							ruch = 1;
						}
						wynik = checkWinner();

					}
				}
				if (ev.mouse.x > 1000 && ev.mouse.x < 1300 && ev.mouse.y > 370 && ev.mouse.y < 670)
				{
					al_play_sample(click, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
					if (board[1][2] == ' ') {
						x = 1150;
						y = 520;
						czysto = 1;
						if (wzor == 0) {
							board[1][2] = 'O';
							wzor = 1;
						}
						else {
							board[1][2] = 'X';
							wzor = 0;
							ruch = 1;
						}
						wynik = checkWinner();

					}
				}
				if (ev.mouse.x > 400 && ev.mouse.x < 700 && ev.mouse.y > 670 && ev.mouse.y < 970)
				{
					al_play_sample(click, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
					if (board[2][0] == ' ') {
						x = 550;
						y = 820;
						czysto = 1;
						if (wzor == 0) {
							board[2][0] = 'O';
							wzor = 1;
						}
						else {
							board[2][0] = 'X';
							wzor = 0;
							ruch = 1;
						}
						wynik = checkWinner();

					}
				}
				if (ev.mouse.x > 700 && ev.mouse.x < 1000 && ev.mouse.y > 670 && ev.mouse.y < 970)
				{
					al_play_sample(click, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
					if (board[2][1] == ' ') {
						x = 850;
						y = 820;
						czysto = 1;
						if (wzor == 0) {
							board[2][1] = 'O';
							wzor = 1;
						}
						else {
							board[2][1] = 'X';
							wzor = 0;
							ruch = 1;
						}
						wynik = checkWinner();

					}
				}
				if (ev.mouse.x > 1000 && ev.mouse.x < 1300 && ev.mouse.y > 670 && ev.mouse.y < 970)
				{
					al_play_sample(click, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
					if (board[2][2] == ' ') {
						x = 1150;
						y = 820;
						czysto = 1;
						if (wzor == 0) {
							board[2][2] = 'O';
							wzor = 1;
						}
						else {
							board[2][2] = 'X';
							wzor = 0;
							ruch = 1;
						}
						wynik = checkWinner();

					}
				}
			}
		}


		if (wzor == 0 && czysto == 1) {
			draw_X(x, y);
		}

		else if (wzor == 1 && czysto == 1) {
			al_draw_circle(x, y, 100, al_map_rgb(255, 0, 255), 7);
		}

		if (wynik != 0) {
			if (mode == 1) {
				if (wynik == 2)
					al_draw_text(font80, al_map_rgb(255, 255, 255), 550, 475, 0, "O PLAYER WINS!");
				if (wynik == 1)
					al_draw_text(font80, al_map_rgb(255, 255, 255), 550, 475, 0, "X PLAYER WINS!");
				al_play_sample(win, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
			}
			else if (mode == 3 || mode == 4 || mode == 5) {
				if (wynik == 2) {
					al_draw_text(font80, al_map_rgb(255, 255, 255), 700, 475, 0, "YOU LOSE!");
					al_play_sample(los, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
				}
				if (wynik == 1) {
					al_draw_text(font80, al_map_rgb(255, 255, 255), 700, 475, 0, "YOU WIN!");
					al_play_sample(win, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
				}
				ZapiszGrePlik(mode, wynik);
			}
			blok1 = 0;
			mode = 0;
			wzor = 2;
		}

		if (checkFreeSpaces() == 0 && wynik == 0) {
			
			al_draw_text(font80, al_map_rgb(255, 255, 255), 700, 475, 0, "DRAW!");
			
			
			if (mode > 2)
				ZapiszGrePlik(mode, wynik);
			blok1 = 0;
			mode = 0;
			wzor = 2;

		}
		


		al_flip_display();


	}
	al_destroy_event_queue(event_queue);
	al_destroy_timer(timer);
	al_destroy_font(font80);
	al_destroy_font(font30);
	al_destroy_font(font40);
	al_destroy_display(display);
	al_destroy_bitmap(image);
	al_destroy_bitmap(menu);
	al_destroy_bitmap(com);
	al_destroy_bitmap(player);
	al_destroy_bitmap(mod);
	al_uninstall_keyboard();
	al_uninstall_mouse();
	al_destroy_sample(click);
	al_destroy_sample(win);
	al_destroy_sample(los);
	



	return 0;
}