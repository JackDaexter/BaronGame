#include <stdio.h>
#include <MLV/MLV_all.h>



/* dimension du monde en nombre de cases*/
#define LONG 18
#define LARG 12
/* l’origine est en haut a gauche*/

/* les deux clans */
#define ROUGE 'R'
#define BLEU  'B'

/* les genres d’agents */
#define MANANT 'm'
#define BARON  'b'
#define GUERRIER  'g'
#define CHATEAU 'c'

/* les temps de production */
#define TMANANT 2
#define TGUERRIER 4
#define TBARON 6

/* les couts */
#define CMANANT 1
#define CGUERRIER 5
#define CBARON 10
#define CCHATEAU 30

typedef struct agent{
	char couleur; /* ROUGE ou BLEU */
	char genre; /* BARON, MANANT, GUERRIER, CHATEAU */
	int posx, posy; /* position actuelle */
	int destx, desty; /* destination (negatif si immobile) */
	char produit; /* production actuelle d’un chateau */
	int temps; /* tours restant pour cette production */
	struct agent *asuiv, *aprec; /* liste des agents liees a un chateau */
	struct agent *vsuiv, *vprec; /* liste des voisins */
}Agent,*AListe;

typedef struct{
	Agent *chateau; /* s’il y a un chateau sur la case */
	AListe habitant; /* les autres occupants */
}Case;

typedef struct{
	Case plateau[LONG][LARG];
	AListe rouge, bleu;
	int tour; /* Numero du tour */
	int tresorRouge, tresorBleu ;
}Monde;



/* Cette fonction sera appelée pour la création de agents tout simplement*/
AListe creation(char genre,char couleur,int posx,int posy){
	
	Agent * tmp;
	
	tmp = malloc(sizeof(Agent));
	
	if (tmp == NULL){
		
	
		return 0;
	}
	
	/* Donc cette première case vaut chateau, on l'initialise à une position */
	tmp->genre = genre;
	tmp->couleur = couleur;
	tmp->posx = posx;
	tmp->posy = posy;
	tmp->temps = -1;
	tmp->destx = -1;
	tmp->desty = -1;
	tmp->asuiv = NULL;
	tmp->aprec = NULL;
	tmp->vsuiv = NULL;
	tmp->vprec = NULL;
	tmp->produit = '/';
	
	//(*liste) = tmp;
	return tmp;

}

/* Cette fonction sert à vérifier que la case ou l'on veut placer l'agent est vide
 * Si c'est le cas elle le place sinon elle ne fait rien */
 
int execution(Monde *world,char produit,Agent *liste,int tresor,char couleur,int posx,int posy){
	
	Agent *tmp;
	Agent *creationAgent = NULL;
	
	int tours = 0;
	
	tmp = liste;
	
	int save1 = posx, save2 = posy;
	
	/* Je vais expliquer le fonctionnement pour le baron qui est exactement le m^eme que pour tous les autres*/
	
	if(produit == BARON){
			/*Si le produit que le chateau veut créer est un baron donc ici*/
			while(liste != NULL){
					/* On se place préalaplement en vin de liste pour pouvoir placer l'agent créer à la fin */
					if (liste->asuiv == NULL){
					
					// test --------
						while(world->plateau[posx][posy].chateau != NULL || world->plateau[posx][posy].habitant != NULL){
							
							/*Ici comme on vérifie les 8 case autours du chateau on met un compteur */
							tours ++;
							
							posx = save1, posy = save2;// On se replace sur la case du chateau
							
							if(tours == 1 && posx-1 >= 0){ // On vérifier la case à l'OUEST du chateau
								posx -= 1;
								
							}
							
							if(tours == 2 && (posx+1 <LARG-1 && posy-1 >= 0)){ // NORD 
								//On vérifie la case au dessus du chateau;
								posy -= 1;
								
							}
							
							if(tours == 3 && (posx-1 >= 0 || posy-1 >= 0)){ //NORD_OUEST
								//On vérifie la case en au a droite;
								
								posx -= 1;
								posy -= 1;
								
							}
							
							if(tours == 4 && (posx+1 < LARG-1) ){		//SUD
								
								posx += 1;
						
							}
							if(tours == 5 && (posx+1 < LARG-1) ){		//EST
								// On vérifie la case de Droite 
								posx += 1;
							
							}
							
							if(tours == 6){ //SUD_EST
								// One replace le point x sur la case du chateau
								//posx -= 1;
	
								//On vérifie la case en desous a droite du chateau
								posx += 1;
								posy += 1;
								
							}
							
							if(tours == 7 && (posy+1 < LONG-1 && posx-1 >= 0) ){ //NORD_EST
								//On vérifie la case en haut a droite du chateau
								posx -= 1;
								posy += 1;
								
							}
					
							if(tours == 8 && (posy+1 < LONG-1 && posx-1 >= 0) ){ // SUD_OUEST
					
								//On vérifie la case en bas a gauche du chateau
								posx += 1;
								posy -= 1;
							}
							
							/* Si il n'ya aucun case disponible autour du chateau, on sort de la fonction */
							if(tours > 8){
								
								return 0;
							}
							
							
						}
						/* Ici on créer l'agent et on éfféectue les liaison avec la case précédent de la liste chainée de son équipe */
						creationAgent = creation(BARON,couleur,posx,posy);
						creationAgent->aprec = liste;
						liste->asuiv = creationAgent;
						liste = liste->asuiv;
						world->plateau[posx][posy].habitant = liste;
					
					}
					liste = liste->asuiv;
			}
			liste = tmp;
		}
	
	
	
	
	// ici dans ce cas, on créer un soldat de l'hiver déter
	else if(produit == GUERRIER){
	
			while(liste != NULL){
				
				if (liste->asuiv == NULL){
				
				while(world->plateau[posx][posy].chateau != NULL || world->plateau[posx][posy].habitant != NULL){
						tours ++;
							posx = save1, posy = save2;// On se replace sur la case du chateau
							
							if(tours == 1 && posx-1 >= 0){ // OUEST
								posx -= 1;
								
							}
							
							if(tours == 2 && (posx+1 <LARG-1 && posy-1 >= 0)){ // NORD
								//On vérifie la case au dessus du chateau;
								posy -= 1;
								
							}
							
							if(tours == 3 && (posx-1 >= 0 || posy-1 >= 0)){ //NORD_OUEST
								//On vérifie la case en au a droite;
								
								posx -= 1;
								posy -= 1;
								
							}
							
							if(tours == 4 && (posx+1 < LARG-1) ){		//SUD
								
								posx += 1;
						
							}
							if(tours == 5 && (posx+1 < LARG-1) ){		//EST
								// On vérifie la case de Droite 
								posx += 1;
							
							}
							
							if(tours == 6){ //SUD_EST
								// One replace le point x sur la case du chateau
								//posx -= 1;
	
								//On vérifie la case en desous a droite du chateau
								posx += 1;
								posy += 1;
								
							}
							
							if(tours == 7 && (posy+1 < LONG-1 && posx-1 >= 0) ){ //NORD_EST
								//On vérifie la case en haut a droite du chateau
								posx -= 1;
								posy += 1;
								
							}
					
							if(tours == 8 && (posy+1 < LONG-1 && posx-1 >= 0) ){ // SUD_OUEST
					
								//On vérifie la case en bas a gauche du chateau
								posx += 1;
								posy -= 1;
							}
							
						
							if(tours > 8){
								
								return 0;
							}
							
							
							
							if(tours > 8){
								MLV_draw_text(890, 500, "Plus de place pour placer le joueur",MLV_COLOR_WHITE);
								return 0;
							}
						}
					creationAgent = creation(GUERRIER,couleur,posx,posy);
					creationAgent->aprec = liste;
					liste->asuiv = creationAgent;
					liste = liste->asuiv;
					world->plateau[posx][posy].habitant = liste;
					
				}
				liste = liste->asuiv;
			}
			
			liste = tmp;
		}
	
	
	
	else if(produit == MANANT){
	
			while(liste != NULL){
					
					if (liste->asuiv == NULL){
					
					// test --------
						while((world->plateau[posx][posy].chateau != NULL || world->plateau[posx][posy].habitant != NULL) || (posx < 0 ||  posx > LARG-1 || posy < 0 || posx > LONG-1)){
							
							tours ++;
							posx = save1, posy = save2;// On se replace sur la case du chateau
							
							if(tours == 1 && posx-1 >= 0){ // OUEST
								posx -= 1;
								
							}
							
							if(tours == 2 && (posx+1 <LARG-1 && posy-1 >= 0)){ // NORD
								//On vérifie la case au dessus du chateau;
								posy -= 1;
								
							}
							
							if(tours == 3 && (posx-1 >= 0 || posy-1 >= 0)){ //NORD_OUEST
								//On vérifie la case en au a droite;
								
								posx -= 1;
								posy -= 1;
								
							}
							
							if(tours == 4 && (posx+1 < LARG-1) ){		//SUD
								
								posx += 1;
						
							}
							if(tours == 5 && (posx+1 < LARG-1) ){		//EST
								// On vérifie la case de Droite 
								posx += 1;
							
							}
							
							if(tours == 6){ //SUD_EST
								// One replace le point x sur la case du chateau
								//posx -= 1;
	
								//On vérifie la case en desous a droite du chateau
								posx += 1;
								posy += 1;
								
							}
							
							if(tours == 7 && (posy+1 < LONG-1 && posx-1 >= 0) ){ //NORD_EST
								//On vérifie la case en haut a droite du chateau
								posx -= 1;
								posy += 1;
								
							}
					
							if(tours == 8 && (posy+1 < LONG-1 && posx-1 >= 0) ){ // SUD_OUEST
					
								//On vérifie la case en bas a gauche du chateau
								posx += 1;
								posy -= 1;
							}
							
						
							if(tours > 8){
								
								return 0;
							}
						
							if((posx > LARG - 1 && posy > LONG- 1) || (posx < 0 && posy < 0) || (posx < 0 && posy > LONG- 1) || (posx > LARG -1 && posy < 0)){
								MLV_draw_text(1290, 500, "Plus de place pour placer l'agent",MLV_COLOR_WHITE);
								MLV_wait_seconds(2);
								printf("pas la place");
								return 0;
							}
					
						}

						creationAgent = creation(MANANT,couleur,posx,posy);
						creationAgent->aprec = liste;
						liste->asuiv = creationAgent;
						liste = liste->asuiv;
						world->plateau[posx][posy].habitant = liste;
					
					}
					liste = liste->asuiv;
				}
				liste = tmp;
	}
	
	return 9;
}

/* Cette fonction permet de faire les calcul pour les déplacement s des agents mobiles */
int listeDeplacement(Agent *liste){
		
	int x,y;
	
	int positionx = 0,positiony = 0;
	
	/*Je prend ces valeur la car ce sont celle de la première case 
	 * cela me servira donc de repère en quelque sorte*/
	int compteur1 = 66, compteur2 = 86;
	
	MLV_actualise_window();
	
	MLV_draw_text(1180, 400, "Cliquez sur une case pour commencer le déplacement",MLV_COLOR_ORANGE);
	
	MLV_actualise_window();
	
	int commence = 0,ok = -1;
	
	/* Je lance une bouqle qui ne se terminera que lorsque l'utilisateur aura choisit une case valide */
	while (commence == 0){
		
		MLV_wait_mouse(&x,&y);
		
		while (ok == -1){
			
			positionx = 0,positiony = 0;
			compteur1 = 66, compteur2 = 86;
			while (compteur1 < x || compteur2 < y){
				
				
				if(compteur1 < x){
					positiony += 1;
				}
				
				if(compteur2 < y){
					positionx += 1;
				}
				compteur1 += 60;
				compteur2 += 60;
				
				
			}
			
			
			/* Si le joueur choisit la meme case que celle ou il se trouve , il doit en choisir une autre */
			if(positionx == liste->posx && positiony == liste->posy){
				MLV_draw_text(1280, 340, "Vous etes sur cette case",MLV_COLOR_RED);
				MLV_actualise_window();
				MLV_wait_mouse(&x,&y);
			}
			/* Si il choisit une case hors plateau, il ya une erreur aussi et il doit de nouveau choisir une case */
			else if(positionx > 11 || positiony > 17){
				MLV_draw_text(1280, 330, "Case invalide",MLV_COLOR_RED);
				MLV_actualise_window();
				MLV_wait_mouse(&x,&y);
			}
			else{
				ok = 1;
			}
			
		}
		
		liste->destx = positionx;
		liste->desty = positiony;
		
			
		commence= 1;
	}
	MLV_actualise_window();
	
	return 1;
}

/* Cette fonction permet à un baron ou à un guerrer d'attaquer les ennemis qui se trouve sur la meme 
 * case que la leur alors qu'ils ne sont pas en mouvements */
 
int detectionEnnemiProche(Agent *tmp,Agent *cible){
	
	Agent *pointeTete = cible; /* Elle pointe sur la liste chainé de l'équipe adverse : Rouge Ou Bleu */
	
	Agent *pointeCible = pointeTete;  /* Elle pointe sur la liste chainé de l'équipe adverse : Rouge Ou Bleu */
	
	if(tmp->genre == BARON || tmp->genre == GUERRIER){
				while (pointeCible != NULL){
				
				if(tmp->posy == pointeCible->posy && tmp->posx == pointeCible->posx){
						
						/* J'ai choisit d'utiliser produit pour les barons et guerrier comme vérification de possibilité d'attaque puisse qu'ile
						 * ne sert a rien autre pour eux et cela m'évite de faire des vérification inutile */
						 
						tmp->produit = 'c';
						return 0;
				}
				pointeCible = pointeCible->asuiv;
				
				/* On parcours toute la liste chainée ainsi que les élément qui se trouve sur d'autres chateaux
				 * créer par des barons */
				 
				if(pointeTete->vsuiv != NULL && pointeCible == NULL){ 
					pointeTete = pointeTete->vsuiv;
					pointeCible = pointeTete;
				}
		}	
	}
	/* Si il n'ya pas d'ennemi et que l'agent est immobile, on met 'r' pour indiquer qu'il ny'a pas d'ennemi a l'horizon */
	if(tmp->destx == -1 && tmp->desty == -1){
		tmp->produit = 'r';
	}
	return 1;
}


/* Cette fonction est uniquement utiliser pour détecter les ennemi qui se trouve sur la case suivante ou je dirige notre baron ou guerrier
 * Elle permettra de choisir entre attaquer ou ne rien faire */
 
int detection(Agent *tmp,Agent *cible){
	
	Agent *pointeTete = cible;  /* Elle pointe sur la liste chainé de l'équipe adverse : Rouge Ou Bleu */
	Agent *pointeCible = pointeTete; /* On pointe sur l'élément précédent */
	
	
	if(tmp->genre == BARON || tmp->genre == GUERRIER){
				
			while (pointeCible != NULL){
				
				if((tmp->posx < tmp->destx || tmp->posx > tmp->destx) && tmp->destx != -1){
					
					/* % a refaire %*/
					if((tmp->posx+1 == pointeCible->posx || tmp->posx-1 == pointeCible->posx) && tmp->posy == pointeCible->posy) {
						
						tmp->produit = 'c';
						return 0;
					}
				}
			
				if((tmp->posy < tmp->desty ||tmp->posy > tmp->desty)){
				
								
					if((tmp->posy+1 == pointeCible->posy || tmp->posy-1 == pointeCible->posy) && tmp->posx == pointeCible->posx){
							
							tmp->produit = 'c';

							MLV_actualise_window();
							return 0;
							
						}
					}
					pointeCible = pointeCible->asuiv;
						
					if(pointeTete->vsuiv != NULL && pointeCible == NULL){
						pointeTete = pointeTete->vsuiv;
						pointeCible = pointeTete;
					}
			}
	}
	tmp->produit = 'r';
	return 1;
	
}

/* Ici on va gérer le déplacement de nos agents */

int seDeplace(Monde *world,Agent *tmp,Agent *cible,Agent *camp,int siAttaque){
	
	/*Agent *SurCase, *PointeTete;*/
	
	/*PointeTete = camp; On va pointer sur la liste de notre Equipe : Rouge ou bleu selon le tour de jeu 
	SurCase = PointeTete;  On pointe sur le pointeur précédent */
	
	MLV_actualise_window();
	
	int blocage = 0; /* Servira à éviter d'entrer dans les if précédent */
	
	/* ON DESCEND DE +1 */
	if(tmp->posx < tmp->destx && blocage < 10){
		
		/* Si le plateau pointait sur cet agent alors on cherche un autre agent sur cette case et on pointera sur lui
		 * Cette boucle devait permettre aussi de gérer le déplacement d'agent et de pointer sur le nouvelle arrivant
		 * pour a nouveu les lier entre par une liste vsuiv */
		
		/*if(world->plateau[tmp->posx][tmp->posy].habitant == tmp){
				while(SurCase != NULL){
					 if(SurCase->posx == tmp->posx && SurCase->posy == tmp->posy && SurCase->genre != CHATEAU){
						 world->plateau[tmp->posx][tmp->posy].habitant = SurCase;
						 
					 }
					 SurCase = SurCase->asuiv;
					 
					 if(SurCase == NULL && PointeTete->vsuiv != NULL){
						 PointeTete = PointeTete->vsuiv;
						 SurCase = SurCase->asuiv;
					 }
				}
		}*/
		
		/* Si il n ' ya aucun autre agent sur cette case alors la case pointe sur NULL */
		
		
		
		world->plateau[tmp->posx][tmp->posy].habitant = NULL;
		
		tmp->posx += 1;
		detection(tmp,cible);
		
		world->plateau[tmp->posx][tmp->posy].habitant = tmp;
		
		
		
		
			
		blocage = 10;
		
	}
	/* ON MONTE DE +1 */
	else if(tmp->posx > tmp->destx && blocage < 10){
		
		/* Vous verrez ce type de code mis en commentaire un peu partout dans le programme
		 * il devait permettre de relier tous les éléménts d'une m^eme cae entre eux
		 * mes en avancant dans le programmes des erreurs sont apparus et je n'est pas trouvé comment
		 * les résoudre. Je les ai laissé pour vous montrer que j'avais tout de meme
		 * compris le principe */
		
		/*if(world->plateau[tmp->posx][tmp->posy].habitant == tmp){
				while(SurCase != NULL){
					 if(SurCase->posx == tmp->posx && SurCase->posy == tmp->posy && SurCase->genre != CHATEAU){
						 world->plateau[tmp->posx][tmp->posy].habitant = SurCase;
						 
					 }
					 SurCase = SurCase->asuiv;
					 
					 if(SurCase == NULL && PointeTete->vsuiv != NULL){
						 PointeTete = PointeTete->vsuiv;
						 SurCase = SurCase->asuiv;
					 }
				}
		}*/
		
		
		world->plateau[tmp->posx][tmp->posy].habitant = NULL;
		
		
		tmp->posx -= 1;
		detection(tmp,cible);
		
		world->plateau[tmp->posx][tmp->posy].habitant = tmp;
		blocage = 10;
		
	}
	
	/* ON VA A DROITE */
	else if(tmp->posy < tmp->desty && blocage < 10){
		
		/*if(world->plateau[tmp->posx][tmp->posy].habitant == tmp){
				while(SurCase != NULL){
					 if(SurCase->posx == tmp->posx && SurCase->posy == tmp->posy && SurCase->genre != CHATEAU){
						 world->plateau[tmp->posx][tmp->posy].habitant = SurCase;
						 
					 }
					 SurCase = SurCase->asuiv;
					 
					 if(SurCase == NULL && PointeTete->vsuiv != NULL){
						 PointeTete = PointeTete->vsuiv;
						 SurCase = SurCase->asuiv;
					 }
				}
		} */
		
		
		world->plateau[tmp->posx][tmp->posy].habitant = NULL;
		
		
		tmp->posy += 1;
		detection(tmp,cible);
		
		world->plateau[tmp->posx][tmp->posy].habitant = tmp;
		
		blocage = 10;
	}
	/* ON VA A GAUCHE */
	else if(tmp->posy > tmp->desty && blocage < 10){
		
		/*if(world->plateau[tmp->posx][tmp->posy].habitant == tmp){
				while(SurCase != NULL){
					 if(SurCase->posx == tmp->posx && SurCase->posy == tmp->posy && SurCase->genre != CHATEAU){
						 world->plateau[tmp->posx][tmp->posy].habitant = SurCase;
						 
					 }
					 SurCase = SurCase->asuiv;
					 
					 if(SurCase == NULL && PointeTete->vsuiv != NULL){
						 PointeTete = PointeTete->vsuiv;
						 SurCase = SurCase->asuiv;
					 }
				}
		} */
		
		world->plateau[tmp->posx][tmp->posy].habitant = NULL;
		
		
		tmp->posy -= 1;
		detection(tmp,cible);
		
		world->plateau[tmp->posx][tmp->posy].habitant = tmp;
		
		blocage = 10;
	}
	
	/* Si notre agent est arrivé a destination , on le rend immobile */
	if(tmp->posx == tmp->destx && tmp->posy == tmp ->desty){
		tmp->destx = -1;
		tmp->desty = -1;
		return -1;
	}
	return -1;
}

/* Je n'utilise plus cette fonction mais je la laisse car c'est entre autre a cause de celle ci
 * que le reste du code est assez compliqué a décripter.
 * Cette contion devait servir a DETUIRE un chateau et ses agent, si jamais le chateau se fesait détruire
 * Mais bizarrement, on ne m'autaurise pas a détruire les chateau mais UNIQUEMENT les chateaux mais 
 * je peux supprimer tous les autres agents sans problème*/
void plusDeRoyaume(Monde *world,Agent *Ennemi,Agent *TeamAttaquant){
	
	Agent *pointe1;
	
	Agent *pointeA;
	
	pointeA = TeamAttaquant;
	
	pointe1 = Ennemi;
	
	pointe1->vprec = NULL;
	
	while(pointe1->genre != CHATEAU){
		pointe1 = pointe1->aprec;
	}
	pointeA = pointe1;
	
	pointeA = pointeA->vprec;
	pointeA->vsuiv =NULL;
	
	
	
}
/* -------------------------------------------------------*/

/*Cette fonction est TRES LONGUE du faut que l'on doivent gérer l'attaque selon le guerrier ou le baron 
 * mais elle est assez récursive et pas super compluquer a comprendre
 * Elle sert a Gerer l'attaque et donc à gérer les combats  */
 
int gestionAttaque(Monde *world,Agent *tmp,Agent *cible,Agent *pointe){
	

	int nombreElementSurCase = 0; /* Cette variable sert à compter le nombre d'ennemi à éliminer */
	
	/* je lance 2 dée plutot que un seul à cause des combats GUERRIER vs GUERRIER ou MANATS vs MANANTS */
	int lanceDee = 0, lanceDee2 = 0; 
	
	
	Agent *pointeTete = cible; /* On pointe sur la liste des agents de l'équipe adverse Rouge ou Bleu en fonction de l'équipe que l'on joue */
	Agent *pointeCible = pointeTete;/* On pointe sur lélément précedent*/
	
	Agent *decolle;/*Servira à décoller les cadavre  (les agents ou chateaux morts) du reste */
	
	int soldat = 0;
	int baron = 0;
	int manants = 0;
	int inverse = 0;
	
	/* On va ici tout simplement parcourir l'ensemble des Ennemis et comparer avec l'agent qui arrive sur la case*/
	while (pointeCible != NULL){
		
		if((tmp->posx < tmp->destx || tmp->posx > tmp->destx) && tmp->destx != -1 && tmp->desty != -1){
			
			if( tmp->posx+1 == pointeCible->posx || tmp->posx-1 == pointeCible->posx){
				
				nombreElementSurCase++;
				
				if(pointeCible->genre == GUERRIER){
					soldat += 1;
				}
				else if(pointeCible->genre == BARON){
					baron += 1;
				}
				else if(pointeCible->genre == MANANT){
				//	manants += 1;
				}
			}
		}
		
		else if((tmp->posy < tmp->desty ||tmp->posy > tmp->desty)  && tmp->posx == tmp->destx){
			
			if(tmp->posy+1 == pointeCible->posy || tmp->posy-1 == pointeCible->posy){
			
				nombreElementSurCase++;
				
				if(pointeCible->genre == GUERRIER){
					soldat += 1;
				}
				else if(pointeCible->genre == BARON){
					baron += 1;
				}
				else if(pointeCible->genre == MANANT){
					manants += 1;
				}
			
			}
		}
		
		else if(tmp->destx == -1 && tmp->desty == -1){
			
			if(tmp->posy == pointeCible->posy && tmp->posx == pointeCible->posx){
				
				nombreElementSurCase++;
				
				if(pointeCible->genre == GUERRIER){
					soldat += 1;
				}
				else if(pointeCible->genre == BARON){
					baron += 1;
					
				}
				else if(pointeCible->genre == MANANT){
					manants += 1;
				}
			
			}
		}
		
		
		pointeCible = pointeCible->asuiv;
		
		if(pointeTete->vsuiv != NULL && pointeCible == NULL){
			pointeTete = pointeTete->vsuiv;
			pointeCible = pointeTete;
		}
	}
	
	pointeTete = cible;
	pointeCible = pointeTete;
	
	
	
	/* Je précise que l'ordre est fait exprès, pour éviter que l'on attaque 
	 * un chateau en premier si il y'a d'autres agents sur la case */
	while(nombreElementSurCase > 0){
		
			while (pointeCible != NULL && nombreElementSurCase > 0){
				/*ICI ON vérifie uniquement les ennemie sur la meme case que le concerné 
				 * et si notre agent est immobile*/
				
				if(tmp->destx == -1 ||tmp->desty == -1){
					
					if(tmp->posx == pointeCible->posx && tmp->posy == pointeCible->posy){
						lanceDee = MLV_get_random_integer(0,100);
						lanceDee2 = MLV_get_random_integer(0,100);
						
						if(pointeCible->genre == GUERRIER){	
							
							if(tmp->genre == BARON){
								if(lanceDee * CGUERRIER > lanceDee2 * CBARON){
										soldat -= 1;
										decolle = pointeCible;
										
										world->plateau[decolle->posx][decolle->posy].habitant = NULL;
										
										pointeCible = pointeCible->aprec;
										
										
										if(pointeCible->asuiv->asuiv != NULL){
											pointeCible->asuiv = pointeCible->asuiv->asuiv;
											pointeCible = pointeCible->asuiv;
											pointeCible->aprec = pointeCible->aprec->aprec;
											pointeCible = pointeCible->aprec;
											
										}
										else{
											pointeCible->asuiv = NULL;
										}
										decolle->aprec = NULL;
										decolle->asuiv = NULL;
										
										nombreElementSurCase --;
								}
								else{
										decolle = tmp;
										tmp = tmp->aprec;
										
										world->plateau[tmp->posx][tmp->posy].habitant = NULL;
										
										if(tmp->asuiv->asuiv != NULL){
											tmp->asuiv = pointeCible->asuiv->asuiv;
											tmp = pointeCible->asuiv;
											tmp->aprec = pointeCible->aprec->aprec;
											tmp = pointeCible->aprec;
											
										}
										else{
											tmp->asuiv = NULL;
										}
										decolle->aprec = NULL;
										decolle->asuiv = NULL;
										
										nombreElementSurCase = 0;
								}
								
							}
							
							if(tmp->genre == GUERRIER){
								if(lanceDee * CGUERRIER > lanceDee2 * CGUERRIER){
										soldat -= 1;
										decolle = pointeCible;
										
										world->plateau[decolle->posx][decolle->posy].habitant = NULL;
										
										pointeCible = pointeCible->aprec;
										
										
										if(pointeCible->asuiv->asuiv != NULL){
											pointeCible->asuiv = pointeCible->asuiv->asuiv;
											pointeCible = pointeCible->asuiv;
											pointeCible->aprec = pointeCible->aprec->aprec;
											pointeCible = pointeCible->aprec;
											
										}
										else{
											pointeCible->asuiv = NULL;
										}
										decolle->aprec = NULL;
										decolle->asuiv = NULL;
										
										nombreElementSurCase --;
									}
								else{
										decolle = tmp;
										tmp = tmp->aprec;
										
										world->plateau[tmp->posx][tmp->posy].habitant = NULL;
										
										if(tmp->asuiv->asuiv != NULL){
											tmp->asuiv = pointeCible->asuiv->asuiv;
											tmp = pointeCible->asuiv;
											tmp->aprec = pointeCible->aprec->aprec;
											tmp = pointeCible->aprec;
											
										}
										else{
											tmp->asuiv = NULL;
										}
										decolle->aprec = NULL;
										decolle->asuiv = NULL;
										
										nombreElementSurCase = 0;
								}
								
							}
							//
						}
			else if(pointeCible->genre == BARON){
							//
							if(tmp->genre == BARON){
								if(lanceDee * CBARON > lanceDee2 * CBARON){
										baron -=1;
										decolle = pointeCible;
										pointeCible = pointeCible->aprec;
										
										world->plateau[decolle->posx][decolle->posy].habitant = NULL;
										
										if(pointeCible->asuiv->asuiv != NULL){
											pointeCible->asuiv = pointeCible->asuiv->asuiv;
											pointeCible = pointeCible->asuiv;
											pointeCible->aprec = pointeCible->aprec->aprec;
											pointeCible = pointeCible->aprec;
											
										}
										else{
											pointeCible->asuiv = NULL;
										}
										decolle->aprec = NULL;
										decolle->asuiv = NULL;
										
										nombreElementSurCase --;
								}
							else{
									decolle = tmp;
									tmp = tmp->aprec;
									
									world->plateau[decolle->posx][decolle->posy].habitant = NULL;
										
									if(tmp->asuiv->asuiv != NULL){
										tmp->asuiv = pointeCible->asuiv->asuiv;
										tmp = pointeCible->asuiv;
										tmp->aprec = pointeCible->aprec->aprec;
										tmp = pointeCible->aprec;
										
										}
									else{
										tmp->asuiv = NULL;
										}
									
									decolle->aprec = NULL;
									decolle->asuiv = NULL;
									nombreElementSurCase = 0;
								}
							}
							
						if(tmp->genre == GUERRIER){
								
								if(lanceDee * CGUERRIER > lanceDee2 * CBARON){
										baron -= 1;
										decolle = pointeCible;
										
										world->plateau[decolle->posx][decolle->posy].habitant = NULL;
										
										pointeCible = pointeCible->aprec;
										
										
										if(pointeCible->asuiv->asuiv != NULL){
											pointeCible->asuiv = pointeCible->asuiv->asuiv;
											pointeCible = pointeCible->asuiv;
											pointeCible->aprec = pointeCible->aprec->aprec;
											pointeCible = pointeCible->aprec;
											
										}
										else{
											pointeCible->asuiv = NULL;
										}
										decolle->aprec = NULL;
										decolle->asuiv = NULL;
										
										nombreElementSurCase --;
								}
								else{
										decolle = tmp;
										tmp = tmp->aprec;
										
										world->plateau[tmp->posx][tmp->posy].habitant = NULL;
										
										if(tmp->asuiv->asuiv != NULL){
											tmp->asuiv = pointeCible->asuiv->asuiv;
											tmp = pointeCible->asuiv;
											tmp->aprec = pointeCible->aprec->aprec;
											tmp = pointeCible->aprec;
											
										}
										else{
											tmp->asuiv = NULL;
										}
										decolle->aprec = NULL;
										decolle->asuiv = NULL;
										
										nombreElementSurCase = 0;
								}
								
							}
						}
				else if(pointeCible->genre == MANANT){
							//
					if(tmp->genre == BARON){
								if(lanceDee * CBARON > lanceDee2 * CBARON){
									manants -= 1;
									decolle = pointeCible;
									pointeCible = pointeCible->aprec;
									
									world->plateau[decolle->posx][decolle->posy].habitant = NULL;
									
									if(pointeCible->asuiv->asuiv != NULL){
										pointeCible->asuiv = pointeCible->asuiv->asuiv;
										pointeCible = pointeCible->asuiv;
										pointeCible->aprec = pointeCible->aprec->aprec;
										pointeCible = pointeCible->aprec;
										
									}
									else{
										pointeCible->asuiv = NULL;
									}
									decolle->aprec = NULL;
									decolle->asuiv = NULL;
									
									nombreElementSurCase --;
							}	
							
							else{
								decolle = tmp;
								tmp = tmp->aprec;
								
								world->plateau[decolle->posx][decolle->posy].habitant = NULL;
									
								if(tmp->asuiv->asuiv != NULL){
									tmp->asuiv = pointeCible->asuiv->asuiv;
									tmp = pointeCible->asuiv;
									tmp->aprec = pointeCible->aprec->aprec;
									tmp = pointeCible->aprec;
									
									}
								else{
									tmp->asuiv = NULL;
									}
								
								decolle->aprec = NULL;
								decolle->asuiv = NULL;
								nombreElementSurCase = 0;
								}
							}
							// ATAQQUANT GUERRIER
					if(tmp->genre == GUERRIER){
						
								if(lanceDee * CGUERRIER > lanceDee2 * CMANANT){
										manants -= 1;
										decolle = pointeCible;
										
										if(world->plateau[decolle->posx][decolle->posy].habitant == pointeCible)
											world->plateau[decolle->posx][decolle->posy].habitant = NULL;
										
										pointeCible = pointeCible->aprec;
										
										
										if(pointeCible->asuiv->asuiv != NULL){
											pointeCible->asuiv = pointeCible->asuiv->asuiv;
											pointeCible = pointeCible->asuiv;
											pointeCible->aprec = pointeCible->aprec->aprec;
											pointeCible = pointeCible->aprec;
											
										}
										else{
											pointeCible->asuiv = NULL;
										}
										decolle->aprec = NULL;
										decolle->asuiv = NULL;
										
										nombreElementSurCase --;
								}
								else{
										decolle = tmp;
										tmp = tmp->aprec;
										
										world->plateau[tmp->posx][tmp->posy].habitant = NULL;
										
										if(tmp->asuiv->asuiv != NULL){
											tmp->asuiv = pointeCible->asuiv->asuiv;
											tmp = pointeCible->asuiv;
											tmp->aprec = pointeCible->aprec->aprec;
											tmp = pointeCible->aprec;
											
										}
										else{
											tmp->asuiv = NULL;
										}
										decolle->aprec = NULL;
										decolle->asuiv = NULL;
										
										nombreElementSurCase = 0;
								}
								
							}
						}
/*-----------*/ else if(pointeCible->genre == CHATEAU){
							
							if(tmp->genre == BARON){
								if(lanceDee * CBARON > lanceDee2 * CCHATEAU){
										if(pointeCible->vsuiv != NULL){
												
												/* Si il ya un chateau après celui_ci ou un chanteau avant celui_ci (dans la liste chainée
												 * on va retourner une valeur pour sortir de la fonction
												 * ( Je ne peux pas modifier la liste chainée de la fonction sinon il me retourne une erreur)
												 * et on modifiera la liste chainée pour pouvoir supprimer le chateau de la liste chainée */
												 
												
												return 90;
											}
											if(pointeCible->vprec != NULL){
												
												/* Si on détruit un chateau créer par un baron et qu'il ya d'autre chateaux 
												 * alors on va pointer vers le chateau précedent et supprimer l'actuel*/
												 
												pointeCible = pointeCible->vprec;
												if(pointeCible->vsuiv->vsuiv != NULL){
													
													pointeCible->vsuiv = pointeCible->vsuiv->vsuiv;
													pointeCible = pointeCible->vsuiv;
													pointeCible->vprec = pointeCible->vprec->vprec;
													return 0;
												}
												else{
													pointeCible->vsuiv = NULL;
													return 0;
												}
											}
											else{
												/*Sinon on retourne -10 pour dire que tout le monde est mort dans cette équipe et que la partie est terminée */
												
												if(tmp->couleur == ROUGE)
													return -10;
										
												else 
													return -20;
											}
											
											nombreElementSurCase = 0;
								}
						
								else{
										decolle = tmp;
										tmp = tmp->aprec;
										
										world->plateau[decolle->posx][decolle->posy].habitant = NULL;
											
										if(tmp->asuiv->asuiv != NULL){
											tmp->asuiv = pointeCible->asuiv->asuiv;
											tmp = pointeCible->asuiv;
											tmp->aprec = pointeCible->aprec->aprec;
											tmp = pointeCible->aprec;
											
											}
										else{
											tmp->asuiv = NULL;
											}
										
										decolle->aprec = NULL;
										decolle->asuiv = NULL;
										nombreElementSurCase = 0;
										}
							}
							
							// SI L'ATTAQUANT EST UN GUERRIER
				else if(tmp->genre == GUERRIER){
								if(lanceDee * CGUERRIER > lanceDee2 * CCHATEAU){
									
										if(pointeCible->vsuiv != NULL){
												
												/* Si il ya un chateau après celui_ci ou un chanteau avant celui_ci (dans la liste chainée
												 * on va retourner une valeur pour sortir de la fonction
												 * ( Je ne peux pas modifier la liste chainée de la fonction sinon il me retourne une erreur)
												 * et on modifiera la liste chainée pour pouvoir supprimer le chateau de la liste chainée */
												 
												
												return 90;
											}
											if(pointeCible->vprec != NULL){
												
												/* Si on détruit un chateau créer par un baron et qu'il ya d'autre chateaux 
												 * alors on va pointer vers le chateau précedent et supprimer l'actuel*/
												 
												pointeCible = pointeCible->vprec;
												if(pointeCible->vsuiv->vsuiv != NULL){
													
													pointeCible->vsuiv = pointeCible->vsuiv->vsuiv;
													pointeCible = pointeCible->vsuiv;
													pointeCible->vprec = pointeCible->vprec->vprec;
													return 0;
												}
												else{
													pointeCible->vsuiv = NULL;
													return 0;
												}
											}
											else{
												/*Sinon on retourne -10 pour dire que tout le monde est mort dans cette équipe et que la partie est terminée */
												
												if(tmp->couleur == ROUGE)
													return -10;
										
												else 
													return -20;
											}
											
											nombreElementSurCase = 0;
								}
								else{
										decolle = tmp;
										tmp = tmp->aprec;
										
										world->plateau[tmp->posx][tmp->posy].habitant = NULL;
										
										if(tmp->asuiv->asuiv != NULL){
											tmp->asuiv = pointeCible->asuiv->asuiv;
											tmp = pointeCible->asuiv;
											tmp->aprec = pointeCible->aprec->aprec;
											tmp = pointeCible->aprec;
											
										}
										else{
											tmp->asuiv = NULL;
										}
										decolle->aprec = NULL;
										decolle->asuiv = NULL;
										
										nombreElementSurCase = 0;
								}
								
							}
						}
					}
						
					
				}
				/* On gère ici les ennemi setrouvant en haut en en dessous de l'ennemi*/
				if((tmp->posx < tmp->destx || tmp->posx > tmp->destx) && tmp->destx != -1 && tmp->desty != -1){
					
					if((tmp->posx+1 == pointeCible->posx || tmp->posx-1 == pointeCible->posx)){
						
						lanceDee = MLV_get_random_integer(1,100);
						lanceDee2 = MLV_get_random_integer(1,100);
						
						/*Si la cible est un guerrier*/
						if(pointeCible->genre == GUERRIER){	
							
							if(tmp->genre == BARON){
									if(lanceDee * CBARON > lanceDee2 * CGUERRIER){
										/* Si le baron gagne */
										
										soldat -= 1;
										decolle = pointeCible;
										
										world->plateau[decolle->posx][decolle->posy].habitant = NULL;
										
										pointeCible = pointeCible->aprec;
										
										/* Si On le détahce du reste des élément */
										if(pointeCible->asuiv->asuiv != NULL){
											pointeCible->asuiv = pointeCible->asuiv->asuiv;
											pointeCible = pointeCible->asuiv;
											pointeCible->aprec = pointeCible->aprec->aprec;
											pointeCible = pointeCible->aprec;
											
										}
										else{
											pointeCible->asuiv = NULL;
										}
										// 321
										free(decolle); /* On libère la mémoire */
										
										nombreElementSurCase --;
									}
									
									else{
										/* Si le baron perd le combat, on le détache du reste des élément */
										decolle = tmp;
										tmp = tmp->aprec;
										
										world->plateau[tmp->posx][tmp->posy].habitant = NULL;
										
										if(tmp->asuiv->asuiv != NULL){
											tmp->asuiv = pointeCible->asuiv->asuiv;
											tmp = pointeCible->asuiv;
											tmp->aprec = pointeCible->aprec->aprec;
											tmp = pointeCible->aprec;
											
										}
										else{
											tmp->asuiv = NULL;
										}
										free(decolle);
										nombreElementSurCase = 0;
									}
								
							}
							//////////////////////////////////////////////////////////////////////////////////////
							if(tmp->genre == GUERRIER){
								if(lanceDee * CGUERRIER > lanceDee2 * CGUERRIER){
									soldat -= 1;
									decolle = pointeCible;
									
									world->plateau[decolle->posx][decolle->posy].habitant = NULL;
									
									pointeCible = pointeCible->aprec;
									
									
									if(pointeCible->asuiv->asuiv != NULL){
										pointeCible->asuiv = pointeCible->asuiv->asuiv;
										pointeCible = pointeCible->asuiv;
										pointeCible->aprec = pointeCible->aprec->aprec;
										pointeCible = pointeCible->aprec;
										
									}
									else{
										pointeCible->asuiv = NULL;
									}
									
									free(decolle);
									
									nombreElementSurCase --;
								}
								else{
									decolle = tmp;
									tmp = tmp->aprec;
									
									world->plateau[tmp->posx][tmp->posy].habitant = NULL;
									
									if(tmp->asuiv->asuiv != NULL){
										tmp->asuiv = pointeCible->asuiv->asuiv;
										tmp = pointeCible->asuiv;
										tmp->aprec = pointeCible->aprec->aprec;
										tmp = pointeCible->aprec;
										
									}
									else{
										tmp->asuiv = NULL;
									}
									free(decolle);
									
									nombreElementSurCase = 0;
								}
								
							}
							//
						}
						
						// Si l'ennemi est un baron ////////////////////////////////////////////////////
						
						if(pointeCible->genre == BARON){
							//
							if(tmp->genre == BARON){
								
								if(lanceDee * CBARON > lanceDee2 * CBARON){
										baron -=1;
										decolle = pointeCible;
										pointeCible = pointeCible->aprec;
										
										world->plateau[decolle->posx][decolle->posy].habitant = NULL;
										
										if(pointeCible->asuiv->asuiv != NULL){
											pointeCible->asuiv = pointeCible->asuiv->asuiv;
											pointeCible = pointeCible->asuiv;
											pointeCible->aprec = pointeCible->aprec->aprec;
											pointeCible = pointeCible->aprec;
											
										}
										else{
											pointeCible->asuiv = NULL;
										}
										decolle->aprec = NULL;
										decolle->asuiv = NULL;
										
										nombreElementSurCase --;
									}
							else{
									decolle = tmp;
									tmp = tmp->aprec;
									
									world->plateau[decolle->posx][decolle->posy].habitant = NULL;
										
									if(tmp->asuiv->asuiv != NULL){
										tmp->asuiv = pointeCible->asuiv->asuiv;
										tmp = pointeCible->asuiv;
										tmp->aprec = pointeCible->aprec->aprec;
										tmp = pointeCible->aprec;
										
										}
									else{
										tmp->asuiv = NULL;
										}
									
									decolle->aprec = NULL;
									decolle->asuiv = NULL;
									nombreElementSurCase = 0;
								}
							}
							
							if(tmp->genre == GUERRIER){
								
								if(lanceDee * CGUERRIER > lanceDee2 * CMANANT){
										baron -= 1;
										decolle = pointeCible;
										
										world->plateau[decolle->posx][decolle->posy].habitant = NULL;
										
										pointeCible = pointeCible->aprec;
										
										
										if(pointeCible->asuiv->asuiv != NULL){
											pointeCible->asuiv = pointeCible->asuiv->asuiv;
											pointeCible = pointeCible->asuiv;
											pointeCible->aprec = pointeCible->aprec->aprec;
											pointeCible = pointeCible->aprec;
											
										}
										else{
											pointeCible->asuiv = NULL;
										}
										decolle->aprec = NULL;
										decolle->asuiv = NULL;
										
										nombreElementSurCase --;
								}
								
								else{
										decolle = tmp;
										tmp = tmp->aprec;
										
										world->plateau[tmp->posx][tmp->posy].habitant = NULL;
										
										if(tmp->asuiv->asuiv != NULL){
											tmp->asuiv = pointeCible->asuiv->asuiv;
											tmp = pointeCible->asuiv;
											tmp->aprec = pointeCible->aprec->aprec;
											tmp = pointeCible->aprec;
											
										}
										else{
											tmp->asuiv = NULL;
										}
										decolle->aprec = NULL;
										decolle->asuiv = NULL;
										
										nombreElementSurCase = 0;
								}
								
							}
						}
						
						
				if(pointeCible->genre == MANANT){
							//
							if(tmp->genre == BARON){
								
								if(lanceDee * CBARON > lanceDee2 * CMANANT){
										manants -= 1;
										decolle = pointeCible;
										pointeCible = pointeCible->aprec;
										
										world->plateau[decolle->posx][decolle->posy].habitant = NULL;
										
										if(pointeCible->asuiv->asuiv != NULL){
											pointeCible->asuiv = pointeCible->asuiv->asuiv;
											pointeCible = pointeCible->asuiv;
											pointeCible->aprec = pointeCible->aprec->aprec;
											pointeCible = pointeCible->aprec;
											
										}
										else{
											pointeCible->asuiv = NULL;
										}
										decolle->aprec = NULL;
										decolle->asuiv = NULL;
										
										nombreElementSurCase --;
							}	
							
							else{
								decolle = tmp;
								tmp = tmp->aprec;
								
								world->plateau[decolle->posx][decolle->posy].habitant = NULL;
									
								if(tmp->asuiv->asuiv != NULL){
									tmp->asuiv = pointeCible->asuiv->asuiv;
									tmp = pointeCible->asuiv;
									tmp->aprec = pointeCible->aprec->aprec;
									tmp = pointeCible->aprec;
									
									}
								else{
									tmp->asuiv = NULL;
									}
								
								decolle->aprec = NULL;
								decolle->asuiv = NULL;
								nombreElementSurCase = 0;
								}
							}
							// ATAQQUANT GUERRIER
							if(tmp->genre == GUERRIER){
								if(lanceDee * CGUERRIER > lanceDee2 * CMANANT){
									manants -= 1;
									decolle = pointeCible;
									
									world->plateau[decolle->posx][decolle->posy].habitant = NULL;
									
									pointeCible = pointeCible->aprec;
									
									
									if(pointeCible->asuiv->asuiv != NULL){
										pointeCible->asuiv = pointeCible->asuiv->asuiv;
										pointeCible = pointeCible->asuiv;
										pointeCible->aprec = pointeCible->aprec->aprec;
										pointeCible = pointeCible->aprec;
										
									}
									else{
										pointeCible->asuiv = NULL;
									}
									decolle->aprec = NULL;
									decolle->asuiv = NULL;
									
									nombreElementSurCase --;
								}
								else{
									decolle = tmp;
									tmp = tmp->aprec;
									
									world->plateau[tmp->posx][tmp->posy].habitant = NULL;
									
									if(tmp->asuiv->asuiv != NULL){
										tmp->asuiv = pointeCible->asuiv->asuiv;
										tmp = pointeCible->asuiv;
										tmp->aprec = pointeCible->aprec->aprec;
										tmp = pointeCible->aprec;
										
									}
									else{
										tmp->asuiv = NULL;
									}
									decolle->aprec = NULL;
									decolle->asuiv = NULL;
									
									nombreElementSurCase = 0;
								}
								
							}
						}
						
/*------	*/else if(pointeCible->genre == CHATEAU ){
							
							if(tmp->genre == BARON){
								if(lanceDee * CBARON > lanceDee2 * CCHATEAU){
									
									if(pointeCible->vsuiv != NULL && pointeCible->vprec != NULL){
										return 90;
									}
									if(pointeCible->vprec != NULL){
										pointeCible = pointeCible->vprec;
										if(pointeCible->vsuiv->vsuiv != NULL){
											
											pointeCible->vsuiv = pointeCible->vsuiv->vsuiv;
											pointeCible = pointeCible->vsuiv;
											pointeCible->vprec = pointeCible->vprec->vprec;
											return 0;
										}
										else{
											pointeCible->vsuiv = NULL;
											return 0;
										}
									}
									else{
										if(tmp->couleur == ROUGE)
											return -10;
										
										else 
											return -20;
									}
									return 0;
								}
						
								else{
									decolle = tmp;
									tmp = tmp->aprec;
									
									world->plateau[decolle->posx][decolle->posy].habitant = NULL;
										
									if(tmp->asuiv->asuiv != NULL){
										tmp->asuiv = pointeCible->asuiv->asuiv;
										tmp = pointeCible->asuiv;
										tmp->aprec = pointeCible->aprec->aprec;
										tmp = pointeCible->aprec;
										
										}
									else{
										tmp->asuiv = NULL;
										}
									
									decolle->aprec = NULL;
									decolle->asuiv = NULL;
									nombreElementSurCase = 0;
									}
							}
							
							// SI L'ATTAQUANT EST UN GUERRIER
							else if(tmp->genre == GUERRIER){
								
								if(lanceDee * CGUERRIER > lanceDee2 * CCHATEAU){
									if(pointeCible->vsuiv != NULL){
												
												/* Si il ya un chateau après celui_ci ou un chanteau avant celui_ci (dans la liste chainée
												 * on va retourner une valeur pour sortir de la fonction
												 * ( Je ne peux pas modifier la liste chainée de la fonction sinon il me retourne une erreur)
												 * et on modifiera la liste chainée pour pouvoir supprimer le chateau de la liste chainée */
												 
												
												return 90;
											}
											if(pointeCible->vprec != NULL){
												
												/* Si on détruit un chateau créer par un baron et qu'il ya d'autre chateaux 
												 * alors on va pointer vers le chateau précedent et supprimer l'actuel*/
												 
												pointeCible = pointeCible->vprec;
												if(pointeCible->vsuiv->vsuiv != NULL){
													
													pointeCible->vsuiv = pointeCible->vsuiv->vsuiv;
													pointeCible = pointeCible->vsuiv;
													pointeCible->vprec = pointeCible->vprec->vprec;
													return 0;
												}
												else{
													pointeCible->vsuiv = NULL;
													return 0;
												}
											}
											else{
												/*Sinon on retourne -10 pour dire que tout le monde est mort dans cette équipe et que la partie est terminée */
												
												if(tmp->couleur == ROUGE)
													return -10;
										
												else 
													return -20;
											}
											
											nombreElementSurCase = 0;
											
										return 0;
								}
								else{
									decolle = tmp;
									tmp = tmp->aprec;
									
									world->plateau[tmp->posx][tmp->posy].habitant = NULL;
									
									if(tmp->asuiv->asuiv != NULL){
										tmp->asuiv = pointeCible->asuiv->asuiv;
										tmp = pointeCible->asuiv;
										tmp->aprec = pointeCible->aprec->aprec;
										tmp = pointeCible->aprec;
										
									}
									else{
										tmp->asuiv = NULL;
									}
									decolle->aprec = NULL;
									decolle->asuiv = NULL;
									
									nombreElementSurCase = 0;
								}
								
							}
						}
					}
				}
				
				
				else if((tmp->posy < tmp->desty ||tmp->posy > tmp->desty)  && tmp->posx == tmp->destx && tmp->destx != -1){
					
					if((tmp->posy+1 == pointeCible->posy || tmp->posy-1 == pointeCible->posy) && tmp->posx == pointeCible->posx){
						
						MLV_actualise_window();
						lanceDee = MLV_get_random_integer(0,100);
						lanceDee2 = MLV_get_random_integer(0,100);
						
	/* CIBLE */		if(pointeCible->genre == GUERRIER){	
							
							if(tmp->genre == BARON){
								
								if(lanceDee * CGUERRIER > lanceDee2 * CBARON){
										soldat -= 1;
										decolle = pointeCible;
										
										world->plateau[decolle->posx][decolle->posy].habitant = NULL;
										
										pointeCible = pointeCible->aprec;
										
										
										if(pointeCible->asuiv->asuiv != NULL){
											pointeCible->asuiv = pointeCible->asuiv->asuiv;
											pointeCible = pointeCible->asuiv;
											pointeCible->aprec = pointeCible->aprec->aprec;
											pointeCible = pointeCible->aprec;
											
										}
										else{
											pointeCible->asuiv = NULL;
										}
										
											free(decolle);
										
										nombreElementSurCase --;
								}
								else{
									decolle = tmp;
									tmp = tmp->aprec;
									
									world->plateau[tmp->posx][tmp->posy].habitant = NULL;
									
									if(tmp->asuiv->asuiv != NULL){
										tmp->asuiv = pointeCible->asuiv->asuiv;
										tmp = pointeCible->asuiv;
										tmp->aprec = pointeCible->aprec->aprec;
										tmp = pointeCible->aprec;
										
									}
									else{
										tmp->asuiv = NULL;
									}
									
									free(decolle);
									
									nombreElementSurCase = 0;
								}
								
							}
						if(tmp->genre == GUERRIER){
								if(lanceDee * CGUERRIER > lanceDee2 * CGUERRIER){
									soldat -= 1;
									decolle = pointeCible;
									
									world->plateau[decolle->posx][decolle->posy].habitant = NULL;
									
									pointeCible = pointeCible->aprec;
									
									
									if(pointeCible->asuiv->asuiv != NULL){
										pointeCible->asuiv = pointeCible->asuiv->asuiv;
										pointeCible = pointeCible->asuiv;
										pointeCible->aprec = pointeCible->aprec->aprec;
										pointeCible = pointeCible->aprec;
										
									}
									else{
										pointeCible->asuiv = NULL;
									}
									free(decolle);
									
									nombreElementSurCase --;
								}
								else{
									decolle = tmp;
									tmp = tmp->aprec;
									
									world->plateau[tmp->posx][tmp->posy].habitant = NULL;
									
									if(tmp->asuiv->asuiv != NULL){
										tmp->asuiv = pointeCible->asuiv->asuiv;
										tmp = pointeCible->asuiv;
										tmp->aprec = pointeCible->aprec->aprec;
										tmp = pointeCible->aprec;
										
									}
									else{
										tmp->asuiv = NULL;
									}
									free(decolle);
									
									nombreElementSurCase = 0;
								}
								
							}
							//
						}
		/*CIBLE*/else if(pointeCible->genre == BARON){
							
							if(tmp->genre == BARON){
								if( lanceDee * CBARON > lanceDee2 * CBARON ){
									
									baron -=1;
									decolle = pointeCible;
									pointeCible = pointeCible->aprec;
									
									world->plateau[decolle->posx][decolle->posy].habitant = NULL;
									
									if(pointeCible->asuiv->asuiv != NULL){
										pointeCible->asuiv = pointeCible->asuiv->asuiv;
										pointeCible = pointeCible->asuiv;
										pointeCible->aprec = pointeCible->aprec->aprec;
										pointeCible = pointeCible->aprec;
										
									}
									else{
										
										pointeCible->asuiv = NULL;
									}
									
									
									
									free(decolle);
									
									nombreElementSurCase --;
									
								}
							else{
								decolle = tmp;
								tmp = tmp->aprec;
								
								world->plateau[decolle->posx][decolle->posy].habitant = NULL;
									
								if(tmp->asuiv->asuiv != NULL){
									tmp->asuiv = pointeCible->asuiv->asuiv;
									tmp = pointeCible->asuiv;
									tmp->aprec = pointeCible->aprec->aprec;
									tmp = pointeCible->aprec;
									
									}
								else{
									tmp->asuiv = NULL;
									}
								
								free(decolle);
								nombreElementSurCase = 0;
								}
							}
							
							else if(tmp->genre == GUERRIER){
			
								if(lanceDee * CGUERRIER > lanceDee2 * CBARON){
									baron -=1;
									decolle = pointeCible;
									pointeCible = pointeCible->aprec;
									
									world->plateau[decolle->posx][decolle->posy].habitant = NULL;
									
									if(pointeCible->asuiv->asuiv != NULL){
										pointeCible->asuiv = pointeCible->asuiv->asuiv;
										pointeCible = pointeCible->asuiv;
										pointeCible->aprec = pointeCible->aprec->aprec;
										pointeCible = pointeCible->aprec;
										
									}
									else{
										
										pointeCible->asuiv = NULL;
									}
									
									
									
									free(decolle);
									
									nombreElementSurCase --;
									
								}
								else{
									decolle = tmp;
									tmp = tmp->aprec;
									
									world->plateau[tmp->posx][tmp->posy].habitant = NULL;
									
									if(tmp->asuiv->asuiv != NULL){
										tmp->asuiv = pointeCible->asuiv->asuiv;
										tmp = pointeCible->asuiv;
										tmp->aprec = pointeCible->aprec->aprec;
										tmp = pointeCible->aprec;
										
									}
									else{
										tmp->asuiv = NULL;
									}
									decolle->aprec = NULL;
									decolle->asuiv = NULL;
									
									nombreElementSurCase = 0;
								}
								
							}
						}
				else if(pointeCible->genre == MANANT){
							
							if(tmp->genre == BARON){
										if(lanceDee * CBARON > lanceDee2 * CMANANT){
										
											manants -= 1;
											decolle = pointeCible;
											pointeCible = pointeCible->aprec;
											
											world->plateau[decolle->posx][decolle->posy].habitant = NULL;
											
											if(pointeCible->asuiv->asuiv != NULL){
												pointeCible->asuiv = pointeCible->asuiv->asuiv;
												pointeCible = pointeCible->asuiv;
												pointeCible->aprec = pointeCible->aprec->aprec;
												pointeCible = pointeCible->aprec;
												
											}
											else{
												pointeCible->asuiv = NULL;
											}
											free(decolle);
											
											nombreElementSurCase --;
											
									}	
									
									else{
										
										decolle = tmp;
										tmp = tmp->aprec;
										
										world->plateau[decolle->posx][decolle->posy].habitant = NULL;
											
										if(tmp->asuiv->asuiv != NULL){
											tmp->asuiv = pointeCible->asuiv->asuiv;
											tmp = pointeCible->asuiv;
											tmp->aprec = pointeCible->aprec->aprec;
											tmp = pointeCible->aprec;
											
											}
										else{
											tmp->asuiv = NULL;
											}
										
										decolle->aprec = NULL;
										decolle->asuiv = NULL;
										nombreElementSurCase = 0;
										}
							}
							// ATAQQUANT GUERRIER
							if(tmp->genre == GUERRIER){
							
									if(lanceDee * CGUERRIER > lanceDee2 * CMANANT){
											manants -= 1;
											decolle = pointeCible;
											
											world->plateau[decolle->posx][decolle->posy].habitant = NULL;
											
											pointeCible = pointeCible->aprec;
											
											
											if(pointeCible->asuiv->asuiv != NULL){
												pointeCible->asuiv = pointeCible->asuiv->asuiv;
												pointeCible = pointeCible->asuiv;
												pointeCible->aprec = pointeCible->aprec->aprec;
												pointeCible = pointeCible->aprec;
												
											}
											else{
												pointeCible->asuiv = NULL;
											}
											free(decolle);
											
											nombreElementSurCase --;
									}
									else{
											decolle = tmp;
											tmp = tmp->aprec;
											
											world->plateau[tmp->posx][tmp->posy].habitant = NULL;
											
											if(tmp->asuiv->asuiv != NULL){
												tmp->asuiv = pointeCible->asuiv->asuiv;
												tmp = pointeCible->asuiv;
												tmp->aprec = pointeCible->aprec->aprec;
												tmp = pointeCible->aprec;
												
											}
											else{
												tmp->asuiv = NULL;
											}
											free(decolle);
											
											nombreElementSurCase = 0;
									}
									
							}
						}
/*--------*/  else if(pointeCible->genre == CHATEAU){
						
				
						if(tmp->genre == BARON){
							
							if(lanceDee * CBARON > lanceDee2 * CCHATEAU ){
											
											/* Ici les chose vont se compliquer et etre mois logique */
											
											if(pointeCible->vsuiv != NULL){
												
												/* Si il ya un chateau après celui_ci ou un chanteau avant celui_ci (dans la liste chainée
												 * on va retourner une valeur pour sortir de la fonction
												 * ( Je ne peux pas modifier la liste chainée de la fonction sinon il me retourne une erreur)
												 * et on modifiera la liste chainée pour pouvoir supprimer le chateau de la liste chainée */
												 
												
												return 90;
											}
											if(pointeCible->vprec != NULL){
												
												/* Si on détruit un chateau créer par un baron et qu'il ya d'autre chateaux 
												 * alors on va pointer vers le chateau précedent et supprimer l'actuel*/
												 
												pointeCible = pointeCible->vprec;
												if(pointeCible->vsuiv->vsuiv != NULL){
													
													pointeCible->vsuiv = pointeCible->vsuiv->vsuiv;
													pointeCible = pointeCible->vsuiv;
													pointeCible->vprec = pointeCible->vprec->vprec;
													return 0;
												}
												else{
													pointeCible->vsuiv = NULL;
													return 0;
												}
											}
											else{
												/*Sinon on retourne -10 pour dire que tout le monde est mort dans cette équipe et que la partie est terminée */
												
												if(tmp->couleur == ROUGE)
													return -10;
										
												else 
													return -20;
											}
											
											nombreElementSurCase = 0;
										}
						
								else{
										decolle = tmp;
										tmp = tmp->aprec;
										
										world->plateau[decolle->posx][decolle->posy].habitant = NULL;
											
										if(tmp->asuiv->asuiv != NULL){
											tmp->asuiv = pointeCible->asuiv->asuiv;
											tmp = pointeCible->asuiv;
											tmp->aprec = pointeCible->aprec->aprec;
											tmp = pointeCible->aprec;
											
											}
										else{
											tmp->asuiv = NULL;
											}
										
										decolle->aprec = NULL;
										decolle->asuiv = NULL;
										nombreElementSurCase = 0;
										}
							}
							
							// SI L'ATTAQUANT EST UN GUERRIER
						if(tmp->genre == GUERRIER){
								if(lanceDee * CGUERRIER > lanceDee2 * CCHATEAU){
										
										if(pointeCible->vsuiv != NULL){
												
												/* Si il ya un chateau après celui_ci ou un chanteau avant celui_ci (dans la liste chainée
												 * on va retourner une valeur pour sortir de la fonction
												 * ( Je ne peux pas modifier la liste chainée de la fonction sinon il me retourne une erreur)
												 * et on modifiera la liste chainée pour pouvoir supprimer le chateau de la liste chainée */
												 
												
												return 90;
											}
											if(pointeCible->vprec != NULL){
												
												/* Si on détruit un chateau créer par un baron et qu'il ya d'autre chateaux 
												 * alors on va pointer vers le chateau précedent et supprimer l'actuel*/
												 
												pointeCible = pointeCible->vprec;
												if(pointeCible->vsuiv->vsuiv != NULL){
													
													pointeCible->vsuiv = pointeCible->vsuiv->vsuiv;
													pointeCible = pointeCible->vsuiv;
													pointeCible->vprec = pointeCible->vprec->vprec;
													return 0;
												}
												else{
													pointeCible->vsuiv = NULL;
													return 0;
												}
											}
											else{
												/*Sinon on retourne -10 pour dire que tout le monde est mort dans cette équipe et que la partie est terminée */
												
												if(tmp->couleur == ROUGE)
													return -10;
										
												else 
													return -20;
											}
											
											nombreElementSurCase = 0;
									}
									else{
											decolle = tmp;
											tmp = tmp->aprec;
											
											world->plateau[tmp->posx][tmp->posy].habitant = NULL;
											
											if(tmp->asuiv->asuiv != NULL){
												tmp->asuiv = pointeCible->asuiv->asuiv;
												tmp = pointeCible->asuiv;
												tmp->aprec = pointeCible->aprec->aprec;
												tmp = pointeCible->aprec;
												
											}
											else{
												tmp->asuiv = NULL;
											}
											decolle->aprec = NULL;
											decolle->asuiv = NULL;
											
											nombreElementSurCase = 0;
									}
									
								}
						}
					}
						
					
				}
				
				
				if( inverse != 1){
					
					pointeCible = pointeCible->asuiv;
				}
				if(inverse == 1 ){
					
					pointeCible = pointeCible->aprec;
				}
				if(pointeTete->vsuiv != NULL && pointeCible == NULL){
				
					
					pointeTete = pointeTete->vsuiv;
					pointeCible = pointeTete;
					
					
					while(pointeCible->asuiv != NULL){
						
						pointeCible = pointeCible->asuiv;
					}
					
					
					inverse = 2;
				}
				
			}
			
			pointeTete = cible;
			pointeCible = pointeTete;
	}
	tmp->produit = 'r';
	return 1;
}

/* Dans cette fonction on va vérifier sur cette fonction qu'il n'y ait pas d'autre manant que celuis qu'on à placer
 * Pour éviter d'avoir 2 manants sur la meme case, (Comme demandé dans la consigne ) */

/*void liaisonCase(Agent *tmp,Agent *teamBleu, Agent *teamRouge){
	 Cette fonction devait permettre de faire la dernière question à savoir relier les élément 
	 * d'un meme case entre eux, mais j'ai du la retirer à cause de plusieurs bugs qui sont apparu
	 * après 
	 
	while(VoisinCase != NULL){
								
			if(VoisinCase->posx == tmp->posx && VoisinCase->posy == tmp->posy && VoisinCase->genre != CHATEAU){
						
				tmp->vsuiv = VoisinCase;
				VoisinCase->vprec = tmp;
			}
			else{
									
				tmp->vsuiv = NULL;
				VoisinCase->vprec = NULL;
			}
			if(VoisinCase == NULL && VoisinDuVoisin->vsuiv != NULL){
				VoisinCase = VoisinCase->vsuiv;
				VoisinDuVoisin = VoisinDuVoisin->vsuiv;
				}
				VoisinCase = VoisinCase->asuiv;
		}
}*/

int ManantSurCase(Agent *tmp,Agent *cible,Agent *tmp2){
	Agent *TeteListe2 = tmp2;
	
	Agent *corps2 = tmp2; 
	
	Agent *TeteListe = tmp;	
	
	Agent *corps = tmp; 
	
	Agent *Exclu = tmp;	/* On pointera sur le manants POUR lequel on compare les cases */
	
	// Comme on cherche a avoir le debut de la liste chainée, on commence par remonter jusqu'au chateau qui est en tete de liste 
	while(TeteListe->genre != CHATEAU){
		
		TeteListe = TeteListe->aprec;
	}
	
	// Puis on vérifie si il y'a d'autre chaines chez les voisins 
	while(TeteListe->vprec != NULL){
		
		TeteListe = TeteListe->vprec;
	}
	
	 //On reparcours le tout 
	
	while(corps != NULL){
		
		if(corps->posx == Exclu->posx && corps->posy == Exclu->posy && corps->genre == MANANT && corps != Exclu){
			Exclu->produit = 'x';
			return -1;
		}
		corps = corps ->asuiv;
		if(corps == NULL && TeteListe->vsuiv!= NULL){
			TeteListe = TeteListe->vsuiv;
			corps = TeteListe;
		}
	}
	
	
	
	
	while(corps2 != NULL){
		
		if(corps2->posx == Exclu->posx && corps2->posy == Exclu->posy && corps2->genre == MANANT && corps2 != Exclu){
			Exclu->produit = 'x';
			return -1;
		}
		corps2 = corps2 ->asuiv;
		
		if(corps2 == NULL && TeteListe2->vsuiv!= NULL){
			TeteListe2 = TeteListe2->vsuiv;
			corps2 = TeteListe2;
		}
	}
	
	return 1;
}

/* On affichera la liste des menu et des choix selon le tours de l'agent que l'on joue 
 * Les boutton sont violets si on peut les utiliser, gris sinons*/

void sauvegarde( Agent *teamRouge, Agent *teamBleu){
	
	 
	AListe rouge = teamRouge;
	Agent *Ragent = rouge;
	
	AListe bleu = teamBleu;
	Agent *Bagent = bleu;
	
	FILE* fichier = NULL;
	
	fichier = fopen("sauvegarde", "w+");
	
	while(Ragent != NULL){
		
		fprintf(fichier,"%c %d %d\n",Ragent->genre,Ragent->posx,Ragent->posy);
		
		
		Ragent = Ragent->asuiv;
		
		if(Ragent==NULL && rouge->vsuiv != NULL){
			fprintf(fichier,"/\n");
			rouge = rouge->vsuiv;
			Ragent = rouge;
		}
	}
	fprintf(fichier,"!\n");
	
	while(Bagent != NULL){
		
		fprintf(fichier,"%c %d %d\n",Bagent->genre,Bagent->posx,Bagent->posy);
		
		
		Bagent = Bagent->asuiv;
		
		if(Bagent==NULL && bleu->vsuiv != NULL){
			fprintf(fichier,"/\n");
			bleu = bleu->vsuiv;
			Ragent = bleu;
		}
	}
	
	fclose(fichier);
	
}

int  listeMenu(Agent *liste,Agent *teamRouge,Agent *teamBleu){
	
	int x, y ;
	
	int aretourner1 = 0;
	
	int commence = 1;
	
	while (commence == 1){
		
		MLV_draw_filled_rectangle( 1280, 620 , 160, 40, MLV_COLOR_ORANGE);
		MLV_draw_text(1310, 635, "SAUVEGARDER",MLV_COLOR_WHITE);
		
		if(liste->genre == CHATEAU){
			
			
			/* Si le chateau ne produit rien ('/') alors on entre dans cette liste de menu */
			
			if(liste->produit == '/'){
				MLV_draw_filled_rectangle( 1280, 160 , 160, 40, MLV_COLOR_PURPLE);
				MLV_draw_text(1310, 170, "ATTENDRE",MLV_COLOR_WHITE);
				MLV_draw_filled_rectangle( 1280, 220 , 170, 40, MLV_COLOR_PURPLE);
				MLV_draw_text(1290, 235, "PRODUIRE GUERRIER",MLV_COLOR_WHITE);
				MLV_draw_filled_rectangle( 1280, 280 , 160, 40, MLV_COLOR_PURPLE );
				MLV_draw_text(1290, 295, "PRODUIRE MANANT",MLV_COLOR_WHITE);
				MLV_draw_filled_rectangle( 1280, 340 , 160, 40, MLV_COLOR_PURPLE );
				MLV_draw_text(1290, 352, "PRODUIRE BARON",MLV_COLOR_WHITE);
				MLV_draw_filled_rectangle( 1280, 540 , 160, 40, MLV_COLOR_RED );
				MLV_draw_text(1290, 552, "QUITTER LE JEU",MLV_COLOR_WHITE);
				
				
				MLV_actualise_window();
				MLV_wait_mouse(&x, &y);
				
				/*les valeur ici sont par rapport à la position des différents boutons que l'ont à placé 
				 * 
				 * */
				 
				if((x >= 1280 && x <= 1438) && (y >= 160 && y <= 199)){
					return -1;
				}
				if((x >= 1280 && x <= 1438) && (y >= 221 && y <= 258)){
					liste->produit = GUERRIER;
					return 4;
				}
				if((x >= 1280 && x <= 1438) && (y >= 281 && y <= 319)){
					liste->produit = MANANT;
					return 2;
				}
				if((x >= 1280 && x <= 1438) && (y >= 341 && y <= 379)){
					liste->produit = BARON;
					return 6;
				}
				if((x >= 1280 && x <= 1438) && (y >= 541 && y <= 577)){
					
					/*on retourne 666 ici pour indiquer que le arrete le jeu */
					return 666;
				}
				if((x >= 1280 && x<= 1438) && (y >= 621 && y <= 658)){
				
					sauvegarde(teamRouge,teamBleu);
					MLV_draw_text(1280, 675, "Partie sauvegardé",MLV_COLOR_GREEN);
					MLV_actualise_window();
				}
			}
			else{
				MLV_draw_filled_rectangle( 1280, 160 , 160, 40, MLV_COLOR_PURPLE);
				MLV_draw_text(1310, 170, "ATTENDRE",MLV_COLOR_WHITE);
				MLV_draw_filled_rectangle( 1280, 220 , 170, 40, MLV_COLOR_GREY);
				MLV_draw_text(1290, 235, "PRODUIRE GUERRIER",MLV_COLOR_WHITE);
				MLV_draw_filled_rectangle( 1280, 280 , 160, 40, MLV_COLOR_GREY);
				MLV_draw_text(1290, 295, "PRODUIRE MANANT",MLV_COLOR_WHITE);
				MLV_draw_filled_rectangle( 1280, 340 , 160, 40, MLV_COLOR_GREY );
				MLV_draw_text(1290, 352, "PRODUIRE BARON",MLV_COLOR_WHITE);
				MLV_draw_filled_rectangle( 1280, 540 , 160, 40, MLV_COLOR_RED );
				MLV_draw_text(1290, 552, "QUITTER LE JEU",MLV_COLOR_WHITE);
				
				
				MLV_actualise_window();
				MLV_wait_mouse(&x, &y);
				if((x >= 1280 && x <= 1438) && (y >= 160 && y <= 199)){
					return -1;
				}
				if((x >= 1280 && x <= 1438) && (y >= 221 && y <= 379)){
					MLV_draw_text(1290, 500, "Le chateau travaille déjà",MLV_COLOR_WHITE);
				}
				if((x >= 1280 && x <= 1438) && (y >= 541 && y <= 577)){
					return 666;
				}
				if((x >= 1280 && x<= 1438) && (y >= 621 && y <= 658)){
				
					sauvegarde(teamRouge,teamBleu);
					MLV_draw_text(1280, 675, "Partie sauvegardé",MLV_COLOR_GREEN);
					MLV_actualise_window();
				}
				
			}
		}
		
		if(liste->genre == GUERRIER){
			
			if(liste->destx == -1 && liste -> desty== -1){
				MLV_actualise_window();
				MLV_draw_filled_rectangle( 1280, 160 , 160, 40, MLV_COLOR_PURPLE);
				MLV_draw_text(1310, 170, "IMMOBILE",MLV_COLOR_WHITE);
				MLV_draw_filled_rectangle( 1280, 220 , 160, 40,MLV_COLOR_PURPLE );
				MLV_draw_text(1290,235, "DEPLACER",MLV_COLOR_WHITE);
				if(liste->produit == 'c'){
					MLV_draw_filled_rectangle( 1280, 280 , 160, 40,MLV_COLOR_PURPLE );
					MLV_draw_text(1290,295, "ATTAQUER",MLV_COLOR_WHITE);
				}
				else{
					MLV_draw_filled_rectangle( 1280, 280 , 160, 40,MLV_COLOR_GREY);
					MLV_draw_text(1290,295, "ATTAQUER",MLV_COLOR_WHITE);
				}
				MLV_draw_filled_rectangle( 1280, 540 , 160, 40, MLV_COLOR_RED );
				MLV_draw_text(1290, 552, "QUITTER LE JEU",MLV_COLOR_WHITE);
				
				
				MLV_actualise_window();
				MLV_wait_mouse(&x, &y);
				if((x >= 1280 && x <= 1438) && (y >= 160 && y <= 199)){
					return 0;
				}
				if((x >= 1280 && x <= 1438) && (y >= 221 && y <= 258)){
					
					aretourner1 = listeDeplacement(liste);
					
					return aretourner1;
				}
				if(liste->produit =='c'){
					if((x >= 1280 && x <= 1438) && (y >= 281 && y <= 319)){
					
					
					return 6;
					}
				}
				else{
					if((x >= 1280 && x <= 1438) && (y >= 281 && y <= 319)){
						MLV_draw_text(1290, 350, "Aucune cible en vue",MLV_COLOR_RED);
					}
				}
				if((x >= 1280 && x <= 1438) && (y >= 541 && y <= 577)){
					return 666;
				}
				if((x >= 1280 && x<= 1438) && (y >= 621 && y <= 658)){
				
					sauvegarde(teamRouge,teamBleu);
					MLV_draw_text(1280, 675, "Partie sauvegardé",MLV_COLOR_GREEN);
					MLV_actualise_window();
				}
			}
		
			else{
				MLV_draw_filled_rectangle( 1280, 160 , 160, 40, MLV_COLOR_PURPLE);
				MLV_draw_text(1310, 170, "PASSER",MLV_COLOR_WHITE);
				MLV_draw_filled_rectangle( 1280, 220 , 160, 40,MLV_COLOR_GREY );
				MLV_draw_text(1290,235, "IMMOBILE",MLV_COLOR_WHITE);
				MLV_draw_filled_rectangle( 1280, 280 , 160, 40,MLV_COLOR_GREY);
				if(liste->produit == 'c'){
					MLV_draw_filled_rectangle( 1280, 280 , 160, 40,MLV_COLOR_PURPLE );
					MLV_draw_text(1290,295, "ATTAQUER",MLV_COLOR_WHITE);
				}
				else{
					MLV_draw_filled_rectangle( 1280, 280 , 160, 40,MLV_COLOR_GREY);
					MLV_draw_text(1290,295, "ATTAQUER",MLV_COLOR_WHITE);
				}
				MLV_draw_filled_rectangle( 1280, 540 , 160, 40, MLV_COLOR_RED );
				MLV_draw_text(1290, 552, "QUITTER LE JEU",MLV_COLOR_WHITE);
				
				MLV_actualise_window();
				
				MLV_wait_mouse(&x, &y);
				
				if((x >= 1280 && x <= 1438) && (y >= 160 && y <= 199)){
					
					return 0;
				}
				if((x >= 1280 && x <= 1438) && (y >= 282 && y <= 318)){
					return 6;
				} 
				if((x >= 1280 && x <= 1438) && (y >= 221 && y <= 319)){
					
					MLV_draw_text(1290, 160, "Le guerrier se déplace",MLV_COLOR_WHITE);
				}
				if((x >= 1280 && x<= 1438) && (y >= 621 && y <= 658)){
				
					sauvegarde(teamRouge,teamBleu);
					MLV_draw_text(1280, 675, "Partie sauvegardé",MLV_COLOR_GREEN);
					MLV_actualise_window();
				}
				
			 }
		}
		
		if(liste->genre == BARON){
			
			if(liste->destx == -1 && liste->desty == -1){
				MLV_draw_filled_rectangle( 1280, 160 , 160, 40, MLV_COLOR_PURPLE);
				MLV_draw_text(1310, 170, "IMMOBILE",MLV_COLOR_WHITE);
				MLV_draw_filled_rectangle( 1280, 220 , 160, 40,MLV_COLOR_PURPLE );
				MLV_draw_text(1290,235, "DEPLACER",MLV_COLOR_WHITE);
				if(liste->produit == 'c'){
					MLV_draw_filled_rectangle( 1280, 280 , 160, 40,MLV_COLOR_PURPLE );
					MLV_draw_text(1290,295, "ATTAQUER",MLV_COLOR_WHITE);
				}
				else{
					MLV_draw_filled_rectangle( 1280, 280 , 160, 40,MLV_COLOR_GREY);
					MLV_draw_text(1290,295, "ATTAQUER",MLV_COLOR_WHITE);
				}
				MLV_draw_filled_rectangle( 1280, 540 , 160, 40, MLV_COLOR_RED );
				MLV_draw_text(1290, 552, "QUITTER LE JEU",MLV_COLOR_WHITE);
				
				MLV_actualise_window();
				MLV_wait_mouse(&x, &y);
				if((x >= 1280 && x <= 1438) && (y >= 160 && y <= 199)){
					return 0;
				}
				if((x >= 1280 && x <= 1438) && (y >= 221 && y <= 258)){
				
					aretourner1 = listeDeplacement(liste);
					
					return aretourner1;
				}
				if(liste->produit =='c'){
					if((x >= 1280 && x <= 1438) && (y >= 281 && y <= 319)){
					
					return 6;
					}
				}
				else{
					if((x >= 1280 && x <= 1438) && (y >= 281 && y <= 319)){
						MLV_draw_text(1290, 350, "Aucune cible en vue",MLV_COLOR_RED);
					}
				}
				if((x >= 1280 && x <= 1438) && (y >= 541 && y <= 577)){
					return 666;
				}
				if((x >= 1280 && x<= 1438) && (y >= 621 && y <= 658)){
				
					sauvegarde(teamRouge,teamBleu);
					MLV_draw_text(1280, 675, "Partie sauvegardé",MLV_COLOR_GREEN);
					MLV_actualise_window();
				}
			}
			else{
				MLV_draw_filled_rectangle( 1280, 160 , 160, 40, MLV_COLOR_PURPLE);
				MLV_draw_text(1310, 170, "PASSER",MLV_COLOR_WHITE);
				MLV_draw_filled_rectangle( 1280, 220 , 160, 40,MLV_COLOR_GREY);
				MLV_draw_text(1290,235, "IMMOBILE",MLV_COLOR_WHITE);
				MLV_draw_filled_rectangle( 1280, 280 , 160, 40, MLV_COLOR_GREY);
				MLV_draw_text(1290, 295, "DEPLACER",MLV_COLOR_WHITE);
				if(liste->produit == 'c'){
					MLV_draw_filled_rectangle( 1280, 340 , 160, 40,MLV_COLOR_PURPLE );
					MLV_draw_text(1290,352, "ATTAQUER",MLV_COLOR_WHITE);
				}
				else{
					MLV_draw_filled_rectangle( 1280, 340 , 160, 40,MLV_COLOR_GREY);
					MLV_draw_text(1290,352, "ATTAQUER",MLV_COLOR_WHITE);
					
				}
				MLV_draw_filled_rectangle( 1280, 540 , 160, 40, MLV_COLOR_RED );
				MLV_draw_text(1290, 552, "QUITTER LE JEU",MLV_COLOR_WHITE);
				
				MLV_actualise_window();
				MLV_wait_mouse(&x, &y);
				if((x >= 1280 && x <= 1438) && (y >= 160 && y <= 199)){
					return 1;
				}
				if((x >= 1280 && x <= 1438) && (y >= 341 && y <= 379) && liste->produit =='c'){
					
					return 6;
				}
				if((x >= 1280 && x <= 1438) && (y >= 221 && y <= 379) && liste->produit != 'c'){
					
					MLV_draw_text(1290, 350, "Le Baron travaille déjà",MLV_COLOR_WHITE);
					
				}
				if(liste->produit =='c'){
					if((x >= 1280 && x <= 1438) && (y >= 281 && y <= 319)){
					
					return 6;
					}
				}
				if((x >= 1280 && x <= 1438) && (y >= 541 && y <= 577)){
					return 666;
				}
				if((x >= 1280 && x<= 1438) && (y >= 621 && y <= 658)){
				
					sauvegarde(teamRouge,teamBleu);
					MLV_draw_text(1280, 675, "Partie sauvegardé",MLV_COLOR_GREEN);
					MLV_actualise_window();
				}
				
			}
		}
		if(liste->genre == MANANT){
			
			if(liste->produit != 't' && liste->destx == -1){
				MLV_draw_filled_rectangle( 1280, 160 , 160, 40, MLV_COLOR_PURPLE);
				MLV_draw_text(1310, 170, "IMMOBILE",MLV_COLOR_WHITE);
				MLV_draw_filled_rectangle( 1280, 220 , 160, 40,MLV_COLOR_PURPLE );
				MLV_draw_text(1290,235, "DEPLACER",MLV_COLOR_WHITE);
				MLV_draw_filled_rectangle( 1280, 540 , 160, 40, MLV_COLOR_RED );
				MLV_draw_text(1290, 552, "QUITTER LE JEU",MLV_COLOR_WHITE);
				
				MLV_actualise_window();
				MLV_wait_mouse(&x, &y);
				if((x >= 1280 && x <= 1438) && (y >= 160 && y <= 199)){
					
					liste->produit = 't';
					return 0;
				}
				if((x >= 1280 && x <= 1438) && (y >= 221 && y <= 258)){
					
					
					aretourner1 = listeDeplacement(liste);
					return aretourner1;
				}
				if((x >= 1280 && x <= 1438) && (y >= 541 && y <= 577)){
					return 666;
				}
				if((x >= 1280 && x<= 1438) && (y >= 621 && y <= 658)){
				
					sauvegarde(teamRouge,teamBleu);
					MLV_draw_text(1280, 675, "Partie sauvegardé",MLV_COLOR_GREEN);
					MLV_actualise_window();
				}

			}
			
			else {
				MLV_draw_filled_rectangle( 1280, 160 , 160, 40, MLV_COLOR_PURPLE);
				MLV_draw_text(1310, 170, "PASSER",MLV_COLOR_WHITE);
				MLV_draw_filled_rectangle( 1280, 220 , 160, 40,MLV_COLOR_GREY);
				MLV_draw_text(1290,235, "IMMOBILE",MLV_COLOR_WHITE);
				MLV_draw_filled_rectangle( 1280, 280, 160, 40, MLV_COLOR_GREY);
				MLV_draw_text(1290, 295, "DEPLACER",MLV_COLOR_WHITE);
				MLV_draw_filled_rectangle( 1280, 540 , 160, 40, MLV_COLOR_RED );
				MLV_draw_text(1290, 552, "QUITTER LE JEU",MLV_COLOR_WHITE);
				
				MLV_actualise_window();
				MLV_wait_mouse(&x, &y);
				if((x >= 1280 && x <= 1438) && (y >= 160 && y <= 199)){
					return 0;
				}
				if((x >= 1280 && x <= 1438) && (y >= 221 && y <= 319)){
					MLV_draw_text(1290, 230, "Le manant travaille déjà",MLV_COLOR_WHITE);
				}
				if((x >= 1280 && x <= 1438) && (y >= 541 && y <= 577)){
					return 666;
				}
				if((x >= 1280 && x<= 1438) && (y >= 621 && y <= 658)){
				
					sauvegarde(teamRouge,teamBleu);
					MLV_draw_text(1280, 675, "Partie sauvegardé",MLV_COLOR_GREEN);
					MLV_actualise_window();
				}
		}
		MLV_actualise_window();
	}
	
	
	
	
	}
	return -1;
}

/* On fait ici ,(comme l'indique le nom), dessines les différents éléments du jeux */


void dessineTableau(Agent *rouge,Agent *bleu){
	
	int i,j,bonus = 0,nusbo = 0;
	
	Agent *pointer, *pointeb;
	
	pointer = rouge;
	pointeb = bleu;
	
	Agent *tmp = rouge;
	Agent *tmp2 = bleu;

	
	for(i = 0; i < LARG; i++){
			for(j = 0; j < LONG; j++){
					MLV_draw_rectangle( 10 + bonus, 30 + nusbo , 60, 60, MLV_COLOR_WHITE );/*On dessines les différentes cases */
					bonus += 60;
			}	
			bonus = 0;
			nusbo += 60;

	}
	
	int init = 0;
	
	while (init == 0){
		i = 0,j = 0;
		nusbo = 0;
	
		while(i < LARG){
			j = 0;
			while(j < LONG){
				
				while(tmp != NULL){
					
					if ((tmp->posx == i && tmp->posy ==j) ){
						
						/* On dessine chanque élément selon sa classe */
						
						if ( tmp->genre == GUERRIER){
								MLV_draw_filled_circle( 28 + bonus, 75 + nusbo ,10, MLV_COLOR_RED);
						}
						else if ( tmp->genre == CHATEAU){
							
								if(tmp->produit != '/'){
									MLV_draw_filled_circle( 30 + bonus, 43 + nusbo ,15, MLV_COLOR_GREY);
								}
								MLV_draw_filled_rectangle( 22 + bonus, 43 + nusbo ,16,16, MLV_COLOR_RED);
								MLV_draw_filled_rectangle( 15 + bonus, 40 + nusbo ,10,10, MLV_COLOR_RED);
								MLV_draw_filled_rectangle( 34 + bonus, 40 + nusbo ,10,10, MLV_COLOR_RED);
								
								MLV_draw_filled_rectangle( 15 + bonus, 36 + nusbo ,4,4, MLV_COLOR_RED);
								MLV_draw_filled_rectangle( 21 + bonus, 36 + nusbo ,4,4, MLV_COLOR_RED);
									
								MLV_draw_filled_rectangle( 34 + bonus, 36 + nusbo ,4,4, MLV_COLOR_RED);
								MLV_draw_filled_rectangle( 40 + bonus, 36 + nusbo ,4,4, MLV_COLOR_RED);
							}
						else if(tmp->genre == BARON){
							MLV_draw_filled_rectangle( 42 + bonus, 38 + nusbo ,20,20, MLV_COLOR_RED);
							MLV_draw_filled_circle( 51 + bonus , 45 + nusbo ,5, MLV_COLOR_GREEN);
							}
						else if(tmp->genre == MANANT){
							if(tmp->destx == -2 && tmp->desty == -2){
								MLV_draw_filled_circle( 60 + bonus, 71 + nusbo, 17, MLV_COLOR_YELLOW);
							}
							
							MLV_draw_filled_rectangle( 46 + bonus, 63 + nusbo ,20,20, MLV_COLOR_RED);
							MLV_draw_filled_circle( 55 + bonus, 71 + nusbo ,6, MLV_COLOR_BROWN);
									
							}
								
						
						}
						tmp = tmp->asuiv;
							
				}
				tmp = pointer;
				
				while (tmp2 !=NULL){
						
						if (tmp2->posx == i && tmp2->posy ==j){
							
							if(tmp2->genre == CHATEAU){
								if(tmp2->produit != '/'){
									MLV_draw_filled_circle( 30 + bonus, 43 + nusbo ,15, MLV_COLOR_GREY);
								}
								MLV_draw_filled_rectangle( 22 + bonus, 43 + nusbo ,16,16, MLV_COLOR_BLUE);
								MLV_draw_filled_rectangle( 15 + bonus, 40 + nusbo ,10,10, MLV_COLOR_BLUE);
								MLV_draw_filled_rectangle( 34 + bonus, 40 + nusbo ,10,10, MLV_COLOR_BLUE);
								
								MLV_draw_filled_rectangle( 15 + bonus, 36 + nusbo ,4,4, MLV_COLOR_BLUE);
								MLV_draw_filled_rectangle( 21 + bonus, 36 + nusbo ,4,4, MLV_COLOR_BLUE);
								
								MLV_draw_filled_rectangle( 34 + bonus, 36 + nusbo ,4,4, MLV_COLOR_BLUE);
								MLV_draw_filled_rectangle( 40 + bonus, 36 + nusbo ,4,4, MLV_COLOR_BLUE);
							}
							else if (tmp2->genre == GUERRIER){
								MLV_draw_filled_circle( 30 + bonus, 49 + nusbo ,10, MLV_COLOR_BLUE);
								MLV_draw_filled_circle( 30 + bonus, 49 + nusbo ,5, MLV_COLOR_WHITE);
							}
							else if(tmp2->genre == BARON){
								MLV_draw_filled_rectangle( 42 + bonus, 38 + nusbo ,20,20, MLV_COLOR_BLUE);
								MLV_draw_filled_circle( 51 + bonus, 45 + nusbo ,5, MLV_COLOR_GREEN);
							}
							else if(tmp2->genre == MANANT){
								if(tmp2->destx == -2 && tmp2->desty == -2){
									MLV_draw_filled_circle( 60 + bonus, 71 + nusbo , 17, MLV_COLOR_YELLOW);
								}
							
							MLV_draw_filled_rectangle( 50 + bonus, 63 + nusbo ,20,20, MLV_COLOR_BLUE);
							MLV_draw_filled_circle( 55 + bonus, 71 + nusbo ,6, MLV_COLOR_BROWN);
									
							}
							
						}
						tmp2 = tmp2->asuiv;
					}
					tmp2 = pointeb;	
				
				bonus += 60;
				j+= 1;
			}
			nusbo += 60;
			bonus =0;
			i += 1;
		}
		
		// On vérifie les autres chateaux créer par les barons 
		MLV_actualise_window();
		
		/* On parcours ici toute la liste chainée */
		if(pointer->vsuiv != NULL){
			pointer = pointer -> vsuiv;
			tmp = pointer;
		}
		else {
			tmp = NULL;
			
		}
		if(pointeb->vsuiv != NULL){
			pointeb = pointeb -> vsuiv;
			tmp2 = pointeb;
		}
		else 
			tmp2 = NULL;
		
		if(tmp2 == NULL && tmp == NULL){ /* Si on a fini de parcourir les liste chainée , alors on a fini */
			
			init = -1;
		}
	}
	
	MLV_actualise_window();
	
	// ----- on dessine les boutton -------
}

/* Cette fonction sert juste à décrémenter le temps de production de 1
 * à chaque tour du chateau concerné */
 
void fabrication(Agent *tmp){
	tmp->temps -= 1;
}

/* -------------------- Noyaux du programme  ---------------- */

int jeu(Monde *world){
	/* Il ya énormément de pointeur ici car on va faire de nombreuses
	 * opération avec les liste chainée puisque on ne peux pas supprimer les chateaux dans 
	 * les autres fonctions*
	 */
	 
	Agent *tmp,*tmp2,*detache;
	
	Agent *pointeRouge,*pointeBleu;
	Agent *teamRouge,*teamBleu;
	Agent *pointeur1, *pointeur2;	

	Monde info;
	
	Agent *VoisinCase,*VoisinDuVoisin;
	
	teamRouge = world->rouge; /* On pointe sur world->rouge qui pointe ici sur le premier chateau rouge de la liste chainée de l'équipe rouge*/

	teamBleu = world->bleu;/* On pointe sur world->bleu qui pointe ici sur le premier chateau bleu de la liste chainée de l'équipe bleu*/
	
	pointeRouge = teamRouge; 
	
	pointeBleu = teamBleu;
	
	/* On utilisera cette variable pour décider quand la partie se termine ou non */
	int jeuContinue = 0;
	
	
	// init a null//
	int compte = 0 ,compte2 = 0;
	
	/* On initialise toutes les case vers NULLpour éviter les problème de création d'agent
	 * ou de déplacement */
	 
	for(compte = 0;compte < LARG; compte++){
		for(compte2 = 0;compte2 < LONG;compte2++){
			info.plateau[compte2][compte].chateau = NULL;
			info.plateau[compte2][compte].habitant = NULL;
		}
	}
	
	
	
	teamBleu = pointeBleu;
	
	/* On initialise le trésor à 0 */
	info.tresorRouge = 50;
	info.tresorBleu = 50;
	
	info.tour = 0;
	
	MLV_actualise_window();
	
	
	// Le premier à jouer est choisit au hasard
	
	int pileouface = MLV_get_random_integer(0,31); /*On lance pile ou face, j'ai agrandi le nombre de possibilité sinon on tombait presque toujours sur le meme chiffre */
	
	
	// ---------------------------------------

	// ----------------- Va gérer pour chacun le temps avant la fin de production ------------//
	int result = 0;
	
	
	while((world->rouge != NULL && world->bleu != NULL)&& jeuContinue >= 0){
		int caseManant = 0;
		
		/* RedTuen et BlueTurn permettra d'éviter les problèmes de tours qui passait plus vite lorsque 
		 * l'équipe bleu commence */
		 
		int blueTurn = 0,redTurn = 0,ProblemeDeMonney = 0;
		
		MLV_actualise_window();
		
		
		
		if(pileouface%2 == 0){
			
			tmp = teamRouge;
			while (tmp != NULL){
				//ordre = 0;
				MLV_clear_window(MLV_COLOR_BLACK);
				dessineTableau(teamRouge,teamBleu);
				MLV_draw_text(1130,30,"-------Tour numéro %d ------",MLV_COLOR_WHITE,info.tour);
				MLV_draw_text(1130,60, "Tour joueur",MLV_COLOR_WHITE);
				MLV_draw_text(1225,60, "ROUGE",MLV_COLOR_RED);
				MLV_draw_text(1130,80, "Tresor : %d",MLV_COLOR_WHITE,info.tresorRouge);
				
				// ------ Test -------- //
				if (tmp->genre == CHATEAU){
					
						MLV_draw_text(1130, 100, "Tour chateau rouge (%d,%d)",MLV_COLOR_WHITE,tmp->posx,tmp->posy);
						
						/* On fait un do while car on veut permettre au joueur de changer de choi si jamais il essaie
						 * de créer un agent alors qu'il n'a pas l'argent nécessaire */
						 
						do{
							result = listeMenu(tmp,teamRouge,teamBleu);
							
							ProblemeDeMonney = -1;
							
							/* si résult == 666 alors on sort de la boucle et le jeu se termine */
							if (result == 666){
									return 0;
							}
							/* Si le temps de producton est > 0 alors le chateau créer quelque chose et on annonce 
							 * combien de temps il reste avant sa création */
							 
							if( tmp->temps > 0){
								MLV_actualise_window();
								if(tmp->temps > 0){
										MLV_draw_text(1260, 450, "Création dans %d tours",MLV_COLOR_WHITE,tmp->temps -1);
									}
								else{
									MLV_draw_text(1260, 450, "Votre Agent sort du Chateau sur son cheval de feu",MLV_COLOR_WHITE,tmp->temps -1);
								}
							
								MLV_actualise_window();
								MLV_wait_seconds(1);
								
								fabrication(tmp);// On décrémente de -1
							}
							
							/* On va gérer ici la productuin, le trésors débiter et le temps de production
							 * par rapport à la valeur retourner dans le result */
							 
							if(result > 0 && tmp->temps < 0){
								if(result == 6 && info.tresorRouge - 20 >= 0){
									info.tresorRouge -= CBARON;
									tmp->temps = 1;
									
									ProblemeDeMonney = -1;
									
									result = -1;
								}
								else if(result == 4 && info.tresorRouge - 5 >= 0){
									info.tresorRouge -= CGUERRIER;
									tmp->temps = 4;
									
									ProblemeDeMonney = -1;
									
									result = -1;
								}
								else if(result == 2 && info.tresorRouge - 1 >= 0){
									info.tresorRouge -= CMANANT;
									tmp->temps = 2;
									
									ProblemeDeMonney = -1;
									
									result = -1;
								}
								else{
									MLV_draw_text(1180,530,"Votre royaume n'a pas les fonds nécessaire",MLV_COLOR_RED);
									
									ProblemeDeMonney = 0;// On remet ProblèmeDeMonney a 0 pour relancer la fonction et permettre au joueur de rejouer */
									result = 0;
								}
							}
							
						}while(ProblemeDeMonney  == 0 && result != -1);
						
						MLV_actualise_window();
						
						if(tmp->temps == 0){
							execution(&info,tmp->produit,pointeRouge,info.tresorRouge,ROUGE,tmp->posx,tmp->posy);
							tmp->produit = '/';
							tmp->temps = -1;
						}
						
				}
				
				/* Si l'agent jouer est un guerrier */
				if (tmp->genre == GUERRIER){
						MLV_draw_text(1130, 100, "Tour Guerrier Rouge (%d,%d)",MLV_COLOR_WHITE,tmp->posx,tmp->posy);// On affiche sa position comme pour chaque agent 
						
						detectionEnnemiProche(tmp,teamBleu); /* On détecte les ennemis aux alentours */
						
						result = listeMenu(tmp,teamRouge,teamBleu);
					
						/* si résult == 666 alors on sort de la boucle et le jeu se termine */
						if (result == 666){
								return 0;
						}
						
						/* Si produit = c, donc si detectionEnnemi nous annonce qu'il ya un ennemi 
						 * sur la meme case ou l'on se trouve et qu'on a choisit attaque , 
						 * alors on rentre dans cette fonction 
						 * qui va nous pemettre de le ou les attaquers */
						 
						if(tmp->produit =='c' && result == 6){	
							
							jeuContinue = gestionAttaque(&info,tmp,teamBleu,pointeRouge);
							
							/* Si jeuContinue == 90, ça veut dire que le chateau ou pointe world->bleu ( 1er chateau créer est détruit)
							 * alors on pointe vers vsuiv */
							if(jeuContinue == 90){
								teamBleu = teamBleu->vsuiv;
								teamBleu->vprec = NULL;
							}
						}
						
						/* Si notre destination est différente de -1
						 * alors on rentre dans la fonction se déplace 
						 * qui gerera notre déplacement */
						if(tmp->destx > 0 || tmp->desty > 0){
							
							seDeplace(&info,tmp,teamBleu,pointeRouge,result);
						}
						
				}
				
				/* Si on joue le baron */
				
				if (tmp->genre == BARON){
						MLV_draw_text(1130, 100, "Tour Baron rouge (%d,%d)",MLV_COLOR_WHITE,tmp->posx,tmp->posy);
						
						
						//detectionEnnemiProche(tmp,teamBleu);// On recherche les ennemis alentours de notre case
						
						result = listeMenu(tmp,teamRouge,teamBleu);
						
						if (result == 666){
									return 0;
						}
						
						if(tmp->produit =='c' && result == 6){	
							
							
							jeuContinue = gestionAttaque(&info,tmp,teamBleu,pointeRouge);
							
							if(jeuContinue == 90){
								
								teamBleu = teamBleu->vsuiv;
								teamBleu->vprec = NULL;
							}
						}
						
						if((tmp->destx >= 0 || tmp->desty >= 0)){
							
							seDeplace(&info,tmp,teamBleu,pointeRouge,result);
							
							
						}
						
						if(result == 0 && info.tresorRouge - 30 >= 0){
							
							// On retire le baron de la liste chainée pour ensuite créer le chateau //
							
							if(info.plateau[tmp->posx][tmp->posy].chateau == NULL){
								
								info.tresorRouge -= 30;
								pointeur1 = creation(CHATEAU,ROUGE,tmp->posx,tmp->posy);
								
								info.plateau[tmp->posx][tmp->posy].chateau = tmp;
								
								detache = tmp;
								
								tmp = tmp->aprec;
								
								
								if(tmp->asuiv->asuiv != NULL){
									tmp->asuiv = tmp->asuiv->asuiv ;
									tmp = tmp->asuiv;
									tmp->aprec = tmp->aprec->aprec ;
									tmp = tmp->aprec;
								}
								else{
									tmp->asuiv = NULL;
								}
								
								tmp = tmp->asuiv;
								
								detache->asuiv = NULL;
								detache->aprec = pointeur1;
								
								pointeur1->asuiv = detache;
								
								pointeRouge->vsuiv = pointeur1;
								pointeur1->vprec = pointeRouge;
								
								
								
							}
							VoisinCase = teamRouge;
							VoisinDuVoisin = teamRouge;
							
						}
						
				}
				
				if(tmp != NULL){
					if (tmp->genre == MANANT){
							MLV_draw_text(1130, 100, "Tour Manant rouge (%d,%d)",MLV_COLOR_WHITE,tmp->posx,tmp->posy);
							MLV_actualise_window();
							
							caseManant = 0;
							
							result = listeMenu(tmp,teamBleu,teamRouge);
							
							caseManant = ManantSurCase(tmp,teamRouge,teamBleu);
							if (result == 666){
									return 0;
							}
							if (tmp->produit == 't' && caseManant != -1){
						
								tmp->desty = -2;
								tmp->destx = -2;
								info.tresorRouge += 1;
								
							}
							
							if(tmp->destx >= 0 || tmp->desty >= 0){
								
								seDeplace(&info,tmp,teamBleu,pointeRouge,result);
								MLV_actualise_window();
							}
							
							MLV_actualise_window();
							
							//ON vérifie si il ya des agents sur la case aussi si oui on les relie ente eux//
							
							
							VoisinCase = teamRouge;
							VoisinDuVoisin = teamRouge;
							
								
							 while(VoisinCase != NULL){
								
								if(VoisinCase->posx == tmp->posx && VoisinCase->posy == tmp->posy && VoisinCase->genre != CHATEAU){
									
									tmp->vsuiv = VoisinCase;
									VoisinCase->vprec = tmp;
								}
								else{
									
									tmp->vsuiv = NULL;
									VoisinCase->vprec = NULL;
								}
								if(VoisinCase == NULL && VoisinDuVoisin->vsuiv != NULL){
									VoisinCase = VoisinCase->vsuiv;
									VoisinDuVoisin = VoisinDuVoisin->vsuiv;
								}
								VoisinCase = VoisinCase->asuiv;
							}	
					}
				}
				// -------------------- Traitement nombre de tours -------------- //
				if(tmp != NULL)
					tmp = tmp->asuiv;
					
				if(tmp == NULL && pointeRouge->vsuiv != NULL){
					
					pointeRouge = pointeRouge->vsuiv;
					tmp = pointeRouge;
					
					
				}
					
			}
			
		}
		pointeRouge = teamRouge;
		redTurn++;
		
		pileouface = 1;
		
		
		if(jeuContinue < 0){
			MLV_clear_window(MLV_COLOR_BLACK);
			printf("Jeu terminé");
			
			if(jeuContinue == -10){
					MLV_draw_text(830, 400, "Victoire Team Rouge",MLV_COLOR_WHITE);
			}
			if(jeuContinue == -20){
					MLV_draw_text(830, 400, "Victoire Team Bleu",MLV_COLOR_WHITE);
			}
			return 0;
		}
		
		// ------------------ TeamBleu ---------------------//
		
		/* Je ne vais pas détailler ici les comportement et les choix, car ils sont similaire
		 * aux précédents*/
		 
		if(pileouface%2 != 0){
			
			if(world->bleu != NULL){
				tmp2 = teamBleu;
			
			}
			
			if(world->bleu != NULL){
				
				
				while (tmp2 != NULL){
					
					MLV_clear_window(MLV_COLOR_BLACK);
					dessineTableau(teamRouge,teamBleu);
					MLV_draw_text(1130, 30,"-------Tour numéro %d ------",MLV_COLOR_WHITE,info.tour);
					MLV_draw_text(1130, 60, "Tour joueur",MLV_COLOR_WHITE);
					MLV_draw_text(1225,60, "BLEU",MLV_COLOR_BLUE);
					MLV_draw_text(1130, 80, "Tresor : %d",MLV_COLOR_WHITE,info.tresorBleu);
				
					if (tmp2->genre == CHATEAU){
							MLV_draw_text(1130, 100, "Tour chateau bleu (%d,%d)",MLV_COLOR_WHITE,tmp2->posx,tmp2->posy);
							do{
								result = listeMenu(tmp2,teamRouge,teamBleu);
								if (result == 666){
									return 0;
								}
								
								if(tmp2->temps > 0){
									MLV_actualise_window();
									if(tmp2->temps > 0){
										MLV_draw_text(1230, 450, "Création dans %d tours",MLV_COLOR_WHITE,tmp2->temps -1);
									}
									else{
										MLV_draw_text(1260, 450, "Votre Agent sort du Chateau sur son cheval de feu",MLV_COLOR_WHITE,tmp2->temps -1);
									}
									
									MLV_actualise_window();
									MLV_wait_seconds(1);
									
									fabrication(tmp2);
								}
							
								if(result > 0 && tmp2->temps < 0){
									if(result == 6 && info.tresorBleu - 20 >= 0){
										
										info.tresorBleu -= CBARON;
										tmp2->temps = 1;
										
										ProblemeDeMonney = -1;
										
										// Pour sortir de la boucle//
										result = -1;
									}
									else if(result == 4 &&  info.tresorBleu - 5 >= 0){
										
										info.tresorBleu -= CGUERRIER;
										tmp2->temps = 4;
										
										ProblemeDeMonney = -1;
										
										// Pour sortir de la boucle//
										result = -1;
									}
									else if(result == 2 &&  info.tresorBleu - 1 >= 0){
									
										info.tresorBleu -= CMANANT;
										tmp2->temps = 2;
									
										ProblemeDeMonney = -1;
										
										// Pour sortir de la boucle//
										result = -1;
									}
									else{
										MLV_draw_text(1200,400,"Votre royaume n'a pas les fonds nécessaire",MLV_COLOR_RED,info.tour);
										ProblemeDeMonney = 0;
										result = 0;
									}
								}
							}while(ProblemeDeMonney  == 0 && result != -1);
							
							MLV_actualise_window();
							if(tmp2->temps == 0 ){
							
								execution(&info,tmp2->produit,teamBleu,info.tresorBleu,BLEU,tmp2->posx,tmp2->posy);
								tmp2->produit = '/';
								tmp2->temps = -1;
							}
							
					}
					
					if (tmp2->genre == GUERRIER){
							MLV_draw_text(1130, 100, "Tour Guerrier Bleu (%d,%d)",MLV_COLOR_WHITE,tmp2->posx,tmp2->posy);
							
							detectionEnnemiProche(tmp2,teamRouge);
							
							result = listeMenu(tmp2,teamRouge,teamBleu);
							
							if (result == 666){
									return 0;
							}
								
							if(tmp2->produit =='c' && result == 6){	
							
								jeuContinue = gestionAttaque(&info,tmp2,teamRouge,pointeBleu);
								
								if(jeuContinue == 90){
									teamRouge = teamRouge->vsuiv;
									teamRouge->vprec = NULL;
								}
								
							}
							
							if(tmp2->destx >= 0 || tmp2->desty >= 0){
								
								seDeplace(&info,tmp2,teamRouge,pointeBleu,result);
								
							}
							
					}
					
					if (tmp2->genre == BARON){
							MLV_draw_text(1130, 100, "Tour Baron Bleu (%d,%d)",MLV_COLOR_WHITE,tmp2->posx,tmp2->posy);
							
							detectionEnnemiProche(tmp2,teamRouge);
							
							result = listeMenu(tmp2,teamRouge,teamBleu);
							
							if (result == 666){
									return 0;
							}
							
							if(tmp2->produit =='c' && result == 6){	
							
								jeuContinue = gestionAttaque(&info,tmp2,teamRouge,pointeBleu);
								
								if(jeuContinue == 90){
									teamRouge = teamRouge->vsuiv;
									teamRouge->vprec = NULL;
								}
								
							}
							if(tmp2->destx >= 0 || tmp2->desty >= 0 ){
								
								seDeplace(&info,tmp2,teamRouge,pointeBleu,result);
							}
							
							if(result == 0 && info.tresorBleu - 30 >= 0){
								
								// On retire le baron de la liste chainée pour ensuite créer le chateau //
								
								if(info.plateau[tmp2->posx][tmp2->posy].chateau == NULL){
									info.tresorBleu -= 30;
									
									pointeur2 = creation(CHATEAU,BLEU,tmp2->posx,tmp2->posy);
									
									
									info.plateau[tmp2->posx][tmp2->posy].chateau = tmp2;
									
									detache = tmp2;
									
									tmp2 = tmp2->aprec;
									
									
									if(tmp2->asuiv->asuiv != NULL){
										tmp2->asuiv = tmp2->asuiv->asuiv ;
										tmp2 = tmp2->asuiv;
										tmp2->aprec = tmp2->aprec->aprec ;
										tmp2 = tmp2->aprec;
									}
									else{
										tmp2->asuiv = NULL;
									}
									
									tmp2 = tmp2->asuiv;
									
									detache->asuiv = NULL;
									detache->aprec = pointeur2;
									
									pointeur2->asuiv = detache;
										
									pointeBleu->vsuiv = pointeur2;
									pointeur2->vprec = pointeBleu;
									
									pointeBleu = teamBleu;
									
								}
								
								VoisinCase = teamBleu;
								VoisinDuVoisin = teamBleu;
								
								}
								
						}
					
					if(tmp2 != NULL){
						if (tmp2->genre == MANANT){
								MLV_draw_text(1130, 100, "Tour Manant Bleu (%d,%d)",MLV_COLOR_WHITE,tmp2->posx,tmp2->posy);
								MLV_actualise_window();
								
								
								
								result = listeMenu(tmp2,teamRouge,teamBleu);
								
								caseManant = ManantSurCase(tmp2,teamBleu,teamRouge);
								if (result==666){
									return 0;
								}
								if (tmp2->produit == 't' && caseManant != -1){
									
									tmp2->desty = -2;
									tmp2->destx = -2;
									info.tresorBleu += 1;
									
								}
								
								if(tmp2->destx >= 0 || tmp2->desty >= 0){
									
									seDeplace(&info,tmp2,teamRouge,pointeBleu,result);
									MLV_actualise_window();
								}
								
								MLV_actualise_window();
						}
					}
					
					
					// --- Traitement du timer de chacune des action ----//
					
					if(tmp2 != NULL)
						tmp2 = tmp2->asuiv;
						
					if(tmp2 == NULL && pointeBleu->vsuiv != NULL){
						
						pointeBleu = pointeBleu->vsuiv;
						tmp2 = pointeBleu;
					}
				}
			}
			if(world->bleu != NULL)
				pointeBleu = teamBleu;
				
			blueTurn++;
			pileouface = 0;
			
			
		if(jeuContinue < 0){
			MLV_clear_window(MLV_COLOR_BLACK);
			printf("Jeu terminé");
			
			if(jeuContinue == -10){
					MLV_draw_text(830, 400, "Victoire Team Rouge",MLV_COLOR_WHITE);
			}
			if(jeuContinue == -20){
					MLV_draw_text(830, 400, "Victoire Team Bleu",MLV_COLOR_WHITE);
			}
			return 0;
		}
			
		}
		if (redTurn == blueTurn){ /* ON ajoute un tour seulement si les rouges et les bleus ont joués*/
			info.tour += 1;
		}
		
		
	}
	
return 1;	
}
int chargerPartie(){
	int x,y,choix = 0;
	
	while (choix == 0){
		MLV_draw_filled_rectangle( 400, 620 , 160, 40, MLV_COLOR_ORANGE);
		MLV_draw_text(430, 630, "Charger Partie",MLV_COLOR_WHITE);
		
		MLV_draw_filled_rectangle( 800, 620 , 160, 40, MLV_COLOR_ORANGE);
		MLV_draw_text(826, 630, "Nouvelle Partie",MLV_COLOR_WHITE);
		
		MLV_actualise_window();
		MLV_wait_mouse(&x,&y);
		
		if(( x>= 400 && x <= 588 ) && (y >= 620 && y <= 659)){
			choix = 0;
			return 1;
		}
		
		if(( x>= 801 && x <= 957 ) && (y >= 621 && y <= 658)){
			choix = 0;
			return 2;
		}
	}
	return 0;
}

int main(int argc, char *argv[]){
	
	
	Monde world;
	Agent *tmp2,*tmp3,*pointeCase, *pointeTete;
	
	//On créer les 3 permiers éléments de nos équipes respectives //
	
	
	
	
	// On fait pointer les case du tableau sur les éléments qui sont dessus //
	/* Donc ici tous les éléments créés dans le main */
	
	MLV_create_window( "CLASH OF CLANS", "warrior vs haricots", 1600, 900 );
	
	int chargement = 0;
	
	/*A parte d'ici le joueur aura le choix entre choisir de chargé 
	 * une partie qu'il aura déjà enregistrer ou alors de jouer directement */
	chargement = chargerPartie(); 
	
	/* Si il choisit de charger une partie alors */
	if (chargement == 1){
		
		char genre;
		int posx,posy;
		
		char parcours;
		
		FILE* fichier = NULL;
		
		fichier = fopen("sauvegarde","r");
		
		if(fichier == NULL){
			MLV_draw_text(590, 680, "Aucun fichier de sauvegarde",MLV_COLOR_RED);
			MLV_draw_text(590, 700, "Lancement nouvelle partie",MLV_COLOR_GREEN);
			MLV_actualise_window();
			MLV_wait_seconds(2);
			chargement = 2;
		}
		else{
			int saut = -1; 
			
			parcours = fgetc(fichier);
				
			while(parcours != '!'){
				saut = -1;
				
				if(parcours =='/'){
					saut = 1;
					parcours = fgetc(fichier);
					parcours = fgetc(fichier);
				}
				
				genre = parcours;/*Premier caractère */
				
				
				parcours = fgetc(fichier);
				parcours = fgetc(fichier);
				
				posx = parcours - 48;//Chiffre
				
				parcours = fgetc(fichier);
				
				if(parcours >= 48 && parcours <= 57){
					posx = 10 + (parcours - 48);
					parcours = fgetc(fichier);
					
				}
				parcours = fgetc(fichier);
				
				
				posy = parcours - 48;
				
				parcours = fgetc(fichier);
				
				if(parcours >= 48 && parcours <= 57){
					posy = 10 + (parcours - 48);
					parcours = fgetc(fichier);
				}
				
				
				if(saut == 1 && genre == 'c'){
					
					tmp2 = creation(genre,ROUGE,posx,posy);
					
					world.plateau[posx][posy].chateau = tmp2;
					
					pointeTete->vsuiv = tmp2;
					tmp2->vprec = pointeTete;
					pointeTete = tmp2;
					pointeCase = pointeTete;
					
					saut = 0;
				}
				else if(genre == 'c' && saut == -1){
					world.rouge = creation(genre,ROUGE,posx,posy);
					
					world.plateau[posx][posy].chateau = tmp2;
					
					pointeCase = world.rouge;
					pointeTete = world.rouge;
					
				}
				else if(genre != 'c'){
					
					tmp2 = creation(genre,ROUGE,posx,posy);
					
					world.plateau[posx][posy].habitant = tmp2;
					pointeCase->asuiv = tmp2;
					
					tmp2 ->aprec = pointeCase;
					pointeCase = pointeCase->asuiv;
				}
				
				parcours = fgetc(fichier);
			
			}
			
			
			parcours = fgetc(fichier);
			parcours = fgetc(fichier);
			
			while(parcours != EOF){
				saut = -1;
				
				if(parcours =='/'){
					saut = 1;
					parcours = fgetc(fichier);
					parcours = fgetc(fichier);
				}
				
				genre = parcours;/*Premier caractère */
				
				
				parcours = fgetc(fichier);
				parcours = fgetc(fichier);
				
				posx = parcours - 48;//Chiffre
				
				parcours = fgetc(fichier);
				
				if(parcours >= 48 && parcours <= 57){
					posx = 10 + (parcours - 48);
					parcours = fgetc(fichier);
					
				}
				parcours = fgetc(fichier);
				
				
				posy = parcours - 48;
				
				parcours = fgetc(fichier);
				
				if(parcours >= 48 && parcours <= 57){
					posy = 10 + (parcours - 48);
					parcours = fgetc(fichier);
				}
				
				
				if(saut == 1 && genre == 'c'){
					
					tmp2 = creation(genre,BLEU,posx,posy);
					
					world.plateau[posx][posy].chateau = tmp2;
					
					pointeTete->vsuiv = tmp2;
					tmp2->vprec = pointeTete;
					pointeTete = tmp2;
					pointeCase = pointeTete;
					
					saut = 0;
				}
				else if(genre == 'c' && saut == -1){
					world.bleu = creation(genre,BLEU,posx,posy);
					
					world.plateau[posx][posy].chateau = tmp2;
					pointeCase = world.bleu;
					pointeTete = world.bleu;
					
				}
				else if(genre != 'c'){
					tmp2 = creation(genre,BLEU,posx,posy);
					
					world.plateau[posx][posy].habitant = tmp2;
					pointeCase->asuiv = tmp2;
					
					tmp2 ->aprec = pointeCase;
					pointeCase = pointeCase->asuiv;
				}
				parcours = fgetc(fichier);
			}
		
		}
	}

	
	/*Dans le cas d'une nouvelle partie on charge directement les élément*/
	if (chargement == 2){ 
			world.rouge = creation(CHATEAU,ROUGE,0,0);
			
			world.plateau[0][0].chateau = world.rouge;
			
			pointeCase = world.rouge;
			
			tmp2 = creation(MANANT,ROUGE,1,0);
			
			world.plateau[1][0].habitant = tmp2;
			
			world.rouge->asuiv = tmp2;
			tmp2->aprec = world.rouge;
			
			world.rouge = world.rouge->asuiv;
			
			
			
			tmp3 = creation(BARON,ROUGE,0,1);
			
			world.plateau[0][1].habitant = tmp3;
			
			world.rouge->asuiv = tmp3;
			tmp3->aprec = world.rouge;
			
			world.rouge = pointeCase;
			
			
			// ------------- On créer les bleus ---------------//
			
			world.bleu = creation(CHATEAU,BLEU,11,17);
			
			world.plateau[11][17].habitant = world.bleu;
			
			pointeCase = world.bleu;
			
			tmp2 = creation(MANANT,BLEU,11,16);
			
			world.plateau[11][16].habitant = world.bleu;
			
			world.bleu->asuiv = tmp2;
			tmp2->aprec = world.bleu;
			
			world.bleu = world.bleu->asuiv;
			
			
			
			tmp3 = creation(GUERRIER,BLEU,10,17);
			
			world.plateau[10][17].habitant = world.bleu;
			
			world.bleu->asuiv = tmp3;
			tmp3->aprec = world.bleu;
			
			world.bleu = pointeCase;
			
	}
	
	jeu(&world);
	MLV_actualise_window();

	
	
	MLV_wait_seconds( 1 );

	
	MLV_free_window();

	return 0;
}
	

