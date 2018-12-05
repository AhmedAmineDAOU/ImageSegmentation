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
#define NAME_IMG_OUT "D-segmentation"

/*------------------------------------------------*/
/* PROGRAMME PRINCIPAL   -------------------------*/                     
/*------------------------------------------------*/
main(int argc, char *argv[])
 {
  int i,j;
  float alpha;
  float mean0,mean1,var0,var1; /* parametres statistiques des deux classes : a estimer  */
  int length,width;

  float** y;  /* image a segmenter */
  float** x;  /* champs d'etiquettes a estimer */
  float*  VectHist;
	
 	if(argc<3){
		printf("Usage :\n\t TpIFT6150-4-D image_a_segmenter alpha\n\n");
		return 0;
	}

  /* Ouvrir l'image d'entree */

	/* Lancer l'algorithme des K-Moyennes */

	/* Lancer l'algorithme de Recuit simule */
	
  /* Sauvegarde du champ d'etiquettes */
  
  /* Liberer la memoire des images */

  /*Commande systeme: visualisation de Ingout.pgm*/
  system("display D-segmentation.pgm&"); 
  
  /*retour sans probleme*/ 
  printf("\n C'est fini ... \n\n\n");
	
}

