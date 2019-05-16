#include "globalStuff.h"
#include <iostream>

void SeekFlee(cMeshObject* obj);
void PursueEvade(cMeshObject* obj);
void ApproachFire(cMeshObject* obj);
void WanderIdle(cMeshObject* obj);

float cos_angle = cos(glm::pi<float>() / 2); //cone of sight

void ProcessAI(float dt)
{
	for (int i = 0; i < aiObjects.size(); i++)
	{
		int c = aiObjects[i]->aiType;

		//Update timer for certain AI
		aiObjects[i]->timer += dt;

		switch (c)
		{
			case cMeshObject::aiBehaviour::SEEKFLEE:
			{
				SeekFlee(aiObjects[i]);
			}
			break;
			case cMeshObject::aiBehaviour::PURSUEEVADE:
			{
				PursueEvade(aiObjects[i]);
			}
			break;
			case cMeshObject::aiBehaviour::APPROACHFIRE:
			{
				if (aiObjects[i]->timer > 2.0f)
				{
					aiObjects[i]->timer = 0.0f;
				}
				ApproachFire(aiObjects[i]);
			}
			break;
			case cMeshObject::aiBehaviour::WANDERIDLE:
			{
				WanderIdle(aiObjects[i]);
			}
			break;
			default:
			{
				continue;
			}
		}
	}
}

void SeekFlee(cMeshObject* obj)
{
	//Check if Seeking or Fleeing
	cMeshObject * player = findObjectByFriendlyName("PlayerShip");
	glm::quat playerRotation = player->getQOrientation();
	glm::vec3 point = glm::normalize(obj->position - player->position);
	glm::vec3 playerDir = glm::normalize(playerRotation * glm::vec3(0.0f, 0.0f, 1.0f)); //direction player is facing
	float dotproduct = glm::dot(playerDir, point);
	bool visible = dotproduct > cos_angle;

	glm::vec3 dir = glm::normalize(player->position - obj->position);
	glm::vec3 currentVel = glm::normalize(obj->velocity);
	float currentVelLength = obj->velocity.length();

	if (!visible)
	{
		obj->setDiffuseColour(glm::vec3(1.0f, 0.0f, 0.0f));
		glm::vec3 steeringVel = dir - currentVel;
		obj->velocity += (steeringVel * currentVelLength);
	}
	else
	{
		obj->setDiffuseColour(glm::vec3(0.5f, 0.5f, 0.0f));
		glm::vec3 steeringVel = dir - currentVel;
		obj->velocity = -(steeringVel * currentVelLength);
	}

}

void PursueEvade(cMeshObject* obj)
{
	cMeshObject * player = findObjectByFriendlyName("PlayerShip");
	float step = 2.0f; //steps into the future
	//glm::quat playerRotation = player->getQOrientation();
	glm::vec3 playerVel = player->velocity;
	glm::vec3 playerFuturePos = player->position + (player->velocity * step);

	glm::vec3 dir = glm::normalize(playerFuturePos - obj->position);
	glm::vec3 currentVel = glm::normalize(obj->velocity);
	float currentVelLength = obj->velocity.length();

	float bulletDistance = 999.0f;

	cMeshObject * bullet = NULL;

	//Evade
	for (int i = 0; i < vec_pObjectsToDraw.size(); i++)
	{
		cMeshObject * foundBullet = vec_pObjectsToDraw[i];

		//If the object is a bullet
		if (foundBullet->friendlyName == "bullet")
		{
			glm::vec3 futureBulletPos = foundBullet->position + (glm::normalize(foundBullet->velocity) * step);
			float dist = glm::distance(futureBulletPos, obj->position);
			if (dist < bulletDistance)
			{
				bullet = foundBullet;
				bulletDistance = dist;
			}
		}
	}
	// If the bullet is close enough to the enemy ship, Evade
	if (bulletDistance < 20.0f)
	{
		glm::vec3 futureBulletPos = bullet->position + (glm::normalize(bullet->velocity) * step);
		glm::vec3 adjustDir = glm::normalize(obj->position - futureBulletPos);
		obj->velocity = adjustDir * currentVelLength * 5.0f;//glm::normalize(obj->velocity + adjustDir) * currentVelLength;
		obj->setDiffuseColour(glm::vec3(1.0f, 1.0f, 0.0f));
	}
	else
	{
		//Pursue
		obj->velocity = (dir * currentVelLength * 5.0f);
		obj->setDiffuseColour(glm::vec3(0.0f, 1.0f, 0.0f));
	}
}

void ApproachFire(cMeshObject* obj)
{
	//Approach the player
	cMeshObject * player = findObjectByFriendlyName("PlayerShip");
	float distance = glm::distance(obj->position, player->position);

	glm::vec3 dir = glm::normalize(player->position - obj->position);
	glm::quat q = glm::inverse(glm::lookAt(obj->position, player->position, glm::vec3(0.0f, 1.0f, 0.0f)));
	obj->setQOrientation(q);
	obj->adjMeshOrientationEulerAngles(glm::vec3(glm::radians(0.0f), glm::radians(90.0f), glm::radians(0.0f)));

	//maximum range
	if (distance > 80.0f && obj->timer > 0.0f)
	{
		//move towards player
		float len = obj->velocity.length();
		obj->velocity = dir * len * 5.0f;
		obj->setDiffuseColour(glm::vec3(0.0f, 0.0f, 1.0f));
	}
	else if ((distance < 80.0f) && !(obj->timer > 0.000001f)) //fire
	{
		float bulletSpeed = 30.0f;
		obj->velocity = glm::vec3(0.0f);
		cMeshObject * pBullet = findObjectByFriendlyName("EnemySphere0");

		if (pBullet)
		{
			cMeshObject * bullet = new cMeshObject(pBullet);
			bullet->friendlyName = "EnemySphere0";
			bullet->meshName = pBullet->meshName;
			bullet->position = obj->position;
			bullet->velocity = dir * bulletSpeed;
			bullet->bIsVisible = true;
			bullet->bIsUpdatedByPhysics = true;
			bullet->objectClass = cMeshObject::objClass::ENEMYBULLET;
			vec_pObjectsToDraw.push_back(bullet);
			obj->setDiffuseColour(glm::vec3(1.0f, 0.0f, 1.0f));
		}
	}
	else
	{
		obj->velocity = glm::vec3(0.0f);
		obj->setDiffuseColour(glm::vec3(1.0f, 0.0f, 1.0f));
	}
}

void WanderIdle(cMeshObject* obj)
{
	if (glm::distance(obj->targetPos, obj->position) > 0.1f)
	{
		glm::vec3 dir = glm::normalize(obj->targetPos - obj->position);
		obj->velocity = dir * 5.0f;
		obj->timer = 0.0f;
		//std::cout << "Tx: " << obj->position.x << ", " << "Ty: " << obj->position.y << ", " << "Tz: " << obj->position.z << ", " << std::endl;
		//std::cout << "Tx: " << obj->targetPos.x << ", " << "Ty: " << obj->targetPos.y << ", " << "Tz: " << obj->targetPos.z << ", " << std::endl;
		obj->setDiffuseColour(glm::vec3(0.0f, 1.0f, 1.0f));
	}
	else
	{
		if (obj->timer > 3.0f)
		{
			int winH = std::rand() % windowHeight * 0.6f;
			int winW = std::rand() % windowWidth * 0.6f;
			int randX = std::rand() % ((winW * 2) - 0 + 1);
			randX -= (winW / 2);
			int randY = 0;
			int randZ = std::rand() % ((winH * 2) - 0 + 1);
			randZ -= (winH / 2);

			obj->targetPos = glm::vec3(randX, randY, randZ);
			glm::vec3 dir = glm::normalize(obj->targetPos - obj->position);
			obj->velocity = dir * 5.0f;

			obj->timer = 0.0f;

			obj->setDiffuseColour(glm::vec3(0.5f, 1.0f, 1.0f));

			//std::cout << "x: " << obj->position.x << ", " << "y: " << obj->position.y << ", " << "z: " << obj->position.z << ", " << std::endl;
			//std::cout << "Tx: " << obj->targetPos.x << ", " << "Ty: " << obj->targetPos.y << ", " << "Tz: " << obj->targetPos.z << ", " << std::endl;
		}
		else
		{
			obj->setDiffuseColour(glm::vec3(1.0f, 0.5f, 0.5f));
			obj->velocity = glm::vec3(0.0f);
		}
		std::cout << obj->timer << ", " << std::endl;
	}
}