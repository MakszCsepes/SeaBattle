#include "IsoEngine.h"
#include <math.h>
#include <stdio.h>

#include <iostream>
using namespace std;

unsigned int TILESIZE = 32;

void init_IsoEngine(isoEngineT* isoEngineT) {
    if (isoEngineT == NULL) {
        fprintf(stderr, "Error in init_IsoEngine(...) : IsoEngine parameter is NULL \n");
        return;
    }

    isoEngineT->mapHeight = 0;
    isoEngineT->mapWidth = 0;
    isoEngineT->scrollX = 0;
    isoEngineT->scrollY = 0;
}

void IsoEngineSetMapSize(isoEngineT* isoEngineT, int width, int height) {
    if (isoEngineT == NULL) {
        return;
    }
    isoEngineT->mapWidth = width;
    isoEngineT->mapHeight = height;
}

void Converter2DToIso(point2DT* point) {
    int tmpX = (point->x - point->y);
    int tmpY = (point->x + point->y)*0.5;

    point->x = tmpX;
    point->y = tmpY;
}
void ConverterIsoTo2D(point2DT* point) {
    int tmpX = (2 * point->y + point->x) * 0.5;
    int tmpY = (2 * point->y - point->x) * 0.5;

    point->x = tmpX;
    point->y = tmpY;
}

void GetTileCoordinates (point2DT* point, point2DT* point2DCoord) {
    float tempX = (float)point->x / (float)TILESIZE;
    float tempY = (float)point->y / (float)TILESIZE;

    //convert back to integer
    point2DCoord->x = (int)tempX;
    point2DCoord->y = (int)tempY;
}