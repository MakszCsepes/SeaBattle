#include "IsoEngine.h"
#include <math.h>
#include <stdio.h>

void init_IsoEngine(isoEngineT* isoEngineT, int tileSizeInPixels) {
    if (isoEngineT == NULL) {
        fprintf(stderr, "Error in init_IsoEngine(...) : IsoEngine parameter is NULL \n");
        return;
    }

    if(tileSizeInPixels <= 0) {
        TILESIZE = 32;
    } else {
        TILESIZE = tileSizeInPixels;
    }

    isoEngineT->mapHeight = 0;
    isoEngineT->mapWidth = 0;
    isoEngineT->scrollX = 0;
    isoEngineT->scrollY = 0;
}

void IsoEngineSetMapSize(isoEngineT* isoEngineT, int width, int height) {
    if (isoEngineT == NULL) {
        isoEngineT->mapWidth = width;
        isoEngineT->mapHeight = height;
    }

}

void Converter2DToIso(point2DT* point) {
    int tmpX = (point->x - point->y);
    int tmpY = (point->x - point->y)*0.5;

    point->x = tmpX;
    point->y = tmpY;
}

void GetTileCoordinates (point2DT* point, point2DT* point2DCoord) {
    float tempX = (float)point->x/(float)TILESIZE;
    float tempY = (float)point->y/(float)TILESIZE;

    //convert back to integer

    point2DCoord->x = (int)tempX;
    point2DCoord->y = (int)tempY;
}