#ifndef SEABATTLE_ISOENGINE_H
#define SEABATTLE_ISOENGINE_H

typedef struct isoEngineT {
    int scrollX;
    int scrollY;
    int mapHeight;
    int mapWidth;
}isoEngineT;

typedef struct point2DT {
    int x;
    int y;
}point2DT;


void GetTileCoordinates (point2DT* point, point2DT* point2DCoord);
void Converter2DToIso(point2DT* point);
void ConverterIsoTo2D(point2DT* point);
void IsoEngineSetMapSize(isoEngineT* isoEngineT, int width, int height);
void init_IsoEngine(isoEngineT* isoEngineT, int tileSizeInPixels);

#endif //SEABATTLE_ISOENGINE_H