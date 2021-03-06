#include "headers/Esplosione.h"

Esplosione::Esplosione()  {
    posX=0; posY=0;
    dimX=0; dimY=0; tipo=pallaPic;
    animazione.setFrameCount(0);
    animazione.setFrameDelay(5);
    animazione.setCurrFrame(0);
    explosion=NULL;
}

Esplosione::Esplosione(int x, int y, Tipo tipo)  {
    posX=x;
    posY=y;
    this->tipo=tipo;
    if(tipo==pallaPic)
    {
        dimX=20; dimY=20;
        setExplosion(al_load_bitmap("../images/esplosionePic.png"));
    }    
    else if(tipo==pallaMed)
    {
        dimX=40; dimY=40;
        setExplosion(al_load_bitmap("../images/esplosioneMed.png"));
    }    
    else if(tipo==pallaGra)
    {
        dimX=70; dimY=70;
        setExplosion(al_load_bitmap("../images/esplosioneGra.png"));
    }    
    else if(tipo==bloccoPietra)
    {
        dimX=56; dimY=24;
        setExplosion(al_load_bitmap("../images/stoneAnimation.png"));
    }    
    else if(tipo==bloccoVetro1)
    {
        dimX=29; dimY=61;
        setExplosion(al_load_bitmap("../images/animazioneVetro2.png"));
    }    
    else if(tipo==bloccoVetro2)
    {
        dimX=41; dimY=24;
        setExplosion(al_load_bitmap("../images/animazioneVetro1.png"));
    }    
    animazione.setFrameCount(0);
    animazione.setFrameDelay(5);
    animazione.setCurrFrame(0);
}

void Esplosione::setTipo(int t)  {
    al_destroy_bitmap(explosion);
    switch(t)  {
        case 0:
            tipo=pallaPic;
            dimX=20; dimY=20;
            setExplosion(al_load_bitmap("../images/esplosionePic.png"));
            break;
        case 1:
            tipo=pallaMed;
            dimX=40; dimY=40;
            setExplosion(al_load_bitmap("../images/esplosioneMed.png"));
            break;
        case 2:
            tipo=pallaGra;
            dimX=70; dimY=70;
            setExplosion(al_load_bitmap("../images/esplosioneGra.png"));
            break;
        case 3:
            tipo=bloccoPietra;
            dimX=56; dimY=24;
            setExplosion(al_load_bitmap("../images/stoneAnimation.png"));
            break;
        case 4:
            tipo=bloccoVetro1;
            dimX=29; dimY=61;
            setExplosion(al_load_bitmap("../images/animazioneVetro2.png"));
            break;
        case 5:
            tipo=bloccoVetro2;
            dimX=41; dimY=24;
            setExplosion(al_load_bitmap("../images/animazioneVetro1.png"));
            break;
    }
}

bool Esplosione::Draw()  {
    if(tipo==pallaPic)  {
        if(animazione.eseguiFrame(explosion,getDimX(),getDimY(),getPosX(),getPosY(), false,5))
            return true;
    }            
    else if(animazione.eseguiFrame(explosion,getDimX(),getDimY(),getPosX(),getPosY(), false,4))
        return true;

    return false; 
}