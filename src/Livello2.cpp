#include "headers/Livello2.h"

Livello2::Livello2(Livello1* L1, const int FPS)  {
	sfondo = al_load_bitmap("../images/sfondo2.jpg");
	SCREEN_W = L1->getSW();
	SCREEN_H = L1->getSH();
	GP = new GestorePalle(SCREEN_W, SCREEN_H);
	player = L1->getPlayer();
	font1=al_load_ttf_font("../fonts/SHREK.TTF",30,0);
	font2=al_load_ttf_font("../fonts/SHREK.TTF",25,0);
	vite_bmp = al_load_bitmap("../images/vita.png");
	timer = al_create_timer(1.0 / FPS);
	event_queue = al_create_event_queue();
	display = L1->getDisplay();
	pausa_play = al_load_bitmap("../images/pausa1.png");
	pausa_exit = al_load_bitmap("../images/pausa2.png");
	powerup = new PowerUp;
	
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue,al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));

	//liv 2 stuff
	scala1 = new Scala(160, 218, 0);
	scala2 = new Scala(434, 218, 0);
	piat1 = new Piattaforma(136, 204, false);
	piat2 = new Piattaforma(410, 204, false);
	drago= new Drago(120,120,640,220,560); // posDrago finale 530,220
	drago->setFuocoX(560);
	drago->setX(640);
}

Livello2::~Livello2()  {
	if(scala1)  {
		delete scala1;
	}
	if(scala2)  {
		delete scala2;
	}
	if(piat1)  {
		delete piat1;
	}
	if(piat2)  {
		delete piat2;
	}
	if(drago)  	{
		delete drago;
	}
}

void Livello2::regolaPalle()  {
	GP->aggiungiPalla(SCREEN_W - 300, 150, MED, RED, true);
	GP->aggiungiPalla(300, 150, MED, RED, false);
}

CASO Livello2::Esegui(int vite, int& punteggio, float res_info[])  {
	//DICHIARAZIONE VARIABILI ALLEGRO
	ALLEGRO_TRANSFORM 		redimencionamento;

	//DICHIARAZIONE ALTRE VARIABILI 
	bool 	colpito=false, sfondo2=false, presa=false, redraw = true, 
			keyRight=false, keyLeft=false, keySpace=false, toLeft=false, 
			bitmap_ = true, fullscreen=false, climbing=false, trans=true, 
			keyUp = false, keyDown = false, onlyLeftRight=false, hit=false,
			dragonArrive=true, spitting=false, fire=false, colpitoFuoco=false,
			hook_colp=false, keyUpDown=false, dragonSound=false,p_hit=false,
			p_hitFire = false;

	drawShoot=false; caduto=false; shoot=false; 
	MatchOver=false; drawExplosion=false;

	int 	tempo=9000, H_arma=0, fireCount=300; //fireCount timer per spitFire 300=5 sec
	int 	spawnY, terrain = PLAYER_ALT_NORM+player->getDimY(),
			platform = PLAYER_ALT_PIAT+player->getDimY(), timeEffect = 0;
	CASO 	return_value = EXIT;
	bool 	next[4] = { false };
	
	sound=new SoundEffect();
	musica=new Music(2);
	al_reserve_samples(100);
	blocco1 = new Blocco(200, 130, bloccoPietra);
	blocco2 = new Blocco(292, 170, bloccoPietra);
	blocco3 = new Blocco(380, 130, bloccoPietra);
	player->removeBubble();
	player->setImmuneTime(0);
	regolaPalle();
	al_flush_event_queue(event_queue);
	musica->Play();
	al_start_timer(timer);
	sound->Play("showtime");
	Transition(1,punteggio);
	player->setY(PLAYER_ALT_NORM);
	
	//IL GIOCO VERO E PROPRIO
	while(!MatchOver) {
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if(ev.type == ALLEGRO_EVENT_TIMER)  {
			hit = false;
	
			if(shoot)
				hit = GP->hitByHook(player, spawnY);
			else
				player->posizionaArma();	

			//RAMPINO HA COLPITO PALLA
			if(hit && !presa)  {
				sound->Play("ball");	
				punteggio+=200;
				presa=true;
				drawExplosion=true;
				if(powerup->canSpawn())  {
					powerup->Spawn(player->getArmaX(), spawnY);
				}
			}

			if(powerup->Spawned())  {
				if(piat1->powerUpHere(powerup) || piat2->powerUpHere(powerup))  {
					if(powerup->notArrivedTerrain(platform))
						powerup->Fall();
				}
				else if(powerup->notArrivedTerrain(terrain))
					powerup->Fall();

				int effect = powerup->playerTookIt(player);
				if(effect == 0)
				{
					sound->Play("powerUp1");
					player->activeBubble();
				}	
				if(effect == 1){
					sound->Play("powerUp2");
					timeEffect = 300;
				}	
			}

			if(timeEffect <= 0)  {
				if(blocco1)	GP->bouncerBlocco(blocco1);
				if(blocco2)	GP->bouncerBlocco(blocco2);
				if(blocco3) GP->bouncerBlocco(blocco3);
				GP->bouncerPiattaforma(piat1);
				GP->bouncerPiattaforma(piat2);
				GP->Bouncer();

				//IF PALLA COLPISCE PLAYER
				p_hit = GP->playerHit(player, player->Bubble());
				if(p_hit && player->Bubble())  {
					p_hit = false;
					player->removeBubble();
					player->setImmuneTime(60);
				}
				
				//IF FUOCO COLPISCE PLAYER
				p_hitFire=drago->hitFire(player);
				if(p_hitFire && player->Bubble())  {
					player->removeBubble();
					player->setImmuneTime(60);
					p_hitFire = false;
				}
			}

			if(!hit)
				presa=false;

			climbing =  player->getY() != PLAYER_ALT_NORM && player->getY() != PLAYER_ALT_PIAT;

			//CONTROLLO MOVIMENTI DX/SX GIOCATORE
			if(keyRight && !caduto && !drawShoot && !climbing)  {
				player->setFrames(6);
				drawShoot=false;
				toLeft=false;
				if(player->getY() == PLAYER_ALT_NORM)
					player->muoviSx(false, SCREEN_W);
				else if(piat1->playerHere(player))
					player->muoviSx(false, piat1->getX()+piat1->getDimX());
				else if(piat2->playerHere(player))
					player->muoviSx(false, piat2->getX()+piat2->getDimX());
			}
			if(keyLeft && !caduto && !drawShoot && !climbing)  {
				player->setFrames(6);
				drawShoot=false;
				toLeft=true;
				if(player->getY() == PLAYER_ALT_NORM)
					player->muoviSx(true, 0);
				else if(piat1->playerHere(player))
					player->muoviSx(true, piat1->getX());
				else if(piat2->playerHere(player))
					player->muoviSx(true, piat2->getX());
			}
			if(keySpace && !caduto && !climbing)  {
				if(!shoot)
					drawShoot=true;
				shoot=true;
				keySpace=false;
			}

			if(p_hit && !colpito && !caduto && !player->Immune())  {
				sound->Play("hit");
				return_value = VITAPERSA;
				caduto=true;
				colpito=true;
			}

			if(!dragonArrive){
				if(p_hitFire && !colpito && !caduto && !player->Immune())  {
					sound->Play("hit");
					return_value = VITAPERSA;
					caduto=true;
					colpito=true;
				}
				if(!p_hitFire && !p_hit)
					colpito=false;
			}

			if(player->Immune())
				player->decreaseImmune();

			if(keyUp && player->getY() > PLAYER_ALT_PIAT)  {
				climbing = true;
				if(scala1->playerHere(player))
					player->setX(scala1->getX());
				else if(scala2->playerHere(player))
					player->setX(scala2->getX());
				else
					climbing = false;

				if(climbing)
					player->muoviUp(true, PLAYER_ALT_PIAT);
			}
			else if(keyDown && player->getY() < PLAYER_ALT_NORM)  {
				climbing = true;
				if(scala1->playerHere(player))
					player->setX(scala1->getX());
				else if(scala2->playerHere(player))
					player->setX(scala2->getX());
				else
					climbing = false;

				if(climbing)
					player->muoviUp(false, PLAYER_ALT_NORM);
			}

			if(player->getY() > piat1->getY())
				hook_colp = piat1->hitByHook(player) || piat2->hitByHook(player);

			if(blocco1)  {
				if(blocco1->hitByHook(player))  {
					sound->Play("brick");
					hook_colp = true;
					blocco1->setExploded(true);
					if(powerup->canSpawn())  {
						powerup->Spawn(player->getArmaX(), blocco1->getY());
					}
				}
			}
			if(blocco2)  {
				if(blocco2->hitByHook(player))  {
					sound->Play("brick");
					hook_colp = true;
					blocco2->setExploded(true);
					if(powerup->canSpawn())  {
						powerup->Spawn(player->getArmaX(), blocco2->getY());
					}
				}
			}
			if(blocco3)  {
				if(blocco3->hitByHook(player))  {
					sound->Play("brick");
					hook_colp = true;
					blocco3->setExploded(true);
					if(powerup->canSpawn())  {
						powerup->Spawn(player->getArmaX(), blocco3->getY());
					}	
				}
			}
			
			if(shoot && player->getArmaY()>0 && !presa && !hook_colp)  {
				player->setArmaY(player->getArmaY() - 6);
				H_arma += 6;
			}
			else  {
				H_arma = 0;
				shoot=false;
			}

			if(next[3])  {
				return_value = LIVELLOSUP;
				break;
			}
			if(!dragonArrive && timeEffect <= 0){
				if((fireCount==0 || (fireCount==150 && player->getX()>=470)) && !caduto)  {
					//sound->Play("dragon1");
					spitting=true;
					fireCount=300;
				}	
				fireCount--;
			}
			
			if(spitting)
				fire=true;
			if(caduto)
				fire=false;

			redraw = true;
		}

		else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			return_value = EXIT;
			break;
		}

		if(ev.type == ALLEGRO_EVENT_KEY_DOWN)  {
			if(ev.keyboard.keycode==ALLEGRO_KEY_ESCAPE)  {
				al_stop_timer(timer);
				keyLeft = false;
				keyRight = false;
				if(!Pausa(fullscreen, res_info))  {	
					return_value = MENU;
					break;
				}
				al_start_timer(timer);
			}
			if(ev.keyboard.keycode == ALLEGRO_KEY_N)
				next[0] = true;
			if(ev.keyboard.keycode == ALLEGRO_KEY_E)  {
				if(next[0])
					next[1] = true;
			}
			if(ev.keyboard.keycode == ALLEGRO_KEY_X)  {
				if(next[1])
					next[2] = true;
			}
			if(ev.keyboard.keycode == ALLEGRO_KEY_T)  {
				if(next[2])
					next[3] = true;
			}
			if(ev.keyboard.keycode==ALLEGRO_KEY_SPACE)
				keySpace=true;
			
			if(ev.keyboard.keycode==ALLEGRO_KEY_RIGHT)
				keyRight=true;
			else if(ev.keyboard.keycode==ALLEGRO_KEY_LEFT)
				keyLeft=true;

			if(ev.keyboard.keycode==ALLEGRO_KEY_UP){
				keyUp = true;
				keyUpDown=true;
			}	
			else if(ev.keyboard.keycode==ALLEGRO_KEY_DOWN)  {
				keyDown = true;
				keyUpDown=true;
			}	
			if(ev.keyboard.keycode==ALLEGRO_KEY_F)
				toggleFullscreen(fullscreen, res_info);
			if(ev.keyboard.keycode==ALLEGRO_KEY_M)
				{
						musica->Mute();
						sound->Mute();
						//sound->Play("dragonloop");
				}		
		}
		else if(ev.type==ALLEGRO_EVENT_KEY_UP)  {
			if(ev.keyboard.keycode==ALLEGRO_KEY_RIGHT)
				keyRight=false;
			else if(ev.keyboard.keycode==ALLEGRO_KEY_LEFT)
				keyLeft=false;

			if(ev.keyboard.keycode==ALLEGRO_KEY_UP)  {
				keyUp = false;
				keyUpDown=false;
			}	
			else if(ev.keyboard.keycode==ALLEGRO_KEY_DOWN)  {
					keyDown = false;
					keyUpDown= false;
			}	
		}

		if(redraw && al_is_event_queue_empty(event_queue)) {
			player->setDraw(keyLeft,keyRight,drawShoot,toLeft, caduto, climbing,keyUpDown);
			Draw(vite, punteggio, tempo, H_arma);

			if(!drago->DrawFire(colpito,fire, timeEffect > 0))
				fire=false;

			if(tempo/60<=145)  {
				if(!dragonSound)  {
					//sound->Play("dragonloop");
					dragonSound=true;
				}	
				if(drago->getX()<=530)  {
					dragonArrive=false;
				}
				
				if(!drago->Draw(dragonArrive,spitting,timeEffect<=0))  {
					spitting=false;
				}
			}	

			al_flip_display();
			if(timeEffect > 0)
				timeEffect--;
			else
				tempo--;

			redraw = false;

			//CONTROLLO VITTORIA
			if(GP->Empty())  {
				
				sound->Play("excellent");
				sound->Play("applause");
				Transition(2,punteggio);
				al_flush_event_queue(event_queue);
				while(true)  {
					al_wait_for_event(event_queue, &ev);
					if(ev.type == ALLEGRO_EVENT_KEY_DOWN)  {
						Transition(3,punteggio);
						al_rest(2);
						break;
					}
				}
				return_value = LIVELLOSUP;
				break;
			}


		if(vite==1 && return_value==VITAPERSA){
			al_flush_event_queue(event_queue);
			musica->Stop();
			sound->Play("gameOverMusic");
			sound->Play("gameOver");
			Transition(6,punteggio);
				while(true)  {
					al_wait_for_event(event_queue, &ev);
					if(ev.type == ALLEGRO_EVENT_KEY_DOWN)  {
						break;
					}
				}
			break;	
		}
		
		}			
	}
	
	//DISTRUGGO TUTTO
	if(blocco1)  {
		delete blocco1;
	}
	if(blocco2)  {
		delete blocco2;
	}
	if(blocco3)  {
		delete blocco3;
	}

	player->setX(SCREEN_W/2 - player->getDimX());
   	player->setY(SCREEN_H/1.37 - player->getDimY());
	GP->Clear();
	musica->Stop();
	al_stop_samples();
	powerup->Destroy();
	return return_value;
}

void Livello2::Draw(int vite, int punteggio, int tempo, int H_arma)  {
	al_draw_bitmap(sfondo,0,0,0);
			
	if(vite>=1)
		al_draw_bitmap(vite_bmp, SCREEN_W/25, SCREEN_H/11, 0);
	if(vite>=2)
		al_draw_bitmap(vite_bmp, SCREEN_W/11, SCREEN_H/11, 0);
	if(vite>=3)
		al_draw_bitmap(vite_bmp, SCREEN_W/7, SCREEN_H/11, 0);
	if(vite<=0 || tempo<=0)
		caduto=true;

	al_draw_textf(font1,al_map_rgb(255,255,0),SCREEN_W/4.7,SCREEN_H/1.16,ALLEGRO_ALIGN_RIGHT,"%d",tempo/60);
	al_draw_textf(font2,al_map_rgb(0,0,255),SCREEN_W/1.06,SCREEN_H/1.14,ALLEGRO_ALIGN_RIGHT,"%d",punteggio);

	if(blocco1)  {
		if(!blocco1->Draw())  {
			delete blocco1;
			blocco1=NULL;
		}
	}
	if(blocco2)  {
		if(!blocco2->Draw())  {
			delete blocco2;
			blocco2=NULL;
		}	
	}
	if(blocco3)  {
		if(!blocco3->Draw())  {
			delete blocco3;
			blocco3=NULL;
		}	
	}

	piat1->Draw();
	piat2->Draw();
	scala1->Draw();
	scala2->Draw();

	if(shoot)
		player->ArmaDraw(H_arma);

	if(powerup->Spawned())
		powerup->Draw();

	if(!player->Draw())  {
		if(caduto)  {
			caduto=false;
			MatchOver=true;
			if(punteggio>300)
				punteggio-=300;
			else
				punteggio=0;
		}	
		drawShoot=false;
	}
			
	if(!GP->Draw(drawExplosion))
		drawExplosion=false;
}