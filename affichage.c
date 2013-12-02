#include "fonction.h"

//fonction d'initialisation de la carte
bool init_carte(int terrain [TAILLE_TERRAIN_HAUTEUR][TAILLE_TERRAIN_LARGEUR], int sousTerrain [TAILLE_TERRAIN_HAUTEUR][TAILLE_TERRAIN_LARGEUR], char tutoriel[NB_OBJET][TAILLE_MAX_TABLEAU_TUTORIEL])
{
	//ouverture du fichier
	FILE *fichierTerrain = NULL;
	FILE *fichierTutoriel = NULL;
	fichierTerrain = fopen("terrain.charpenti", "r");
	fichierTutoriel = fopen("tutoriel.charpenti", "r");

	//initialisation des variables
	int i = 0, j = 0;
	char caseActuelle = '\0', tutoActuel = '\0';

	//si le fichie existe et est ouvrable
	if ((fichierTerrain != NULL) && (fichierTutoriel != NULL))
	{
		//CREATION DU TERRAIN

		//On lit un caractère
		caseActuelle = fgetc(fichierTerrain);

		//si ce caractère n'est pas la fin du fichier
		while(caseActuelle != EOF)
		{
			//si ce caractère est un retour à la ligne
			if (caseActuelle == '\n')
			{
				//on change de ligne et on remet les colonnes à zéro
				i++;
				j = 0;
			}
			//sinon, c'est un caractère du terrain
			else
			{
				//On le lit et on lui attribut sa valeur numérique
				terrain[i][j] = (caseActuelle - 48);
				sousTerrain[i][j] = HERBE;
				//On passe à la colonne suivante
				j++;

			}
			//On lit le prochain caractère du fichier
			caseActuelle = fgetc(fichierTerrain);
		}

		//CREATION DU TUTORIEL

		tutoActuel = fgetc(fichierTutoriel);
		printf("%c\n", tutoActuel);

		i = 0;
		j = 0;
		while(tutoActuel != EOF)
		{
			if (tutoActuel != '\n')
			{
				tutoriel[i][j] = tutoActuel;
				j++;
			}
			else
			{
				i++;
				j = 0;
			}
			tutoActuel = fgetc(fichierTutoriel);
		}

	}
	//Si les fichier ne peut être lus
	else{
		//On affiche l'erreur
		printf("Fichier(s) introuvable(s) ou protégé(s)\n");
		//on retourne que le jeu ne peut continuer
		return FIN;
	}

	//si tout se passe bien, on retourne que le jeu peut continuer
	return true;

}



//fonction d'affichage de la carte
void affiche_carte(int terrain[TAILLE_TERRAIN_HAUTEUR][TAILLE_TERRAIN_LARGEUR], int positionPerso[2][2], int compteurPO[2], int vies[2], int clefs[2], char tutoriel[NB_OBJET][TAILLE_MAX_TABLEAU_TUTORIEL]){
	//initialisation des variables
	int couleurs[10];
	int typeCL = 0;
	int i = 0, j = 0;

	//on associe chaque objet à une couleur
	remplirCouleur(couleurs);

	//On efface la console
	system("clear");

	//On affiche le nombre de vie(s), de pièce(s) et de clef(s)
	printf(" Vous   : Vie%s: %d - Pièce%s: %d - Clé%s: %d\n", vies[0] <= 1 ? " " : "s ", vies[0], compteurPO[0] <= 1 ? " " : "s ", compteurPO[0], clefs[0] <= 1 ? " " : "s ", clefs[0]); //Merci Julien pour les opérateurs ternaires
	printf(" Lui : Vie%s: %d - Pièce%s: %d - Clé%s: %d\n", vies[1] <= 1 ? " " : "s ", vies[1], compteurPO[1] <= 1 ? " " : "s ", compteurPO[1], clefs[1] <= 1 ? " " : "s ", clefs[1]);

	//Boucles for imbriquées pour lire le terrain
	for (i = 0; i < TAILLE_TERRAIN_HAUTEUR; i++)
	{
		for (j = 0; j < TAILLE_TERRAIN_LARGEUR; j++)
		{
			//Si la position correspond à celle du joueur
			if ((positionPerso[0][0] == i) && (positionPerso[0][1] == j))
			{
				//On affiche le joueur avec sa couleur correspondante
				printf(" \033[0;%dmX\033[0;37m", PERSOCL); //Merci (encore) Julien pour l'idée des couleurs
			}
			else if ((positionPerso[1][0] == i) && (positionPerso[1][1] == j))
			{
				//On affiche le joueur avec sa couleur correspondante
				printf(" \033[0;%dmX\033[0;37m", MONSTRECL); //Merci (encore) Julien pour l'idée des couleurs
			}
			//Si ce n'est pas la position du joueur
			else
			{	
				//On cherche la couleur correspondante à l'objet de la case et on affiche cet objet avec cette couleur
				typeCL = couleurs[terrain[i][j]] / 100;
				printf(" \033[%d;%dm%d\033[0;37m", typeCL, couleurs[terrain[i][j]] - (100*typeCL), terrain[i][j]);
			}
		}
		//On retourne à la ligne et on affiche le tutoriel
		if ((HAUTEUR_TUTORIEL <= i) && (i <= (NB_OBJET + HAUTEUR_TUTORIEL)))
		{
			printf("\t%s\n", tutoriel[i - HAUTEUR_TUTORIEL]);
		}
		else
		{
			printf("\n");
		}
	}
}



//fonction d'association des couleurs aux objets
void remplirCouleur(int couleurs[10])
{
	//On associe chaque objet à sa couleur
	couleurs[HERBE] = HERBECL; couleurs[FLEUR] = FLEURCL; couleurs[ARBRE] = ARBRECL;
	couleurs[ROCHER] = ROCHERCL; couleurs[CLEF] = CLEFCL; couleurs[PO] = POCL;
	couleurs[CADENAS] = CADENASCL; couleurs[PIEGE] = PIEGECL; couleurs[MONSTRE] = MONSTRECL;
	couleurs[MONSTREEN] = MONSTREENCL;
}

//fonction d'affichage du résultat de la partie
void fin(int compteurPO, int testFin)
{
	//Si le joueur n'a pas atteint les dix pièces d'or et n'a pas arrété le jeu
	if ((compteurPO < PIECES_FINALES) && (testFin != FIN))
	{
		//Il n'avait plus de vie, il a perdu
		printf("Tu as perdu avec %d pièces d'or.\n", compteurPO);
	}
	//Sinon, si le joueur a atteint les dix pièces d'or
	else if (compteurPO >= PIECES_FINALES)
	{
		//Il a gagné
		printf("Tu as gagné !\n");
	}
	//Sinon, il a arrété le jeu
	else
	{
		printf("Vous avez arreté le jeu\n");
	}
}
