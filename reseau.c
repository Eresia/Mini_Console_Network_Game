#include "fonction.h"

void donner_position_serveur(int correspondant, int positionPerso[2])
{
	char positionChar[100] = "";

	write(correspondant, "true", 100);

	bzero (positionChar, 100) ;
	sprintf(positionChar, "%d", positionPerso[0]);
	write(correspondant, positionChar, 100);
	bzero (positionChar, 100) ;
	sprintf(positionChar, "%d", positionPerso[1]);
	write(correspondant, positionChar, 100);
}

bool recevoir_position_serveur(int correspondant, int positionPerso[2])
{
	char positionChar[100] = "";
	char testFinChar[100] = "true";

	read(correspondant, testFinChar, 100);
	if (!(strcmp(testFinChar, "true")))
	{
		bzero (positionChar, 100) ;
		read(correspondant, positionChar, 100);
		positionPerso[0] = atoi(positionChar);
		bzero (positionChar, 100) ;
		read(correspondant, positionChar, 100);
		positionPerso[1] = atoi(positionChar);
		return true;
	}
	else
	{
		return false;
	}
}