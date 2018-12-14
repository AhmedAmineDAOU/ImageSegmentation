/*------------------------------------------------------*/
/* Prog    : ProgDemoN.c                                */
/* Auteur  : Ahmed Amine Daou, Wrushabh Warshe          */
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
#define NAME_IMG_OUT "C-segmentation"
/*------------------------------------------------*/
/* PROGRAMME PRINCIPAL   -------------------------*/
/*------------------------------------------------*/


int main(int argc, char *argv[])
{
    

    float mean0, mean1,var0,var1,sum0, sum1, nb_echantillon0, nb_echantillon1,tmp;
    float mean01 , mean11 ;
    int i,length,width,ok =0,stop=0 ,PM=1 , iteration = 0,nb_voisin0, nb_voisin1, classe;
    float U0, U1, EG;
    float** y;  /* image a segmenter */
    float** x;  /* champs d'etiquettes a estimer */
    float*  VectHist;

    if(argc<3){
        printf("Usage :\n\t TpIFT6150-4-C image_a_segmenter alpha\n\n");
        return 0 ;
    }
    float alpha = atof(argv[argc - 1]);

    /* Ouvrir l'image d'entree */
    y = LoadImagePgm(argv[argc - 2], &length, &width);
    x = fmatrix_allocate_2d(length, width);

    /* Lancer l'algorithme des K-Moyennes */


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
            tmp  = mean0 ;
            mean0 = mean1;
            mean1 = tmp;//permuter les valeurs
        }
        else if (mean0 < mean1) ok =1;
    }


    while( stop == 0 ) {//iterer jusqu'a => clusters ne bougent plus
        sum0 = 0 ;
        sum1 = 0 ;
        nb_echantillon0 = 0 ;
        nb_echantillon1 = 0 ;

        for( int  i=0 ; i< (mean1 - mean0) / 2.0 ; i++ ) {
            sum0   += i * VectHist[i];
            nb_echantillon0 += VectHist[i];
        }


        for( int  i= (mean1 - mean0) / 2.0 ; i <= 255 ; i++ ) {
            sum1   += i * VectHist[i];
            nb_echantillon1 += VectHist[i];
        }

        //nouvelles moyennes
        mean01 = sum0 / nb_echantillon0 ;
        mean11 = sum1 / nb_echantillon1;

        printf("%d%s%f %f\n", i, " update means: ", mean01, mean11);

        //on arrete d'iterer si les moyennes ne changents plus
        if (mean01 == mean0 && mean11 == mean1)  stop = 1;//stop

        //ecraser les anciennes moyenne
        mean0 = mean01;
        mean1 = mean11;
        i++;
    }

    for( int   i=0 ; i < length ; i++ )
        for( int   j=0 ; j < width ; j++ ) {
            if ( y[i][j] > (mean1 - mean0) / 2.0 )
            x[i][j] =  255;
            else
            x[i][j] =  0 ;
        }
    //Estimer la variance des deux classe
    for( int i=0 ; i< (mean1 - mean0) / 2.0 ; i++ ) {
        sum0 += VectHist[i] * SQUARE(i - mean0);
        nb_echantillon0 += VectHist[i];
    }

    for( int i=(mean1 - mean0) / 2.0 ; i <= 255 ; i++ ) {
        sum1 += VectHist[i] * SQUARE(i - mean1);
        nb_echantillon1 += VectHist[i];
    }
    var0 = sum0 / nb_echantillon0 ;
    var1 = sum1 / nb_echantillon1;

    /* Lancer l'algorithme ICM */
    float** ln0 = fmatrix_allocate_2d(length, width);//ln(N(y(i,j);mean0 ;var0))
    float** ln1 = fmatrix_allocate_2d(length, width);//ln(N(y(i,j);mean1;var1))


    

    while(stop==1) {
        PM =EG = 0 ;//reinitialisation de pixel odifiees et energie globale
        //balayer l'ensemble des pixels
        for( int  i=0 ; i < length ; i++ ){
            for( int  j=0 ; j < width ; j++ ) {

            ln0[i][j] = - log(sqrt(2 * PI * var0)) -SQUARE(y[i][j]-mean0)/(2 * var0) ;
            ln1[i][j] = - log(sqrt(2 * PI * var1)) -SQUARE(y[i][j]-mean1)/(2 * var1) ;

                nb_voisin0 = 0 ;
                nb_voisin1 = 0 ;

                for( int  k = fmax(0, i-1); k<= fmin( i+1, length - 1 ); k++)
                    for( int  l = fmax(0, j-1); l<= fmin( j+1, width - 1 ); l++) {

                        if( k!=i && j!=l ){
                            if (x[k][l] == 0) nb_voisin0++;
                            else nb_voisin1++;
                    }
                    }
                //on calcule U0 et U1
                U0 = -ln0[i][j] + alpha * nb_voisin1;
                U1 = -ln1[i][j] + alpha * nb_voisin0 ;

                if(U0 > U1) {
                    classe = 1 ;
                    EG += U1 ;
                } 
                else {
                    classe = 0;
                    EG += U0;
                }

                if(x[i][j] != classe) {
                    x[i][j] = classe;
                    PM++;
                }
            }
        }

        iteration++;
        printf("%s%d%s%f%s%d\n","iteration= ",iteration," energie globale E= ",EG," pixels modifies= ", PM);
        if (PM==0) stop=0; //stop si il n'ya pas de pixels modifies
       
    }


    /* Sauvegarde du champ d'etiquettes */
    Recal(x, length, width);
    SaveImagePgm(NAME_IMG_OUT, x, length, width);

    /* Liberer la memoire des images */
    free_fmatrix_2d(ln0);
    free_fmatrix_2d(ln1);
    free_fmatrix_2d(x);
    free_fmatrix_2d(y);
    

    /*Commande systeme: visualisation de Ingout.pgm*/
    system("display C-segmentation.pgm&");

    /*retour sans probleme*/
    printf("\n C'est fini ... \n\n\n");

    return 0 ;
}

