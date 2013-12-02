#include "fonction.h"

int main(int argc, char const *argv[])
{
	int s_ecoute, s_dial, cli_len ;
	struct sockaddr_in serv_addr, cli_addr ;
	int so_reuseaddr = 1 ;
	//initialisation des variables
	int testFin = 1;
	int terrain[TAILLE_TERRAIN_HAUTEUR][TAILLE_TERRAIN_LARGEUR];
	int sousTerrain[TAILLE_TERRAIN_HAUTEUR][TAILLE_TERRAIN_LARGEUR];
	int positionPerso[2][2];
	int compteurPO[2] = {0,0};
	int vies[2] = {10,10};
	int clefs[2] = {0, 0};
	int tour = 0;

	positionPerso[0][0] = 19;
	positionPerso[0][1] = 19;
	positionPerso[1][0] = 0;
	positionPerso[1][1] = 0;

	char tutoriel[NB_OBJET][TAILLE_MAX_TABLEAU_TUTORIEL] = {"\0"};
	char message[TAILLE_MAX_TABLEAU_AFFICHE] = "Dans quel direction voulez vous aller ? : ";
	char messageEnnemi[TAILLE_MAX_TABLEAU_AFFICHE] = "Dans quel direction voulez vous aller ? : ";


	/* on dÃ©signe l'adresse+port qu'on va ouvrir */
	  serv_addr.sin_family = AF_INET ;
	  serv_addr.sin_addr.s_addr = htonl (INADDR_ANY) ; /* on attend sur toutes nos adresses */
	  serv_addr.sin_port = htons (5000) ; /* on attend sur le port 5000 */
	  memset (&serv_addr.sin_zero, 0, sizeof(serv_addr.sin_zero));

	  /* on crÃ©e la socket d'ecoute et on l'associe au couple (adresse,port) defini ci-dessus */
	  s_ecoute = socket (PF_INET, SOCK_STREAM, 0) ;
	  setsockopt(s_ecoute, SOL_SOCKET,SO_REUSEADDR,&so_reuseaddr, sizeof so_reuseaddr);

	  bind (s_ecoute, (struct sockaddr *)&serv_addr, sizeof serv_addr) ;
	  listen (s_ecoute, 5) ; /* on definit la socket s_ecoute, comme etant une socket d'ecoute*/
	  cli_len = sizeof cli_addr ;

	  /* On se met en ecoute sur la socket. C'est une fonction blocante qui ne
	     se debloque que lorsqu'un client vient se connecter sur cette socket 
	     d'ecoute. La valeur de retour est alors le descripteur de la socket 
	     de connexion permettant  de dialoguer avec CE client. */
	  s_dial = accept (s_ecoute, (struct sockaddr *) &cli_addr, (socklen_t *) &cli_len) ;
	
	//initialisation de la carte (renvoie faux si le fichier n'existe pas)
	testFin = init_carte(terrain, sousTerrain, tutoriel);

	//boucle du jeu
	while(testFin == true)
	{
		//on affiche la carte
		affiche_carte(terrain, positionPerso, compteurPO, vies, clefs, tutoriel);	//on affiche le résultat

		//deplacement du personnage (renvoie faux si le personnage a gagné / perdu / arrété)
		testFin = deplace_personnage(terrain, positionPerso[0], &compteurPO[0], &vies[0], &clefs[0], message);
		
		if (testFin && (tour == 1))
		{
			testFin = deplace_ennemi(terrain, sousTerrain, positionPerso[0], &vies[0], message, MONSTREEN);
		}

		if (testFin == false)
		{
			write(s_dial, "false", 100);
		}
		else
		{
			donner_position_serveur(s_dial, positionPerso[0]);
			testFin = recevoir_position_serveur(s_dial, positionPerso[1]);
			application_terrain(terrain, positionPerso[1], &compteurPO[1], &vies[1], &clefs[1], messageEnnemi);
			if (tour == 1)
			{
				deplace_ennemi(terrain, sousTerrain, positionPerso[1], &vies[1], messageEnnemi, MONSTRE);
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
	//on affiche le résultat
	fin(compteurPO[0] + compteurPO[1], testFin);

	return 0;
}