#ifndef SCALA_H
#define SCALA_H
#include "allegro5/allegro.h"
#include "allegro5/allegro_image.h"
#include "Giocatore.h"

class Scala  {
private:
	int 	dimX,
			dimY,
			posX,
			posY;
	bool 	tipo;
	//tipo0(0) dim_x = 46, dim_y = 135
	//tipo1(1) dim_x = 45, dim_y = 166
	ALLEGRO_BITMAP *bitmap;

public:
	Scala(): dimX(0), dimY(0), posX(0), posY(0), tipo(false)  { bitmap = NULL; }
  	Scala(int,int, bool);				
  	~Scala();

	float getX() const  { return posX; }
	float getY() const  { return posY; }
    bool getTipo() const {return tipo;}

	void Draw()  { al_draw_bitmap(bitmap,posX,posY,0); }

	bool playerHere(Giocatore*);
};


#endif
