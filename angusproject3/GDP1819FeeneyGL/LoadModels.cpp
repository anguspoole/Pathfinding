#include "globalOpenGLStuff.h"
#include "globalStuff.h"		// for g_pRogerRabbit

#include "cVAOMeshManager.h"
#include "cMeshObject.h"

#include "DebugRenderer/cDebugRenderer.h"

#include <iostream>

// Loading models was moved into this function
void LoadModelTypes( cVAOMeshManager* pTheVAOMeshManager, GLuint shaderProgramID )
{
	sModelDrawInfo frigateInfo;
	frigateInfo.meshFileName = "CyclopsFrigate.ply";
	pTheVAOMeshManager->LoadModelIntoVAO(frigateInfo, shaderProgramID);

	//sModelDrawInfo destroyerInfo;
	//destroyerInfo.meshFileName = "OxossiDestroyer.ply";
	//pTheVAOMeshManager->LoadModelIntoVAO(destroyerInfo, shaderProgramID);

	//sModelDrawInfo beaconInfo;
	//beaconInfo.meshFileName = "beacons.ply";
	//pTheVAOMeshManager->LoadModelIntoVAO(beaconInfo, shaderProgramID);

	//sModelDrawInfo heliosInfo;
	//heliosInfo.meshFileName = "HeliosFrigate2.ply";
	//pTheVAOMeshManager->LoadModelIntoVAO(heliosInfo, shaderProgramID);

	//sModelDrawInfo asteroidInfo;
	//asteroidInfo.meshFileName = "AsteroidStation.ply";
	//pTheVAOMeshManager->LoadModelIntoVAO(asteroidInfo, shaderProgramID);

	sModelDrawInfo sphereInfo;
	sphereInfo.meshFileName = "Sphere_320_faces_xyz_n_GARBAGE_uv.ply";			// "Sphere_320_faces_xyz.ply";
	pTheVAOMeshManager->LoadModelIntoVAO(sphereInfo, shaderProgramID);

	sModelDrawInfo sphereInvertedNormalsInfo;
	sphereInvertedNormalsInfo.meshFileName = "Sphere_320_faces_xyz_n_GARBAGE_uv_INVERTED_NORMALS.ply";			// "Sphere_320_faces_xyz.ply";
	pTheVAOMeshManager->LoadModelIntoVAO(sphereInvertedNormalsInfo, shaderProgramID);

	sModelDrawInfo lonelyTri;
	lonelyTri.meshFileName = "singleTriangle_xyz_n.ply";			// "singleTriangle_xyz.ply";
	pTheVAOMeshManager->LoadModelIntoVAO(lonelyTri, shaderProgramID);

	sModelDrawInfo cube1x1x1;
	cube1x1x1.meshFileName = "cube_flat_shaded_xyz_n_uv.ply";			// "cube_flat_shaded_xyz.ply";
	pTheVAOMeshManager->LoadModelIntoVAO(cube1x1x1, shaderProgramID);


	//sModelDrawInfo oldhouse;
	//oldhouse.meshFileName = "Old_House_Combined_xyz_n.ply";		
	//pTheVAOMeshManager->LoadModelIntoVAO(oldhouse, shaderProgramID);

	sModelDrawInfo smokeObject;
	smokeObject.meshFileName = "1x1_Star_1_Quad_2_Sided_xyz_n_uv.ply";		
	pTheVAOMeshManager->LoadModelIntoVAO(smokeObject, shaderProgramID);


	// Load the textures, too
	::g_pTheTextureManager->SetBasePath("assets/textures");

	if ( ! ::g_pTheTextureManager->Create2DTextureFromBMPFile("Justin.bmp", true ) )
	{
		std::cout << "Didn't load texture" << std::endl;
	}
	::g_pTheTextureManager->Create2DTextureFromBMPFile("grass.bmp", true );
	::g_pTheTextureManager->Create2DTextureFromBMPFile("brick-wall.bmp", true );
	::g_pTheTextureManager->Create2DTextureFromBMPFile("220px-Emma_Watson_2013.bmp", true );
	::g_pTheTextureManager->Create2DTextureFromBMPFile("Flowers.bmp", true );
	::g_pTheTextureManager->Create2DTextureFromBMPFile("Smoke_1.bmp", true );

	if ( ::g_pTheTextureManager->Create2DTextureFromBMPFile("rock_cave_stylized_height.bmp", true ) )
	{
		std::cout << "loaded height map, too" << std::endl;
	}

	// Load the cube map
	::g_pTheTextureManager->SetBasePath("assets/textures/cubemaps");
	std::string errorString;
	//if ( ::g_pTheTextureManager->CreateCubeTextureFromBMPFiles("CityCubeMap", 
	//	 "city_lf.bmp", "city_rt.bmp",				// reverse these
	//	 "city_dn.bmp", "city_up.bmp",				// Rotate the image "right 90 degrees")
	//	 "city_ft.bmp", "city_bk.bmp", true, errorString ) )
	//{
	//	std::cout << "Loaded the city cube map OK" << std::endl;
	//}
	//else
	//{
	//	std::cout << "Error: city cube map DIDN't load. On no!" << std::endl;
	//}
	if ( ::g_pTheTextureManager->CreateCubeTextureFromBMPFiles("CityCubeMap", 
		 "SpaceBox_right1_posX.bmp", "SpaceBox_left2_negX.bmp", 
		 "SpaceBox_top3_posY.bmp", "SpaceBox_bottom4_negY.bmp", 
		 "SpaceBox_front5_posZ.bmp", "SpaceBox_back6_negZ.bmp", true, errorString ) )
	{
		std::cout << "Loaded the city cube map OK" << std::endl;
	}
	else
	{
		std::cout << "Error: city cube map DIDN't load. On no!" << std::endl;
	}
	//if ( ::g_pTheTextureManager->CreateCubeTextureFromBMPFiles("CityCubeMap", 
	//	 "TropicalSunnyDayLeft2048.bmp", "TropicalSunnyDayRight2048.bmp",		// Alternate these
	//	 "TropicalSunnyDayDown2048.bmp", "TropicalSunnyDayUp2048.bmp", 			// Rotate these 90 degrees
	//	 "TropicalSunnyDayFront2048.bmp", "TropicalSunnyDayBack2048.bmp", true, errorString ) )
	//{
	//	std::cout << "Loaded the city cube map OK" << std::endl;
	//}
	//else
	//{
	//	std::cout << "Error: city cube map DIDN't load. On no!" << std::endl;
	//}


	return;
}

// This is not a good place to put this, but it's near the LoadModels...
//extern cDebugRenderer* g_pDebugRenderer;// = NULL;


	
// Loads the models we are drawing into the vector
void LoadModelsIntoScene(cVAOMeshManager* pTheVAOMeshManager, std::vector<cMeshObject*> &vec_pObjectsToDraw )
{
	//cMeshObject* pRogerRabbit = NULL;

	{
		//cMeshObject* pBunny = new cMeshObject();
		::g_pRogerRabbit = new cMeshObject();
//		::g_pRogerRabbit->preRotation = glm::vec3( 0.0f, 0.0f, 0.0f );
		::g_pRogerRabbit->position = glm::vec3(  0.0f, 25.0f, 0.0f );
		//::g_pRogerRabbit->postRotation = glm::vec3( glm::radians(45.0f), 0.0f, 0.0f );

		::g_pRogerRabbit->setMeshOrientationEulerAngles( glm::vec3( glm::radians(45.0f), 0.0f, 0.0f ) );

//		::g_pRogerRabbit->nonUniformScale = glm::vec3(9.0f, 9.0f, 9.0f);
		::g_pRogerRabbit->setUniformScale( 5.0f );
//		::g_pRogerRabbit->objColour = glm::vec3( 1.0f, 1.0f, 0.0f );
//		::g_pRogerRabbit->setDiffuseColour( glm::vec3(1.0f, 1.0f, 0.0f) );
		::g_pRogerRabbit->bUseVertexColour = true;

		// Make the terrain really shinny...
		::g_pRogerRabbit->setSpecularPower( 10.0f );
//		::g_pRogerRabbit->setSpecularColour( glm::vec3(1.0f, 0.0f, 0.0f) );
//		::g_pRogerRabbit->setSpecularPower( 0.54f );


		// "Gold" bunny?
		::g_pRogerRabbit->setDiffuseColour( glm::vec3(1.0f, 223.0f/255.0f, 0.0f) );
		::g_pRogerRabbit->bUseVertexColour = false;
		// Taken from: https://docs.unrealengine.com/en-us/Engine/Rendering/Materials/PhysicallyBased
		// Note that the specular HIGHLIGHT colour is DIFFERENT from 
		// the diffuse... 
		::g_pRogerRabbit->setSpecularPower( 100.0f );
		::g_pRogerRabbit->setSpecularColour( glm::vec3(1.000f, 0.766f, 0.336f) );

		

//		::g_pRogerRabbit->meshName = "bun_res3_xyz.ply";
		::g_pRogerRabbit->meshName = "bun_zipper_hi_res_xyz_n_smartUVBlender.ply";
		::g_pRogerRabbit->bIsVisible = true;
		// 
		::g_pRogerRabbit->friendlyName = "Roger";

		::g_pRogerRabbit->pDebugRenderer = ::g_pDebugRenderer;


		sTextureInfo flowers;
		flowers.name = "Flowers.bmp";
		flowers.strength = 1.0f;
		::g_pRogerRabbit->vecTextures.push_back( flowers );

		sTextureInfo justin;
		justin.name = "Justin.bmp";
		justin.strength = 0.0f;
		::g_pRogerRabbit->vecTextures.push_back( justin );


		//vec_pObjectsToDraw.push_back( ::g_pRogerRabbit );
	}
	int indexSelectedObject = 0;

	{	// This will be our "skybox" object.
		// (could also be a cube, or whatever)
		cMeshObject* pSkyBoxObject = new cMeshObject();
		pSkyBoxObject->setDiffuseColour( glm::vec3( 1.0f, 105.0f/255.0f, 180.0f/255.0f ) );
		pSkyBoxObject->bUseVertexColour = false;
		pSkyBoxObject->friendlyName = "SkyBoxObject";
		float scale = 5000.0f;	
		pSkyBoxObject->nonUniformScale = glm::vec3(scale,scale,scale);
		pSkyBoxObject->meshName = "Sphere_320_faces_xyz_n_GARBAGE_uv_INVERTED_NORMALS.ply";			// "Sphere_320_faces_xyz.ply";
//		pSkyBoxObject->bIsWireFrame = true;

		// Invisible until I need to draw it
		pSkyBoxObject->bIsVisible = false;

		vec_pObjectsToDraw.push_back( pSkyBoxObject );
	}

	//******************************
	// Flock Ships
	//******************************
	//{
	//	for (int i = 0; i < 12; i++)
	//	{
	//		// This sphere is the tiny little debug sphere
	//		cMeshObject* pPirate = new cMeshObject();
	//		pPirate->setDiffuseColour(glm::vec3(1.0f, 1.0f, 1.0f));
	//		//		pPirate->setAlphaTransparency( 0.50f );
	//		pPirate->setAlphaTransparency(1.0f);
	//		pPirate->position.x = 25.0f;
	//		pPirate->bIsUpdatedByPhysics = true;
	//		pPirate->friendlyName = "FlockShip" + std::to_string(i);
	//		pPirate->meshName = "CyclopsFrigate.ply";		// "cube_flat_shaded_xyz.ply";
	//		float scale = 0.5f;
	//		pPirate->setUniformScale(scale);
	//		pPirate->position = glm::vec3(0.0f, 0.0f, 0.0f);
	//		pPirate->position.x += (20.0f * (i % 6));
	//		if (i > 5)
	//		{
	//			pPirate->position.z += (20.0f);
	//		}
	//		pPirate->velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	//		pPirate->objectClass = cMeshObject::objClass::PLAYER;

	//		pPirate->pTheShape = new sSphere(scale);
	//		pPirate->shapeType = cMeshObject::SPHERE;

	//		vec_pObjectsToDraw.push_back(pPirate);
	//		aiObjects.push_back(pPirate);
	//	}		
	//}

	//******************
	//	Gatherer
	//******************
	{
		// This sphere is the tiny little debug sphere
		cMeshObject* pPlayer = new cMeshObject();
		pPlayer->setDiffuseColour(glm::vec3(1.0f, 0.0f, 1.0f));
		//		pPirate->setAlphaTransparency( 0.50f );
		pPlayer->setAlphaTransparency(1.0f);
		pPlayer->position.x = 25.0f;
		pPlayer->bIsUpdatedByPhysics = true;
		pPlayer->friendlyName = "Gatherer";
		pPlayer->meshName = "CyclopsFrigate.ply";		// "cube_flat_shaded_xyz.ply";
		float scale = 0.35f;
		pPlayer->setUniformScale(scale);
		pPlayer->position = glm::vec3(0.0f, 0.0f, 0.0f);
		pPlayer->velocity = glm::vec3(0.0f, 0.0f, 0.0f);
		pPlayer->objectClass = cMeshObject::objClass::PLAYER;

		pPlayer->aiType = cMeshObject::aiBehaviour::GATHER;
		pPlayer->needPath = true;

		pPlayer->pTheShape = new sSphere(scale);
		pPlayer->shapeType = cMeshObject::SPHERE;

		vec_pObjectsToDraw.push_back(pPlayer);
	}

	return;
}