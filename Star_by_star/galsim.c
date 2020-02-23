#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graphics.h"
#include <math.h>

//To run enter for example: time ./galsim 2000 input_data/ellipse_N_02000.gal 1000 0.001 1

typedef struct star{
    double posX, posY, mass, velX, velY, brightness;
} star_t;

//Variables for graphics
const float circleRadius = 0.0015, circleColor = 0;
const int windowWidth = 600, windowHeight = 600, W = 1, H = 1;
const int wait = 0;

const double epsilon = 0.15; //This can be changed for other values, but needs to be 0.15 for compare-files

void move(star_t *stars, int N, double deltaT, double gravConst)
{
    double massesDivRCU, posXi, posXj, posYi, posYj, massi, massj, distX, distY, rEpsilon;
    double massesDivRCUSumX = 0, massesDivRCUSumY = 0;
    star_t *starPoint1 = stars;
    star_t *starPoint2 = stars;
    starPoint2++;    
    const double dTG = deltaT * gravConst;

    for (int i = 0; i < N; i++)
    {
        posXi = (starPoint1)->posX;
        posYi = (starPoint1)->posY;
        massi = (starPoint1)->mass;
        
        for (int j = i + 1; j < N; j++)
        {
            posXj = (starPoint2)->posX;
            posYj = (starPoint2)->posY;
            massj = (starPoint2)->mass;
            distX = posXi-posXj;
            distY = posYi-posYj;
            rEpsilon = sqrt(distX*distX + distY*distY) + epsilon;
            massesDivRCU = massj * massi / (rEpsilon*rEpsilon*rEpsilon);
            massesDivRCUSumX += massesDivRCU * distX;
            massesDivRCUSumY += massesDivRCU * distY;
            massesDivRCU = massesDivRCU * dTG / massj;
            (starPoint2)->velX += distX * massesDivRCU;
            (starPoint2)->velY += distY * massesDivRCU;  
            starPoint2++;  
        }
        (starPoint1)->velX += -(dTG * massesDivRCUSumX) / massi;
        (starPoint1)->velY += -(dTG * massesDivRCUSumY) / massi;
        (starPoint1)->posX += ((starPoint1)->velX) * deltaT;
        (starPoint1)->posY += ((starPoint1)->velY) * deltaT;
        
        massesDivRCUSumX = 0;
        massesDivRCUSumY = 0;

        starPoint1++;
        starPoint2 = starPoint1;
        starPoint2++;  
    }
}

int main(int argc, char const *argv[])
{ 
    if (argc!=6){
        printf("Wrong number of input arguments");
        return -1;
    }
    const int N = atoi(argv[1]);
    char *filename = strdup(argv[2]);
    const int nsteps = atoi(argv[3]);
    const double dT = atof(argv[4]);
    const int graphics = atoi(argv[5]);
    star_t *starArr = (star_t*)malloc(N*sizeof(star_t));

    const double gravConst = 100/(double)(N);

    //Read from file
    FILE *fp;
    fp = fopen(filename, "r");
    if (!fp){                           
      printf("Error: Could not open file for reading\n");  
      return -1;
    } 
    for(int i = 0;i < N;i++){
        star_t *newStar = (star_t*)malloc(sizeof(star_t));
        size_t trash;
        trash = fread(&(newStar->posX),sizeof(double),1,fp);
        trash = fread(&(newStar->posY),sizeof(double),1,fp);
        trash = fread(&(newStar->mass),sizeof(double),1,fp);
        trash = fread(&(newStar->velX),sizeof(double),1,fp);
        trash = fread(&(newStar->velY),sizeof(double),1,fp);
        trash = fread(&(newStar->brightness),sizeof(double),1,fp);
        starArr[i]=*newStar;
        free(newStar);
    }
    fclose(fp);
    free(filename);

    //Graphics
    if (graphics == 1){
        InitializeGraphics("galsim",windowWidth,windowHeight);
        SetCAxes(0,1);
    }
    
    for(int i = 0; i < nsteps; i++){
        move(starArr, N, dT, gravConst);
        
        if (graphics == 1){
            ClearScreen();
            for(int j = 0; j < N; j++){
                DrawCircle(starArr[j].posX, starArr[j].posY, W, H, circleRadius, circleColor);
            }
            Refresh();
            usleep(wait);
        }
    }
    
    //Write to file
    fp = fopen("result.gal","w");
    if (!fp){                           
      printf("Error: Could not open file for Writing\n");  
      return -1;
    } 
    star_t *starPointer = starArr;
    for (int i = 0; i < N; i++){
        fwrite(&(starPointer->posX),sizeof(double),1,fp);
        fwrite(&(starPointer->posY),sizeof(double),1,fp);
        fwrite(&(starPointer->mass),sizeof(double),1,fp);
        fwrite(&(starPointer->velX),sizeof(double),1,fp);
        fwrite(&(starPointer->velY),sizeof(double),1,fp);
        fwrite(&(starPointer->brightness),sizeof(double),1,fp);
        starPointer++;
    }
    fclose(fp);
    free(starArr);
    return 0;
}