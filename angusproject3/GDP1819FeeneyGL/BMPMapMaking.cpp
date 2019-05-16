#include "BMPImage.h"
#include "ResourceManager.h"
#include "globalStuff.h"
#include <vector>
#include <utility>

ResourceManager gResourceManager;

std::vector<std::vector<char>> bmpPixelValues;

std::vector<std::vector<cMeshObject*>> nodeMeshObjs;

std::vector<std::vector<cMapNode*>> mapNodes;

std::map<std::string, cMapNode*> positionNodeMap;

std::map<std::string, cMeshObject*> resourceMap;

cMapNode* homeNode = NULL;

char GetColourCharacter(unsigned char r, unsigned char g, unsigned char b)
{
	//printf("r: %u", (unsigned int)r);
	//printf("\n");

	//printf(" g: %u", (unsigned int)g);
	//printf("\n");

	//printf(" b :%u", (unsigned int)b);
	//printf("\n");

	if ((int)r == 255 && (int)g == 0 && (int)b == 0) {
		return 'r';
		//pixelValue = 'r';
	}
	if (r == 0 && g == 255 && b == 0)
	{
		return 'g';
		//pixelValue = 'g';
	}
	if (r == 0 && g == 0 && b == 255)
	{
		return 'b';
	}
	if (r == 255 && g == 255 && b == 255)
	{
		return 'w';
	}
	if (r == 0 && g == 0 && b == 0)
	{
		return '_';
	}
	return 'x';
}

void GetBMPPixelValues(std::string bmpFileName)
{

	//BMPImage* bmp = new BMPImage("zelda.bmp");
	BMPImage* bmp = new BMPImage(bmpFileName);

	char* data = bmp->GetData();
	unsigned long imageWidth = bmp->GetImageWidth();
	unsigned long imageHeight = bmp->GetImageHeight();

	int index = 0;
	int r, g, b;
	r = 0;
	g = 0;
	b = 0;

	for (unsigned long y = 0; y < imageHeight; y++) 
	{
		std::vector<char> newVector;
		bmpPixelValues.push_back(newVector);
		for (unsigned long x = 0; x < imageWidth; x++) 
		{
			char pixelValue;

			//std::cout << index << std::endl;
			// Works only in debug
			//printf("%c", GetColourCharacter(data[index++], data[index++], data[index++]));
			if (index == 0) {
				b = index;

			}
			else {
				index++;
				b = index;

			}
			index++;
			g = index;
			index++;
			r = index;

			pixelValue = GetColourCharacter(data[r], data[g], data[b]);

			bmpPixelValues[y].push_back(pixelValue);

			//printf("%c", bmpPixelValues[y][x]);

		}
		//printf("\n");
	}

	std::vector<std::vector<char>> tempPixelValues;

	//for (int i = imageHeight - 1; i > -1; i--)
	for (int i = 0; i < imageHeight; i++)
	{
		std::vector<char> tmp;
		//tempPixelValues.push_back(bmpPixelValues[i]);

		for (int j = imageWidth - 1; j > -1; j--)
		//for (int j = 0; j < imageWidth; j++)
		{
			tmp.push_back(bmpPixelValues[i][j]);
		}

		tempPixelValues.push_back(tmp);
	}

	bmpPixelValues = tempPixelValues;

	//printf("%d", bmpPixelValues.size());

	/*for (int i = 0; i < imageHeight; i++)
	{
		for (int j = 0; j < imageWidth; j++)
		{
			printf("%c", bmpPixelValues[i][j]);
		}
		printf("\n");
	}*/

	//system("pause");
}

void CreateMap(std::vector<cMeshObject*> &vec_pObjectsToDraw)
{
	for (unsigned int a = 0; a < bmpPixelValues.size(); a++)
	{
		std::vector<cMeshObject*> objVector;
		nodeMeshObjs.push_back(objVector);
		std::vector<cMapNode*> nodeVector;
		mapNodes.push_back(nodeVector);

		for (unsigned int b = 0; b < bmpPixelValues[a].size(); b++)
		{
			bool validColour = true;
			glm::vec3 objColour;
			switch (bmpPixelValues[a][b])
			{
			case 'r':
				objColour = glm::vec3(1.0f, 0.0f, 0.0f);
				break;
			case 'g':
				objColour = glm::vec3(0.0f, 1.0f, 0.0f);
				break;
			case 'b':
				objColour = glm::vec3(0.0f, 0.0f, 1.0f);
				break;
			case 'w':
				objColour = glm::vec3(1.0f, 1.0f, 1.0f);
				break;
			case '_':
				objColour = glm::vec3(0.0f, 0.0f, 0.0f);
				break;
			default:
				validColour = false;
				break;
			}

			//printf("%f, %f, %f\n", objColour.x, objColour.y, objColour.z);

			if (validColour)
			{
				// This sphere is the tiny little debug sphere
				cMeshObject* pCube = new cMeshObject();
				//		pCubeForBallsToBounceIn->objColour = glm::vec3( 0.0f, 1.0f, 0.0f );
				objColour * 255.0f;
				pCube->setDiffuseColour(objColour);
				pCube->bDontLight = true;
				pCube->friendlyName = "cube" + std::to_string(a) + std::to_string(b);
				pCube->meshName = "cube_flat_shaded_xyz_n_uv.ply";		// "cube_flat_shaded_xyz.ply";
				float scale = 5.0f;
				if (bmpPixelValues[a][b] == '_')
				{
					pCube->nonUniformScale = glm::vec3(scale, scale * 3, scale);
				}
				else
				{
					pCube->setUniformScale(scale);
				}
				// Cube is 2x2x2, so with a scale of 100x means it's
				//	200x200x200, centred around the origin (0,0,0)
				// The GROUND_PLANE_Y = -3.0f, so place it +97.0 lines up the 'bottom'
				pCube->position = glm::vec3(b * 2.001f, 0.0f, a * 2.001f);
				pCube->position *= scale;
				//pCube->position = glm::vec3(a * 3.001f, 0.0f, b * 3.001f);
				pCube->bIsWireFrame = false;

				pCube->pDebugRenderer = ::g_pDebugRenderer;

				//sTextureInfo testObjectTexture;
				//testObjectTexture.name = "fo_rock_wall_xy_df.bmp";
				//testObjectTexture.strength = 1.0f;
				//pCube->vecTextures.push_back(sTextureInfo(testObjectTexture));


				int scaleInt = (int)scale;

				//pCube->nonUniformScale = glm::vec3(0.1f,0.1f,0.1f);
				vec_pObjectsToDraw.push_back(pCube);
				nodeMeshObjs[a].push_back(pCube);

				cMapNode* mapNode = new cMapNode();
				mapNode->thisNodeObj = nodeMeshObjs[a][b];
				mapNode->nodeColour = bmpPixelValues[a][b];
				mapNodes[a].push_back(mapNode);

				glm::vec3 roundedPos = pCube->position;
				//roundedPos.x = (int)roundedPos.x;
				//roundedPos.y = (int)roundedPos.y;
				//roundedPos.z = (int)roundedPos.z;

				int xRemainder = (int)(pCube->position.x) % (10);
				int zRemainder = (int)(pCube->position.z) % (10);

				//printf("%s %d", "x val", xRemainder);
				//printf("%s %d", "z val", zRemainder);

				if (xRemainder < 5)
				{
					roundedPos.x = roundedPos.x - xRemainder;
				}
				else
				{
					roundedPos.x = roundedPos.x - xRemainder + 5;
				}

				if (zRemainder < 5)
				{
					roundedPos.z = roundedPos.z - zRemainder;
				}
				else
				{
					roundedPos.z = roundedPos.z - zRemainder + 5;

				}

				//roundedPos.x = roundedPos.x + scaleInt / 2;
				//roundedPos.x -= (int)roundedPos.x % (int)scaleInt;

				//roundedPos.y = 0;

				//roundedPos.z = roundedPos.z + scaleInt / 2;
				//roundedPos.z -= (int)roundedPos.z % (int)scaleInt;

				std::string key = std::to_string((int)roundedPos.z);
				key.append(std::to_string((int)roundedPos.x));

				//printf("%s %s\n", "Key: ", key.c_str());

				mapNode->key = key;

				std::pair<std::string, cMapNode*> vecNodePair = std::make_pair(key, mapNode);

				positionNodeMap.insert(vecNodePair);
			}
		}
	}
	CreateNodeNeighbours();
	PopulateMap(vec_pObjectsToDraw);
}

void CreateNodeNeighbours()
{
	for (int i = 0; i < mapNodes.size(); i++)
	{
		for (int j = 0; j < mapNodes[i].size(); j++)
		{
			cMapNode* node = mapNodes[i][j];
			//If not on top row
			if (i != 0)
			{
				node->neighbourSS = mapNodes[i - 1][j]; //add south neighbour
				if (j != 0)
				{
					node->neighbourSE = mapNodes[i - 1][j - 1]; //add se neighbour
				}
				if (j != mapNodes[i].size() - 1)
				{
					node->neighbourSW = mapNodes[i - 1][j + 1]; //add sw neighbour
				}
			}
			if (i != mapNodes.size() - 1)
			{
				node->neighbourNN = mapNodes[i + 1][j];
				if (j != 0)
				{
					node->neighbourNE = mapNodes[i + 1][j - 1]; //add se neighbour
				}
				if (j != mapNodes[i].size() - 1)
				{
					node->neighbourNW = mapNodes[i + 1][j + 1]; //add sw neighbour
				}
			}
			if (j != 0)
			{
				node->neighbourEE = mapNodes[i][j - 1]; //add east neighbour
			}
			if (j != mapNodes[i].size() - 1)
			{
				node->neighbourWW = mapNodes[i][j + 1]; //add west neighbour
			}
		}
	}
}

void PopulateMap(std::vector<cMeshObject*> &vec_pObjectsToDraw)
{

	int resourceCount = 0;

	for (int i = 0; i < mapNodes.size(); i++)
	{
		for (int j = 0; j < mapNodes[i].size(); j++)
		{
			//Player starting position
			if (mapNodes[i][j]->nodeColour == 'g')
			{
				cMeshObject* player = findObjectByFriendlyName("Gatherer");
				player->position = mapNodes[i][j]->thisNodeObj->position;
				player->position.y += ((mapNodes[i][j]->thisNodeObj->nonUniformScale.y)* 1.5f);
			}

			//Resource positions
			if (mapNodes[i][j]->nodeColour == 'r')
			{

				// This sphere is the tiny little debug sphere
				cMeshObject* pPlayer = new cMeshObject();
				pPlayer->setDiffuseColour(glm::vec3(1.0f, 0.0f, 0.0f));
				//		pPirate->setAlphaTransparency( 0.50f );
				pPlayer->setAlphaTransparency(1.0f);
				pPlayer->bIsUpdatedByPhysics = true;
				pPlayer->friendlyName = "Resource" + resourceCount;
				pPlayer->meshName = "Sphere_320_faces_xyz_n_GARBAGE_uv.ply";		// "cube_flat_shaded_xyz.ply";
				float scale = 3.0f;
				pPlayer->setUniformScale(scale);
				pPlayer->position = mapNodes[i][j]->thisNodeObj->position;
				pPlayer->position.y += ((mapNodes[i][j]->thisNodeObj->nonUniformScale.y)* 1.5f);
				pPlayer->velocity = glm::vec3(0.0f, 0.0f, 0.0f);
				pPlayer->objectClass = cMeshObject::objClass::RESOURCE;

				pPlayer->pTheShape = new sSphere(scale);
				pPlayer->shapeType = cMeshObject::SPHERE;

				vec_pObjectsToDraw.push_back(pPlayer);

				resourceCount++;
				std::string nodeKey = getObjNodeKey(pPlayer);
				resourceMap.insert(std::pair<std::string, cMeshObject*>(nodeKey, pPlayer));
				mapNodes[i][j]->hasResource = true;
			}
			if(mapNodes[i][j]->nodeColour == 'b')
			{
				homeNode = mapNodes[i][j];
			}
		}
	}
}