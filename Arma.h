#ifndef ARMA_H
#define ARMA_H
#include "allegro5/allegro.h"
#include "allegro5/allegro_image.h"

class Arma  {
private:
	float dim;
  	int posX;
  	int posY;
  	ALLEGRO_BITMAP *bitmap;
  	
public:
  	Arma(float, int, int);	
  	~Arma()  { al_destroy_bitmap(bitmap); }
	ALLEGRO_BITMAP* getBitmap() const  { return bitmap; }
	int getX() const  { return posX; }
	int getY() const  { return posY; }
	float getDim() const  { return dim; }
	void setBitmap(ALLEGRO_BITMAP *b_map)  { bitmap=b_map; }
	void setX(int x)  { posX=x; }
	void setY(int y)  { posY=y; }
	void setDim(float d)  { dim=d; }
	void Draw()  { al_draw_bitmap(bitmap,posX,posY,0); }
};

Arma::Arma(float d, int x, int y)  {
	dim = d;
	posX = x;
	posY = y;
	bitmap = al_load_bitmap("arpione.png");
}



#endif