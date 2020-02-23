#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graphics.h"
#include <math.h>
#include <pthread.h>

// To run enter for example: ./galsim 10000 input_data/ellipse_N_10000.gal 1000 0.0001 0.5 1 4

typedef struct star
{
    double posX, posY, mass, velX, velY;
} star_t;

typedef struct quadTreeNode
{
    struct quadTreeNode *sXsY;
    struct quadTreeNode *sXlY;
    struct quadTreeNode *lXsY;
    struct quadTreeNode *lXlY;
    
    double centerOfMassX;
    double centerOfMassY;
    double mass;
    int starIndex;
} node_t;

typedef struct threadInformation
{
    node_t *root;
    star_t *starArr;
    int starStart, intervalLength;
} tInfo_t;

//global variables 
const float circleRadius = 0.0015, circleColor = 0;
const int windowWidth = 800, windowHeight = 800, W = 1, H = 1;
const int wait = 0; //micro seconds
double thetaMax, dT, gravConst;

const double epsilon = 0.15 ;

void createTree(node_t **node, star_t *stars, int i, double minX, double maxX, double minY, double maxY)
{
    int leaf = 0;
    if(*node == NULL)
    {
        node_t *newNode = (node_t*)calloc(1,sizeof(node_t));
        newNode->sXsY = NULL;
        newNode->sXlY = NULL;
        newNode->lXsY = NULL;
        newNode->lXlY = NULL;
        newNode->starIndex = i;
        newNode->centerOfMassX = stars[i].posX * stars[i].mass; //?
        newNode->centerOfMassY = stars[i].posY * stars[i].mass;
        newNode->mass = stars[i].mass;
        *node = newNode;
        leaf = 1;
    } 
    else if ((*node)->starIndex != -1 && (stars[(*node)->starIndex].posX == stars[i].posX) && (stars[(*node)->starIndex].posY == stars[i].posY)) 
    {
        printf("ERROR: Two stars are occupying the same position\n");
        exit(-1);
    } 
    else if (stars[i].posX < (minX + maxX)/2)
    {
        if (stars[i].posY < (minY + maxY)/2) 
        {
            createTree(&(*node)->sXsY, stars, i, minX, (minX + maxX)/2, minY, (minY + maxY)/2);
        }
        else 
        {
            createTree(&(*node)->sXlY, stars, i, minX, (minX + maxX)/2, (minY + maxY)/2, maxY);
        }
    } 
    else 
    {
        if (stars[i].posY < (minY + maxY)/2) 
        {
            createTree(&(*node)->lXsY, stars, i, (minX + maxX)/2, maxX, minY, (minY + maxY)/2);
        } 
        else 
        {
            createTree(&(*node)->lXlY, stars, i, (minX + maxX)/2, maxX, (minY + maxY)/2, maxY);
        }
    }
    
    if (leaf == 0 && (*node)->starIndex != -1)
    {
        int temp = (*node)->starIndex;
        (*node)->starIndex = -1;
        (*node)->centerOfMassX = 0;
        (*node)->centerOfMassY = 0;
        (*node)->mass = 0;
        createTree(node, stars, temp, minX, maxX, minY, maxY);
    }
    return;
}

void freeTree(node_t *node)
{
    if(node == NULL)
    {
        return;
    }
    freeTree(node->sXsY);
    freeTree(node->sXlY);
    freeTree(node->lXsY);
    freeTree(node->lXlY);
    
    free(node);
}

void calcAttrTree(node_t *node, double *cOMX, double *cOMY, double *m)
{
    if (node == NULL)
    {
        return;
    }
    calcAttrTree(node->sXsY,&(node->centerOfMassX),&(node->centerOfMassY),&(node->mass));
    calcAttrTree(node->sXlY,&(node->centerOfMassX),&(node->centerOfMassY),&(node->mass));
    calcAttrTree(node->lXsY,&(node->centerOfMassX),&(node->centerOfMassY),&(node->mass));
    calcAttrTree(node->lXlY,&(node->centerOfMassX),&(node->centerOfMassY),&(node->mass));
    node->centerOfMassX = node->centerOfMassX/node->mass; //leaves are wrong here
    node->centerOfMassY = node->centerOfMassY/node->mass;
    *cOMX = *cOMX + (node->mass * node->centerOfMassX);
    *cOMY = *cOMY + (node->mass * node->centerOfMassY);
    *m = *m + node->mass;
}

void moveWithTree(node_t * restrict node, star_t * restrict stars, int i, double thetaMax, double width, double dTG){
    if (node == NULL)
    {
        return;
    }
    double distanceX = node->centerOfMassX-stars[i].posX;
    double distanceY = node->centerOfMassY-stars[i].posY;
    double distance = sqrt(distanceX * distanceX + distanceY * distanceY);
    if(distance < 0.00000001)
    {
        return;
    }
    double theta = width / distance;
    if (theta > thetaMax && node->starIndex == -1)
    {
        moveWithTree(node->sXsY, stars, i, thetaMax, width/2, dTG);
        moveWithTree(node->sXlY, stars, i, thetaMax, width/2, dTG);
        moveWithTree(node->lXsY, stars, i, thetaMax, width/2, dTG);
        moveWithTree(node->lXlY, stars, i, thetaMax, width/2, dTG);
    } 
    else 
    {
        distance = distance + epsilon;
        double massesDivRCU = node->mass /(distance*distance*distance);
        stars[i].velX += dTG * massesDivRCU *distanceX;
        stars[i].velY += dTG * massesDivRCU *distanceY;  
    }
}

void *threadHandler(void *tArg)
{
    tInfo_t *myData = (tInfo_t*) tArg;
    int starStart = myData->starStart;
    int starEnd = myData->starStart + myData->intervalLength;
    star_t *starArr = myData->starArr;
    node_t *root = myData->root;
    //printf("Start %d End %d\n",starStart, starEnd);
    for (int i = starStart; i < starEnd; i++)
    {
        moveWithTree(root, starArr, i, thetaMax, 1, dT * gravConst);
        starArr[i].posX += starArr[i].velX * dT;
        starArr[i].posY += starArr[i].velY * dT;
        if (starArr[i].posX < 0 || starArr[i].posX > 1 || starArr[i].posY < 0 || starArr[i].posY > 1)
        {
            printf("Error: At least one star has an invalid position\n");
            exit(-1);
        }
    }
    pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{ 
    if (argc!=8)
    {
        printf("Wrong number of input arguments\n");

        return -1;
    }
    const int N = atoi(argv[1]);
    char *filename = strdup(argv[2]);
    const int nsteps = atoi(argv[3]);
    dT = atof(argv[4]);
    thetaMax = atof(argv[5]);
    const int graphics = atoi(argv[6]);
    const int nThreads = atoi(argv[7]); 

    star_t *starArr = (star_t*)malloc(N*sizeof(star_t));
    pthread_t threads[nThreads];
    tInfo_t tInfoArr[nThreads];
    void* status;
    const int intervalLength = N/nThreads;
    
    double *brightArr = (double*)malloc(N*sizeof(double));

    gravConst = 100/(double)(N);

    //Read from file
    FILE *fp;
    fp = fopen(filename, "r");
    if (!fp)
    {                           
      printf("Error: Could not open file for reading\n");  
      return -1;
    } 
    for(int i = 0;i < N;i++)
    {
        star_t *newStar = (star_t*)malloc(sizeof(star_t));
        size_t trash;
        trash = fread(&(newStar->posX),sizeof(double),1,fp);
        trash = fread(&(newStar->posY),sizeof(double),1,fp);
        trash = fread(&(newStar->mass),sizeof(double),1,fp);
        trash = fread(&(newStar->velX),sizeof(double),1,fp);
        trash = fread(&(newStar->velY),sizeof(double),1,fp);
        trash = fread(&(brightArr[i]),sizeof(double),1,fp);
        starArr[i]=*newStar;
        free(newStar);
    }
    fclose(fp);
    free(filename);

    //Graphics
    if (graphics == 1)
    {
        InitializeGraphics("galsim",windowWidth,windowHeight);
        SetCAxes(0,1);
    }
    
    star_t *starArrNew = (star_t*)malloc(N*sizeof(star_t));
    for(int i = 0; i < nsteps; i++)
    {

        node_t *root = NULL; //A4
        double trash;
        for (int j = 0; j < N; j++)
        {
            createTree(&root,starArr,j,0,1,0,1);
        }
        calcAttrTree(root,&trash,&trash,&trash);
        //for loop threaded
        int residual = N%nThreads;
        for(int j = 0; j < nThreads; j++)
        {
            if(j == 0)
            {
                tInfoArr[j].starStart = 0;
            } 
            else 
            {
                tInfoArr[j].starStart = tInfoArr[j-1].starStart + tInfoArr[j-1].intervalLength;
            }
            tInfoArr[j].intervalLength = intervalLength;
            if (residual > 0)
            {
                tInfoArr[j].intervalLength++;
                residual--;
            }
            tInfoArr[j].root = root;
            tInfoArr[j].starArr = starArr;
            //printf("Start %d IntervalLength %d\n",tInfoArr[j].starStart,tInfoArr[j].intervalLength);
            pthread_create(&threads[j], NULL, threadHandler, (void *)&tInfoArr[j]);
        }
        for(int j = 0; j < nThreads; j++)
        {
	        pthread_join(threads[j], &status);
        }
        freeTree(root);
        if (graphics == 1)
        {
            ClearScreen();
            for(int j = 0; j < N; j++)
            {
                DrawCircle(starArr[j].posX, starArr[j].posY, W, H, circleRadius, circleColor);
            }
            Refresh();
            usleep(wait);
        }
    }
    
    //Write to file
    fp = fopen("result.gal","w");
    if (!fp)
    {                           
      printf("Error: Could not open file for Writing\n");  
      return -1;
    } 
    star_t *starPointer = starArr;
    for (int i = 0; i < N; i++)
    {
        fwrite(&(starPointer->posX),sizeof(double),1,fp);
        fwrite(&(starPointer->posY),sizeof(double),1,fp);
        fwrite(&(starPointer->mass),sizeof(double),1,fp);
        fwrite(&(starPointer->velX),sizeof(double),1,fp);
        fwrite(&(starPointer->velY),sizeof(double),1,fp);
        fwrite(&(brightArr[i]),sizeof(double),1,fp);
        starPointer++;
    }
    fclose(fp);
    free(starArr);
    return 0;
}