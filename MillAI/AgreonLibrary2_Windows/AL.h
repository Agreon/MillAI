/* 
 * File:   AL.h
 * Author: daniel
 *
 * Created on February 2, 2015, 5:00 PM
 */

#ifndef AL_H
#define	AL_H

#include "Graphics.h"
#include "Animation.h"
#include "Input.h"
#include "Sound.h"
#include "Jsonparser.h"

struct Rect
{
	Rect()
	{
		x = y = w = h = 0;
	};
	Rect(int x, int y, int w, int h)
	{
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
	};
	int x;
	int y;
	int w;
	int h;
};

#endif	/* AL_H */

