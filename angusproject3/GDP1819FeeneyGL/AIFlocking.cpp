#include "Flock.h"
#include "globalStuff.h"
#include <iostream>
#include <algorithm>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

std::vector<Flock*> flockList;
Flock* smallFlock;
bool flockEnabled = false;
int previousFormation = 0;

std::vector<std::pair<glm::vec3, float>> path; //direction and duration of path segment
float pathTimer = 0.0f;
int currentSegment = 0;
bool pathForward = true;
bool travelPath = false;

void CreateFlockTest()
{
	// Create 6 entities and add to a Flock
	smallFlock = new Flock(0.4f, 0.4f, 0.2f);

	for (int i = 0; i < 12; i++)
	{
		cMeshObject * obj = findObjectByFriendlyName("FlockShip" + std::to_string(i));
		smallFlock->AddFlockMember(obj);
	}

	GenerateFlock();
	//flockList.push_back(smallFlock);
}

Flock* GenerateFlock()
{
	for (int i = 0; i < aiObjects.size(); i++)
	{
		Flock* newFlock = new Flock();
		flockList.push_back(newFlock);
		newFlock->AddFlockMember(aiObjects[i]);
		//		Flock* newFlock = GenerateFlock();
//		newFlock->AddFlockMember(flockShip0);
//		currentFlock = newFlock;
	}
	return flockList[0];
}

void CreateFormation(int formationType)
{
	previousFormation = formationType;
	if (formationType == FormationType::CIRCLE)
	{
		int numEntities = smallFlock->mFlockMembers.size();
		float rotation = 360 / numEntities;
		float dist = 20.0f;

		glm::vec3 centerPos = (smallFlock->mFlockMembers[0])->entity->position;
		glm::vec3 forwardDir = smallFlock->mFlockMembers[0]->entity->getQOrientation() * glm::vec3(0.0f, 0.0f, 1.0f);

		for (int i = 0; i < numEntities; i++)
		{
			glm::vec3 currentPos = (smallFlock->mFlockMembers[i])->entity->position;

			glm::mat4 rotateM = glm::rotate(glm::mat4(1.0f), rotation * i, glm::vec3(0.0f, 1.0f, 0.0f));

			glm::vec4 posT = rotateM * glm::vec4(forwardDir, 1.0f);

			glm::vec3 newPos = glm::vec3(posT.x, posT.y, posT.z);

			//std::cout << newPos.x << "---" << newPos.y << "---" << newPos.z << "---" << std::endl;

			(smallFlock->mFlockMembers[i])->entity->targetPos = centerPos + (newPos * dist);
			(smallFlock->mFlockMembers[i])->entity->atTarget = false;
		}		
	}
	else if (formationType == FormationType::V)
	{
		int numEntities = smallFlock->mFlockMembers.size();
		float rotation = 360 / numEntities;
		float dist = 4.0f;

		glm::vec3 centerPos = (smallFlock->mFlockMembers[0])->entity->position;
		glm::vec3 forwardDir = smallFlock->mFlockMembers[0]->entity->getQOrientation() * glm::vec3(0.0f, 0.0f, 1.0f);

		//Loop over the entities in the flock and assign them new positions
		for (int i = 0; i < numEntities; i++)
		{
			int half = numEntities / 2;
			int d = i % (half + 1);
			if (i == 0)
			{
				//don't move - create V around this position
				(smallFlock->mFlockMembers[i])->entity->targetPos = centerPos;
				(smallFlock->mFlockMembers[i])->entity->atTarget = false;
			}
			else if (i <= half)
			{
				glm::vec3 newPos = smallFlock->mFlockMembers[0]->entity->getQOrientation() * glm::vec3(dist * -1.0f * d, 0.0f, dist * -1.0f * d);

				(smallFlock->mFlockMembers[i])->entity->targetPos = centerPos + (newPos);
				(smallFlock->mFlockMembers[i])->entity->atTarget = false;
			}
			else if (i > half)
			{
				d += 1;
				glm::vec3 newPos = smallFlock->mFlockMembers[0]->entity->getQOrientation() * glm::vec3(dist * 1.0f * d, 0.0f, dist * -1.0f * d);

				(smallFlock->mFlockMembers[i])->entity->targetPos = centerPos + (newPos);
				(smallFlock->mFlockMembers[i])->entity->atTarget = false;
			}
		}
	}
	else if (formationType == FormationType::SQUARE)
	{
		int numEntities = smallFlock->mFlockMembers.size();
		int sideLength = numEntities / 4;
		float rotation = 360 / numEntities;
		float dist = 10.0f;

		glm::vec3 centerPos = (smallFlock->mFlockMembers[0])->entity->position;
		glm::vec3 forwardDir = smallFlock->mFlockMembers[0]->entity->getQOrientation() * glm::vec3(0.0f, 0.0f, 1.0f);

		for (int i = 0; i < numEntities; i++)
		{
			if (i <= sideLength) //1 - 3
			{
				glm::vec3 newPos = smallFlock->mFlockMembers[0]->entity->getQOrientation() * glm::vec3(dist * 1.0f * i, 0.0f, 0.0f);

				(smallFlock->mFlockMembers[i])->entity->targetPos = centerPos + (newPos);
				(smallFlock->mFlockMembers[i])->entity->atTarget = false;
			}
			else if (i <= sideLength * 2) //4 - 6
			{
				int d = i % sideLength;
				d += 1;
				glm::vec3 newPos = smallFlock->mFlockMembers[0]->entity->getQOrientation() * glm::vec3(0.0f, 0.0f, dist * -1.0f * d);

				(smallFlock->mFlockMembers[i])->entity->targetPos = centerPos + (newPos);
				(smallFlock->mFlockMembers[i])->entity->atTarget = false;
			}
			else if (i <= sideLength * 3) //7 - 9
			{
				int d = i % sideLength;
				d += 1;
				glm::vec3 newPos = smallFlock->mFlockMembers[0]->entity->getQOrientation() * glm::vec3(dist * 1.0f * sideLength, 0.0f, dist * -1.0f * d);

				(smallFlock->mFlockMembers[i])->entity->targetPos = centerPos + (newPos);
				(smallFlock->mFlockMembers[i])->entity->atTarget = false;
			}
			else if (i < numEntities)
			{
				int d = i % sideLength;
				//d += 1;
				glm::vec3 newPos = smallFlock->mFlockMembers[0]->entity->getQOrientation() * glm::vec3(dist * 1.0f * d, 0.0f, dist * -1.0f * sideLength);

				(smallFlock->mFlockMembers[i])->entity->targetPos = centerPos + (newPos);
				(smallFlock->mFlockMembers[i])->entity->atTarget = false;
			}
		}
	}

	else if (formationType == FormationType::LINE)
	{
		int numEntities = smallFlock->mFlockMembers.size();
		int sideLength = numEntities / 4;
		float rotation = 360 / numEntities;
		float dist = 10.0f;

		glm::vec3 centerPos = (smallFlock->mFlockMembers[0])->entity->position;
		glm::vec3 forwardDir = smallFlock->mFlockMembers[0]->entity->getQOrientation() * glm::vec3(0.0f, 0.0f, 1.0f);

		for (int i = 0; i < numEntities; i++)
		{
			glm::vec3 newPos = smallFlock->mFlockMembers[0]->entity->getQOrientation() * glm::vec3(dist * 1.0f * i, 0.0f, 0.0f);

			(smallFlock->mFlockMembers[i])->entity->targetPos = centerPos + (newPos);
			(smallFlock->mFlockMembers[i])->entity->atTarget = false;
		}
	}

	else if (formationType == FormationType::TWO_ROWS)
	{
		int numEntities = smallFlock->mFlockMembers.size();
		float dist = 10.0f;
		glm::vec3 centerPos = (smallFlock->mFlockMembers[0])->entity->position;
		glm::vec3 forwardDir = smallFlock->mFlockMembers[0]->entity->getQOrientation() * glm::vec3(0.0f, 0.0f, 1.0f);

		int i = 0;
		for (i; i < numEntities/2; i++)
		{
			glm::vec3 newPos = smallFlock->mFlockMembers[0]->entity->getQOrientation() * glm::vec3(dist * 1.0f * i, 0.0f, 0.0f);

			(smallFlock->mFlockMembers[i])->entity->targetPos = centerPos + (newPos);
			(smallFlock->mFlockMembers[i])->entity->atTarget = false;
		}
		for (i; i < numEntities; i++)
		{
			int half = numEntities / 2;
			int d = i % half;
			glm::vec3 newPos = smallFlock->mFlockMembers[0]->entity->getQOrientation() * glm::vec3(dist * 1.0f * d, 0.0f, dist + 2.0f);

			(smallFlock->mFlockMembers[i])->entity->targetPos = centerPos + (newPos);
			(smallFlock->mFlockMembers[i])->entity->atTarget = false;
		}
	}
}

void UpdateFlocks(float dt)
{
	/*if (flockEnabled)
	{
		for (int i = 0; i < flockList.size(); i++)
		{
			flockList[i]->CalculateSteering();
		}
	}*/


	for (int i = 0; i < aiObjects.size(); i++)
	{
		aiObjects[i]->accel = glm::vec3(0.0f);
	}

	if (flockEnabled)
	{
		//Remove all local flocks
		for (int i = 0; i < flockList.size(); i++)
		{
			flockList[i]->mFlockMembers.clear(); //erase all members of the flock
			flockList[i]->AddFlockMember(aiObjects[i]); //add appropriate member to the flock
			aiObjects[i]->accel = glm::vec3(0.0f);
			//flockList.clear();
			//flockList.erase(std::remove(flockList.begin(), flockList.end(), flockList[i]), flockList.end());
			//delete flockList[i];
			//flockList[i] = 0;
		}

		Flock* currentFlock;

		bool hasFlock = false;

		for (int i = 0; i < aiObjects.size(); i++) //loop over each flocking entity
		{
			cMeshObject * flockShip0 = findObjectByFriendlyName("FlockShip" + std::to_string(i));

			//If the ship does not currently have a local flock, then create one
			//Flock* newFlock = GenerateFlock();
			//newFlock->AddFlockMember(flockShip0);
			currentFlock = flockList[i];

			//Now we have the entity's current flock, so...
			//Loop over all entity objects and add to current flock if close enough
			for (int c = 0; c < aiObjects.size(); c++) //loop over each flocking entity
			{
				hasFlock = false;
				if (i != c)
				{
					float dist = glm::abs(glm::distance(flockShip0->position, aiObjects[c]->position));
					//std::cout << "Distance: " << dist << std::endl;
					if (dist < 35.0f)
					{
						//for (int k = 0; k < currentFlock->mFlockMembers.size(); k++) //loop over the members of each flock
						//{
						//	if (currentFlock->mFlockMembers[k]->entity->friendlyName == aiObjects[c]->friendlyName)
						//	{
						//		hasFlock = true;
						//		break;
						//	}
						//}
						//if (!hasFlock)
						//{
							currentFlock->AddFlockMember(aiObjects[c]);
						//}
					}
				}
			}
			hasFlock = false;
		}

		//std::cout << "FlockList Size: " << flockList.size() << std::endl;

		//Now calculate all flocking behaviour
		for (int i = 0; i < flockList.size(); i++)
		{
			flockList[i]->CalculateSteering();
			//std::cout << "Flock " << i << " size of: " << flockList[i]->mFlockMembers.size() << std::endl;
			//std::cout << flockList[i]->mFlockMembers[0]->entity->velocity.x
			//	<< "--" << flockList[i]->mFlockMembers[0]->entity->velocity.y
			//	<< "--" << flockList[i]->mFlockMembers[0]->entity->velocity.z << std::endl;
		}
	}
}

void MakePath()
{
	std::pair<glm::vec3, float> segment0;
	segment0.first = glm::vec3(0.0f, 0.0f, 5.0f);
	segment0.second = 4.0f;
	path.push_back(segment0);

	segment0.first = glm::vec3(5.0f, 0.0f, 0.0f);
	segment0.second = 4.0f;
	path.push_back(segment0);

	segment0.first = glm::vec3(5.0f, 0.0f, -5.0f);
	segment0.second = 4.0f;
	path.push_back(segment0);

	segment0.first = glm::vec3(-5.0f, 0.0f, -5.0f);
	segment0.second = 4.0f;
	path.push_back(segment0);

	segment0.first = glm::vec3(-5.0f, 0.0f, 0.0f);
	segment0.second = 4.0f;
	path.push_back(segment0);

	segment0.first = glm::vec3(0.0f, 0.0f, 5.0f);
	segment0.second = 4.0f;
	path.push_back(segment0);
}

void TravelPath(float deltaTime)
{
	for (int i = 0; i < aiObjects.size(); i++)
	{
		if (travelPath)
		{
			aiObjects[i]->velocity = glm::vec3(0.0f);
		}
	}

	if ((currentSegment > (path.size() - 1)) || (currentSegment < 0))
	{
		travelPath = false;
	}

	if (travelPath)
	{
		glm::vec3 pathSegment = path[currentSegment].first; //get the current segment's direction

		float timer = path[currentSegment].second; //get the segment's timer

		CreateFormation(previousFormation);

		for (int i = 0; i < aiObjects.size(); i++)
		{
			if (pathForward)
			{
				glm::vec3 currentForward = aiObjects[i]->getQOrientation() * glm::vec3(0.0f, 0.0f, 1.0f);
				float angle = glm::orientedAngle(glm::normalize(currentForward), glm::normalize(pathSegment), glm::vec3(0.0f, 1.0f, 0.0f));
				//glm::quat look = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), pathSegment, glm::vec3(0.0f, 1.0f, 0.0f));

				glm::quat model = glm::rotate(glm::mat4(1.0f), (angle), glm::vec3(0.0, 1.0, 0.0));
				//aiObjects[i]->velocity = glm::vec3(0.0f);
				aiObjects[i]->velocity = pathSegment;
				aiObjects[i]->adjMeshOrientationQ(model);
			}
			else
			{
				glm::vec3 currentForward = aiObjects[i]->getQOrientation() * glm::vec3(0.0f, 0.0f, 1.0f);
				float angle = glm::orientedAngle(glm::normalize(currentForward), glm::normalize(-1.0f * pathSegment), glm::vec3(0.0f, 1.0f, 0.0f));
				//glm::quat look = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), pathSegment, glm::vec3(0.0f, 1.0f, 0.0f));

				glm::quat model = glm::rotate(glm::mat4(1.0f), (angle), glm::vec3(0.0, 1.0, 0.0));
				//aiObjects[i]->velocity = glm::vec3(0.0f);
				aiObjects[i]->velocity = -1.0f * pathSegment;
				aiObjects[i]->adjMeshOrientationQ(model);
			}
		}

		pathTimer += deltaTime;

		if (pathTimer > timer)
		{
			if (pathForward)
			{
				if (currentSegment < path.size())
				{
					currentSegment++;
				}
			}
			else
			{
				if (currentSegment >= 0)
				{
					currentSegment--;
				}
			}
			pathTimer = 0.0f;
		}
	}
}