/*------------------------------------------------------*/
/* Prog    : ProgDemoN.c                                */
/* Auteur  : Ahmed Amine DAOU & Wrushabh Warshe         */
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
int main(int argc, char *argv[])
{
    int i=0,j;

    float mean0, mean1; /* parametres statistiques des deux classes : a estimer  */
    int length,width;
    int ok =0;
    int stop = 0;
    //kmeans compteur iterations
    /* Ouvrir l'image d'entree */

    float** y;  /* image a segmenter */
    float** x;  /* champs d'etiquettes a estimer */
    float*  VectHist;
    float tmp;
    y = LoadImagePgm(argv[argc - 1], &length, &width);
    x = fmatrix_allocate_2d(length, width);
    if(argc<2){
     printf("Usage :\n\t TpIFT6150-4-B image_a_segmenter\n\n");
     return 0;
    }



    /* Lancer l'algorithme des K-Moyennes */

    // initialisation des moyennes
    float mean01 = -1, mean11 = -1;
    VectHist = malloc(sizeof(float) * 255);
    compute_histo(y, length, width, VectHist);



    //initialisation des moyennes a deux differentes valeurs
    // de pixels entre 0 et 255
    while(ok==0){
        srand(time(NULL));
        mean0 = randomize() * 255;
        mean1 = randomize() * 255;
        if (mean0 > mean1 ) {//si la moyenne (obtenu with rand) de la classe noire est superieure
            ok = 1;
            tmp  = mean0;
            mean0 = mean1;
            mean1 = tmp;//permuter les valeurs
        }
        else if (mean0 < mean1) ok =1;
    }

    float sum0, sum1, nb_echantillon0, nb_echantillon1;

    while( stop == 0 ) {//iterer jusqu'a => clusters ne bougent plus
        sum0 = 0;
        sum1 = 0;
        nb_echantillon0 = 0;
        nb_echantillon1 = 0;

        for(int i=0 ; i< (mean1 - mean0) / 2.0 ; i++ ) {
            sum0   += i * VectHist[i];
            nb_echantillon0 += VectHist[i];
        }


        for(int i= (mean1 - mean0) / 2.0 ; i <= 255 ; i++ ) {
            sum1   += i * VectHist[i];
            nb_echantillon1 += VectHist[i];
        }

        //nouvelles moyennes
        mean01 = sum0 / nb_echantillon0;
        mean11 = sum1 / nb_echantillon1;

        printf("%d%s%f %f\n", i, " update means: ", mean01, mean11);

        //on arrete d'iterer si les moyennes ne changents plus
        if (mean01 == mean0 && mean11 == mean1)  stop = 1;//stop

        //ecraser les anciennes moyenne
        mean0 = mean01;
        mean1 = mean11;
        i++;
    }

    for( i=0 ; i < length ; i++ )
        for( j=0 ; j < width ; j++ ) {
            if ( y[i][j] > (mean1 - mean0) / 2.0 )
            x[i][j] =  255;
            else
            x[i][j] =  0;
        }

    /* Sauvegarde du champ d'etiquettes */
    SaveImagePgm(NAME_IMG_OUT, x, length, width);

    /* Liberer la memoire des images */
    free_fmatrix_2d(x);
    free_fmatrix_2d(y);

    /*Commande systeme: visualisation de Ingout.pgm*/
    system("display B-segmentation.pgm&");

    /*retour sans probleme*/
    printf("\n C'est fini ... \n\n\n");
    return 0;
}
