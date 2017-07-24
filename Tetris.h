#pragma once

#include "resource.h"
#define MAP_WID   10
#define MAP_HIG	  15
#define INEDGE(point) (point.y <= 14 && point.y >= 0)

enum Tetris
{
	empty, tetri_i, tetri_j, tetri_l, tetri_s, tetri_o, tetri_z, tetri_t
};


class Tetri
{
public:
	Tetri() {	};
	Tetri(POINT center, Tetris t);
	Tetri(Tetri&);
	int GetBottom();
	VOID Rotate();
	VOID Down();

	POINT center;	//定义方块中心
	POINT p[3];
	Tetris t;
};

Tetri::Tetri(POINT center, Tetris t) : center(center), t(t)
{
	switch (t)
	{
	case tetri_i:
		{
			p[0].x = center.x;
			p[0].y = center.y - 2;
			p[1].x = center.x;
			p[1].y = center.y - 1;
			p[2].x = center.x;
			p[2].y = center.y + 1;
		}
		break;
	case tetri_j:
		{
			p[0].x = center.x - 1;
			p[0].y = center.y ;
			p[1].x = center.x + 1;
			p[1].y = center.y;
			p[2].x = center.x + 1;
			p[2].y = center.y + 1;
		}
		break;
	case tetri_l:
		{
			p[0].x = center.x - 1;
			p[0].y = center.y;
			p[1].x = center.x + 1;
			p[1].y = center.y;
			p[2].x = center.x - 1;
			p[2].y = center.y + 1;
		}
		break;
	case tetri_s:
		{
			p[0].x = center.x - 1;
			p[0].y = center.y;
			p[1].x = center.x;
			p[1].y = center.y - 1;
			p[2].x = center.x + 1;
			p[2].y = center.y - 1;
		}
		break;
	case tetri_o:
		{
			p[0].x = center.x - 1;
			p[0].y = center.y - 1;
			p[1].x = center.x - 1;
			p[1].y = center.y;
			p[2].x = center.x;
			p[2].y = center.y - 1;
		}
		break;
	case tetri_z:
		{
			p[0].x = center.x - 1;
			p[0].y = center.y - 1;
			p[1].x = center.x;
			p[1].y = center.y - 1;
			p[2].x = center.x + 1;
			p[2].y = center.y;
		}
		break;
	case tetri_t:
		{
			p[0].x = center.x - 1;
			p[0].y = center.y;
			p[1].x = center.x + 1;
			p[1].y = center.y;
			p[2].x = center.x;
			p[2].y = center.y + 1;
		}
		break;
	default:
		break;
	}
}

Tetri::Tetri(Tetri& tetri)
{
	center.x = tetri.center.x;
	center.y = tetri.center.y;
	for (int i = 0; i < 3; ++i)
	{
		p[i].x = tetri.p[i].x;
		p[i].y = tetri.p[i].y;
	}
	t = tetri.t;
}

VOID Tetri::Rotate()
{
	if (t == tetri_o)
		return;
	
	for (int i = 0; i < 3; ++i)
	{
		int dx = p[i].x - center.x;
		int dy = p[i].y - center.y;

		//左边的点右移、上移
		if (p[i].x < center.x)
		{
			p[i].x += dx;
			p[i].y -= dy;
		}
		//上边的点右移、下移
		if (p[i].y < center.y)
		{
			p[i].x += dx;
			p[i].y += dy;
		}
		//右边的点左移、下移
		if (p[i].x > center.x)
		{
			p[i].x -= dx;
			p[i].y += dy;
		}
		//下边的点左移、上移
		if (p[i].y > center.y)
		{
			p[i].x -= dx;
			p[i].y -= dy;
		}

		//如果过左界， 集体右移
		if (p[i].x < 0)
		{
			++center.x;
			for (int i = 0; i < 3; ++i)
			{
				++p[i].x;
			}
		}
		//如果过右界， 集体左移
		if (p[i].x >= MAP_WID)
		{
			--center.x;
			for (int i = 0; i < 3; ++i)
			{
				--p[i].x;
			}
		}
	}
	
}

VOID Tetri::Down()
{
	if (INEDGE(center))
		++center.y;
	for (int i = 0; i < 3; ++i)
		if (INEDGE(center))
			++p[i].y;
}


int Tetri::GetBottom()
{
	int y = center.y;
	for (int i = 0; i < 3; ++i)
	{
		if (p[i].y > y)
			y = p[i].y;
	}

	return y;
}