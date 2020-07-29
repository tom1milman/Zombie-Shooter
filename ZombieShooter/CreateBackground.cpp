#include "SFML/Graphics.hpp"
#include "ZombieShooter.h"

int createBackground(VertexArray& rVA, IntRect arena)
{
	const int TILE_SIZE = 50;
	const int TILE_TYPES = 3;
	const int VERTS_IN_QUAD = 4;

	int worldWidth = arena.width / TILE_SIZE;
	int worldHeight = arena.height / TILE_SIZE;

	//set vertexarray
	rVA.setPrimitiveType(Quads);
	rVA.resize(worldWidth * worldHeight * VERTS_IN_QUAD);

	int currentVertex = 0;

	for (int w = 0; w < worldWidth; w++)
	{
		for (int h = 0; h < worldHeight; h++) 
		{
			int wTileSize = w * TILE_SIZE;
			int hTileSize = h * TILE_SIZE;

			rVA[currentVertex + 0].position = Vector2f(wTileSize, hTileSize);

			rVA[currentVertex + 1].position = Vector2f(wTileSize + TILE_SIZE, hTileSize);

			rVA[currentVertex + 2].position = Vector2f(wTileSize + TILE_SIZE, hTileSize + TILE_SIZE);

			rVA[currentVertex + 3].position = Vector2f(wTileSize, hTileSize + TILE_SIZE);

			//If wall set wall texture
			if (h == 0 || h == worldHeight - 1 ||
				w == 0 || w == worldWidth - 1)
			{
				int typesSize = TILE_TYPES * TILE_SIZE;

				rVA[currentVertex + 0].texCoords = Vector2f(0, 0 + typesSize);
				
				rVA[currentVertex + 1].texCoords = Vector2f(TILE_SIZE, 0 + typesSize);

				rVA[currentVertex + 2].texCoords = Vector2f(TILE_SIZE, TILE_SIZE + typesSize);

				rVA[currentVertex + 3].texCoords = Vector2f(0, TILE_SIZE + typesSize);
			}
			//Else, use random texture
			else
			{
				srand((int)time(0) + h * w - h);
				int mOrG = (rand() % TILE_TYPES);
				int verticalOffset = mOrG * TILE_SIZE;

				rVA[currentVertex + 0].texCoords = Vector2f(0, verticalOffset);
				
				rVA[currentVertex + 1].texCoords = Vector2f(TILE_SIZE, verticalOffset);
				
				rVA[currentVertex + 2].texCoords = Vector2f(TILE_SIZE, TILE_SIZE + verticalOffset);
				
				rVA[currentVertex + 3].texCoords = Vector2f(0, TILE_SIZE + verticalOffset);

			}

			currentVertex = currentVertex + VERTS_IN_QUAD;
		}

	}

	return TILE_SIZE;
}