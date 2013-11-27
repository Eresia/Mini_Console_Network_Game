#include "fonction.h"

//fonction de déplacement du personnage
bool deplace_personnage(int terrain[TAILLE_TERRAIN_HAUTEUR][TAILLE_TERRAIN_LARGEUR], int positionPerso[2], int *compteurPO, int *vies, int *clefs, char message[TAILLE_MAX_TABLEAU_AFFICHE])
{
	//Initialisation des variables
	char direction = '\0';
	int directionValide = false;
	char messageModif[TAILLE_MAX_TABLEAU_AFFICHE] = "";

	//Initialisation de la boucle de vérification de direction
	do
	{
		//On affiche les informations
		printf("%s", message);
		//On réinitialise les informations
		sprintf(message, "Dans quel direction voulez vous aller ? : ");
		//On demande au joueur de choisir une direction
		scanf("%s", &direction);

		if (direction <= 58)
		{
			switch (direction)
			{
				//Si la direction correspond à HAUT
				case '8':
					//On essaye de déplacer le joueur en haut
					directionValide = deplacement(terrain, positionPerso,-1, 0, false);
					break;
				//Sinon si la direction correspond à BAS
				case '2':
					//On essaye de déplacer le joueur en bas
					directionValide = deplacement(terrain, positionPerso ,1, 0, false);
					break;
				//Sinon si la direction correspond à DROITE
				case '6':
					//On essaye de déplacer le joueur à droite
					directionValide = deplacement(terrain, positionPerso, 0, 1, false);
					break;
				//Sinon si la direction correspond à GAUCHE
				case '4':
					//On essaye de déplacer le joueur en gauche
					directionValide = deplacement(terrain, positionPerso, 0, -1, false);
					break;
				//Sinon si la direction correspond à QUIT
				case '0':
					//On arrête le jeu
					return FIN;
					break;
				default:
					//On affiche au prochain tour "Mauvaise direction"
					sprintf(messageModif, "Mauvaise direction.\n%s", message);
					strcpy(message, messageModif);
					//On passe le tour
					directionValide = true;
					break;
			}
		}
		else
		{

			if (direction < 97)
			{
				direction += 32;
			}
			switch (direction)
			{
				//Si la direction correspond à HAUT
				case 'z':
					//On essaye de déplacer le joueur en haut
					directionValide = deplacement(terrain, positionPerso,-1, 0, false);
					break;
				//Sinon si la direction correspond à BAS
				case 's':
					//On essaye de déplacer le joueur en bas
					directionValide = deplacement(terrain, positionPerso ,1, 0, false);
					break;
				//Sinon si la direction correspond à DROITE
				case 'd':
					//On essaye de déplacer le joueur à droite
					directionValide = deplacement(terrain, positionPerso, 0, 1, false);
					break;
				//Sinon si la direction correspond à GAUCHE
				case 'q':
					//On essaye de déplacer le joueur en gauche
					directionValide = deplacement(terrain, positionPerso, 0, -1, false);
					break;
				//Sinon si la direction correspond à QUIT
				case 'o':
					//On arrête le jeu
					return FIN;
					break;
				default:
					//On affiche au prochain tour "Mauvaise direction"
					sprintf(messageModif, "Mauvaise direction.\n%s", message);
					strcpy(message, messageModif);
					//On passe le tour
					directionValide = true;
					break;
			}
		}

		//si le joueur a essayé d'aller dans un obstacle
		if (directionValide == false)
		{
			//On affiche au prochain tour Il y a un obstacle devant vous"
			sprintf(messageModif, "Il y a un obstacle devant vous !\n%s", message);
			strcpy(message, messageModif);
			//On passe le tour
			directionValide = true;
		}
		
	//On refait la boucle tant que la direction n'est pas valable
	}while(directionValide == false);

	
	return application_terrain(terrain, positionPerso, compteurPO, vies, clefs, message);
}

//fonction de déplacement d'un objet/personnage
bool deplacement(int terrain[TAILLE_TERRAIN_HAUTEUR][TAILLE_TERRAIN_LARGEUR], int position[2] , int x, int y, int importEnnemi)
{
	//Si le déplacement ne fait sortir du terrain le personnage
	if (((position[0] + x) >= 0) && ((position[0] + x) <= (TAILLE_TERRAIN_HAUTEUR -1)) && ((position[1] + y) >= 0) && ((position[1] + y) <= (TAILLE_TERRAIN_LARGEUR - 1)))
	{
		//Si le déplacement ne mène pas vers un obstacle (ou un autre ennemi dans le cas d'un ennemi)
		if ((terrain[position[0] + x][position[1] + y] != 2) && (terrain[position[0] + x][position[1] + y] != 3) && (terrain[position[0] + x][position[1] + y] != (MONSTRE + 1)) && ((terrain[position[0] + x][position[1] + y] != 8) || (importEnnemi == false)))
		{
			//On modifie la position du personnage
			position[0] += x;
			position[1] += y;
			//On retourne que le déplacement à réussi
			return true;
		}
	}
	//On retourne que le déplacement à échoué
	return false;
}

//fonction de déplacement des ennemis
bool deplace_ennemi(int terrain[TAILLE_TERRAIN_HAUTEUR][TAILLE_TERRAIN_LARGEUR], int sousTerrain[TAILLE_TERRAIN_HAUTEUR][TAILLE_TERRAIN_LARGEUR], int positionPerso[2], int *vies, char message[TAILLE_MAX_TABLEAU_AFFICHE])
{
	int i = 0, j = 0;
	int xDiff = 0, yDiff = 0;
	int x = 0, y = 0;
	int directionInterdite = 0;
	int directionValide = false;
	int positionEnnemi[2] = {0, 0};
	char messageModif[TAILLE_MAX_TABLEAU_AFFICHE] = "";
	for (i = 0; i < TAILLE_TERRAIN_HAUTEUR; i++)
	{
		for (j = 0; j < TAILLE_TERRAIN_LARGEUR; j++)
		{
			if (terrain[i][j] == MONSTRE)
			{
				positionEnnemi[0] = i;
				positionEnnemi[1] = j;
				terrain[positionEnnemi[0]][positionEnnemi[1]] = sousTerrain[positionEnnemi[0]][positionEnnemi[1]];
				xDiff = positionPerso[0] - positionEnnemi[0];
				x = xDiff / fabs(xDiff);
				yDiff = positionPerso[1] - positionEnnemi[1];
				y = yDiff / fabs(yDiff);
				while(!directionValide)
				{
					//Si la position en x de l'ennemi est plus aventageuse
					if ((directionInterdite == 0) && (fabs(xDiff) >= fabs(yDiff)))
					{
						//On essaye de le déplacer en x vers le joueur
						directionValide = deplacement(terrain, positionEnnemi, x, 0, true);
					}
					//Sinon, on essaye de le déplacer vers le joueur en y
					else if (directionInterdite == 1)
					{
						directionValide = deplacement(terrain, positionEnnemi, 0, y, true);
					}
					//Sinon, on essaye de le déplacer dans la position la plus aventageuse
					else if ((directionInterdite == 2) && (fabs(xDiff) >= fabs(yDiff)))
					{
						directionValide = deplacement(terrain, positionEnnemi, -x, 0, true);
					}
					else if (directionInterdite == 3)
					{
						directionValide = deplacement(terrain, positionEnnemi, 0, -y, true);
					}
					//Si il ne peut pas bouger, il reste immmobile
					else if (directionInterdite >= 4)
					{
						directionValide = true;
					}
					directionInterdite++;
				}
				sousTerrain[positionEnnemi[0]][positionEnnemi[1]] = terrain[positionEnnemi[0]][positionEnnemi[1]];
				//Si l'ennemi rencontre le joueur
				if ((positionEnnemi[0] == positionPerso[0]) && (positionEnnemi[1] == positionPerso[1]))
				{
					//On décrémente son nombre de vie
					*vies -= 1;
					//On affiche au prochain tour qu'il a perdu une vie
					sprintf(messageModif, "Vous avez perdu une vie mais l'ennemi est mort\n%s", message);
					strcpy(message, messageModif);

					//S'il n'a plus de vie
					if (*vies == 0)
					{
						//C'est la fin du jeu
						return false;
					}
				}
				//Si l'ennemi n'a pas rencontré de joueur
				else
				{
					//On le place sur le terrain
					terrain[positionEnnemi[0]][positionEnnemi[1]] = MONSTRE + 1;
				}	
				directionInterdite = 0;
				directionValide = false;

			}
		}
	}

	for (i = 0; i < TAILLE_TERRAIN_HAUTEUR; i++)
	{
		for (j = 0; j < TAILLE_TERRAIN_LARGEUR; j++)
		{
			if (terrain[i][j] == (MONSTRE + 1))
			{
				terrain[i][j] = MONSTRE;
			}
		}
	}
	return 1;
}

bool application_terrain(int terrain[TAILLE_TERRAIN_HAUTEUR][TAILLE_TERRAIN_LARGEUR], int positionPerso[2], int *compteurPO, int *vies, int *clefs, char message[TAILLE_MAX_TABLEAU_AFFICHE])
{
	char messageModif[TAILLE_MAX_TABLEAU_AFFICHE] = "";
	//Si le joueur a marché sur une pièce d'or
	if (terrain[positionPerso[0]][positionPerso[1]] == PO)
	{
		//on incrémente son nombre de pièce
		*compteurPO += 1;
		//On enlève la pièce d'or du terrain
		terrain[positionPerso[0]][positionPerso[1]] = HERBE;
		//Si le joueur a ses dix pièces d'or
		if (*compteurPO == 10)
		{
			//C'est la fin du jeu
			return false;
		}
		sprintf(messageModif, "Vous gagnez une pièce d'or\n%s", message);
		strcpy(message, messageModif);
	}
	//Sinon, si le joueur à marché sur le une clef
	else if (terrain[positionPerso[0]][positionPerso[1]] == CLEF)
	{
		//on incrémente son nombre de clefs
		*clefs += 1;
		//On enlève la clé du terrain
		terrain[positionPerso[0]][positionPerso[1]] = HERBE;
		sprintf(messageModif, "Vous ramassez une clé\n%s", message);
		strcpy(message, messageModif);
	}
	//Sinon, si le joueur marche sur un cadenas
	else if (terrain[positionPerso[0]][positionPerso[1]] == CADENAS)
	{
		//Si le joueur a une ou plusieurs clefs
		if (*clefs >= 1)
		{
			//on décrémente son nombre de clefs
			*clefs -= 1;
			//On enlève le coffre du terrain
			terrain[positionPerso[0]][positionPerso[1]] = HERBE;
			//On augmente le nombre de pièces d'or
			*compteurPO += 1;
			//Si le joueur a ses dix pièces d'or
			if (*compteurPO == 10)
			{
				//C'est la fin du jeu
				return false;
			}
			sprintf(messageModif, "Vous ouvrez le coffre et trouver une pièce d'or.\n%s", message);
			strcpy(message, messageModif);
		}
		else
		{
			sprintf(messageModif, "Vous trouvez un coffre mais vous ne pouvez pas l'ouvrir.\n%s", message);
			strcpy(message, messageModif);
		}
	}
	//Sinon, si le joueur marche sur un piège
	else if (terrain[positionPerso[0]][positionPerso[1]] == PIEGE)
	{
		//On décrémente son nombre de vie
		*vies -= 1;
		//On affiche au prochain tour qu'il a perdu une vie
		sprintf(messageModif, "Vous avez perdu une vie\n%s", message);
		strcpy(message, messageModif);
		//S'il n'a plus de vie
		if (*vies == 0)
		{
			//C'est la fin du jeu
			return false;
		}
	}
	//Sinon, si le joueur rencontre un ennemi
	else if (terrain[positionPerso[0]][positionPerso[1]] == MONSTRE)
	{
		//On décrémente son nombre de vie
		*vies -= 1;
		//On affiche au prochain tour qu'il a perdu une vie
		sprintf(messageModif, "Vous avez perdu une vie mais l'ennemi est mort\n%s", message);
		strcpy(message, messageModif);
		//On enlève l'ennemi du terrain
		terrain[positionPerso[0]][positionPerso[1]] = HERBE;
		//S'il n'a plus de vie
		if (*vies == 0)
		{
			//C'est la fin du jeu
			return false;
		}
	}

	return true;
}