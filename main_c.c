#include "fonction.h"

int main(int argc, char const *argv[])
{
	int s_cli ;
  	struct sockaddr_in serv_addr ;
	//initialisation des variables
	int testFin = 1;
	int terrain[TAILLE_TERRAIN_HAUTEUR][TAILLE_TERRAIN_LARGEUR];
	int sousTerrain[TAILLE_TERRAIN_HAUTEUR][TAILLE_TERRAIN_LARGEUR];
	int positionPerso[2][2];
	int compteurPO[2] = {0,0};
	int vies[2] = {10,10};
	int clefs[2] = {0, 0};
	int tour = 0;

	positionPerso[0][0] = 0;
	positionPerso[0][1] = 0;
	positionPerso[1][0] = 19;
	positionPerso[1][1] = 19;

	char tutoriel[NB_OBJET][TAILLE_MAX_TABLEAU_TUTORIEL] = {"\0"};
	char message[TAILLE_MAX_TABLEAU_AFFICHE] = "Dans quel direction voulez vous aller ? : ";
	char messageEnnemi[TAILLE_MAX_TABLEAU_AFFICHE] = "Dans quel direction voulez vous aller ? : ";
	
	s_cli = socket (PF_INET, SOCK_STREAM, 0) ;
	  serv_addr.sin_family = AF_INET ;
	  if ((argc == 2) || (argc == 3))
	  {
	    serv_addr.sin_addr.s_addr = inet_addr (argv[1]);
	  }
	  else
	  {
	    serv_addr.sin_addr.s_addr = inet_addr ("127.0.0.1");
	  }
	  if ((argc == 3))
	  {
	      serv_addr.sin_port = htons (atoi(argv[2])) ;
	  }
	  else
	  {
	      serv_addr.sin_port = htons (5000) ;
	  }
	  memset (&serv_addr.sin_zero, 0, sizeof(serv_addr.sin_zero));
	  connect (s_cli, (struct sockaddr *)&serv_addr, sizeof serv_addr) ;

	//initialisation de la carte (renvoie faux si le fichier n'existe pas)
	testFin = init_carte(terrain, sousTerrain, tutoriel);

	//boucle du jeu
	while(testFin == true)
	{
		//on affiche la carte
		affiche_carte(terrain, positionPerso, compteurPO, vies, clefs, tutoriel);	//on affiche le résultat
		//deplacement du personnage (renvoie faux si le personnage a gagné / perdu / arrété)
		testFin = deplace_personnage(terrain, positionPerso[0], &compteurPO[0], &vies[0], &clefs[0], message);
		
		if (testFin && tour == 1)
		{
			testFin = deplace_ennemi(terrain, sousTerrain, positionPerso[0], &vies[0], message, MONSTRE);
		}

		if (testFin == false)
		{
			write(s_cli, "false", 100);
		}
		else
		{
			donner_position_serveur(s_cli, positionPerso[0]);
			testFin = recevoir_position_serveur(s_cli, positionPerso[1]);
			application_terrain(terrain, positionPerso[1], &compteurPO[1], &vies[1], &clefs[1], messageEnnemi);
			if (tour == 1)
			{
				deplace_ennemi(terrain, sousTerrain, positionPerso[1], &vies[1], messageEnnemi, MONSTREEN);
				tour = 0;
			}
			else
			{
				tour = 1;
			}
		}
		if ((compteurPO[0] + compteurPO[1]) >= PIECES_FINALES)
		{
			testFin = false;
		}

	}
	//On affiche la carte finale
	affiche_carte(terrain, positionPerso, compteurPO, vies, clefs, tutoriel);	//on affiche le résultat
	fin(compteurPO[0] + compteurPO[1], testFin);

	return 0;
}