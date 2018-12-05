/*------------------------------------------------------*/
/* Prog    : ProgDemoN.c                                */
/* Auteur  :                                            */
/* Date    :                                            */
/* version :                                            */ 
/* langage : C                                          */
/* labo    : DIRO                                       */
/*------------------------------------------------------*/

/*------------------------------------------------*/
/* FICHIERS INCLUS -------------------------------*/
/*------------------------------------------------*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "FonctionDemo5.h"

/*------------------------------------------------*/
/* DEFINITIONS -----------------------------------*/      
/*------------------------------------------------*/
#define NAME_IMG_OUT "B-segmentation"

/*------------------------------------------------*/
/* PROGRAMME PRINCIPAL   -------------------------*/                     
/*------------------------------------------------*/
main(int argc, char *argv[])
 {
  int i,j;
  int seuilMV;
  float mean0,mean1,var0,var1; /* parametres statistiques des deux classes : a estimer  */
  int length,width;

  float** y;  /* image a segmenter */
  float** x;  /* champs d'etiquettes a estimer */
  float*  VectHist;
	
 	if(argc<2){
		printf("Usage :\n\t TpIFT6150-4-B image_a_segmenter\n\n");
		return 0;
	}
	
  /* Ouvrir l'image d'entree */

	/* Lancer l'algorithme des K-Moyennes */

  /* Sauvegarde du champ d'etiquettes */
  
  /* Liberer la memoire des images */

  /*Commande systeme: visualisation de Ingout.pgm*/
  system("display B-segmentation.pgm&"); 
  
  /*retour sans probleme*/ 
  printf("\n C'est fini ... \n\n\n");
  return 0; 	 
}
