#include "Flock.h"

#include <assert.h>
#include <iostream>
#include <glm/gtx/vector_angle.hpp>
// #define LOG_SYSTEMS

Flock::Flock(void)
	: cohesionWeight(1.0f / 3.0f)
	, separationWeight(1.0f / 3.0f)
	, alignmentWeight(1.0f / 3.0f)
{

}

Flock::Flock(float cohesionWeight, float separationWeight, float alignmentWeight)
	: cohesionWeight(cohesionWeight)
	, separationWeight(separationWeight)
	, alignmentWeight(alignmentWeight)
{

}

Flock::~Flock(void)
{
	// TODO: Cleanup
}


void Flock::AddFlockMember(cMeshObject* entity)
{
	FlockMemberInfo* flockMemberInfo = new FlockMemberInfo(entity);
	mFlockMembers.push_back(flockMemberInfo);
}

void Flock::RemoveFlockMember(cMeshObject* entity)
{
	// TODO: remove the flock member
}

void Flock::AdjustWeights(float alignAmt, float cohAmt, float sepAmt)
{
	this->alignmentWeight += alignAmt;
	this->cohesionWeight -= alignAmt/2;
	this->separationWeight -= alignAmt/2;

	this->alignmentWeight -= cohAmt/2;
	this->cohesionWeight += cohAmt;
	this->separationWeight -= cohAmt/2;

	this->alignmentWeight -= sepAmt/2;
	this->cohesionWeight -= sepAmt/2;
	this->separationWeight += sepAmt;

	if (this->alignmentWeight < 0.0f)
	{
		this->cohesionWeight += -1 * this->alignmentWeight / 2;
		this->separationWeight += -1 * this->alignmentWeight / 2;
		this->alignmentWeight = 0.0f;
	}
	if (this->cohesionWeight < 0.0f)
	{
		this->alignmentWeight += -1 * this->cohesionWeight / 2;
		this->separationWeight += -1 * this->cohesionWeight / 2;
		this->cohesionWeight = 0.0f;
	}
	if (this->separationWeight < 0.0f)
	{
		this->alignmentWeight += -1 * this->separationWeight / 2;
		this->cohesionWeight += -1 * this->separationWeight / 2;
		this->separationWeight = 0.0f;
	}

	this->alignmentWeight = glm::min(this->alignmentWeight, 1.0f);
	this->cohesionWeight = glm::min(this->cohesionWeight, 1.0f);
	this->separationWeight = glm::min(this->separationWeight, 1.0f);

	//normalize so that the sum is 1
	glm::vec3 weightValues = glm::vec3(this->alignmentWeight, this->cohesionWeight, this->separationWeight);
	this->alignmentWeight = weightValues.x;
	this->cohesionWeight = weightValues.y;
	this->separationWeight = weightValues.z;

	//std::cout << "Alignment Weight: " << this->alignmentWeight
	//	<< " -- Cohesion Weight: " << this->cohesionWeight
	//	<< " -- Separation Weight: " << this->separationWeight << std::endl;
}

void Flock::CalculateAlignment(FlockMemberInfo* member)
{
	alignment = glm::vec3(0.0f);
	float cos_angle = cos(glm::pi<float>()); //cone of sight
	glm::vec3 forward = glm::vec3(0.0f);

	for (std::vector<FlockMemberInfo*>::iterator itMember = mFlockMembers.begin();
		itMember != mFlockMembers.end(); itMember++)
	{
		FlockMemberInfo* currMember = *itMember;

		glm::quat playerRotation = member->entity->getQOrientation();
		glm::vec3 point = glm::normalize(currMember->entity->position - member->entity->position);
		glm::vec3 playerDir = glm::normalize(playerRotation * glm::vec3(0.0f, 0.0f, 1.0f)); //direction player is facing
		float dotproduct = glm::dot(playerDir, point);
		bool visible = dotproduct > cos_angle;

		if (visible)
		{
			if (glm::length(currMember->entity->velocity) > 0.0f)
			{
				forward = glm::normalize(currMember->entity->velocity);
				alignment += forward;
			}
		}
		else
		{
			forward = playerDir;
			alignment += forward;
		}
	}
}

void Flock::CalculateVectors(void)
{
	glm::vec3 forward = glm::vec3(0.0f);

	// Calculate the cohesion for the flock. This is the average
	// position of all of the members of the flock.
	cohesion = glm::vec3(0.0f);
	separation = glm::vec3(0.0f);
	alignment = glm::vec3(0.0f);
	for (std::vector<FlockMemberInfo*>::iterator itMember = mFlockMembers.begin();
		itMember != mFlockMembers.end(); itMember++)
	{
		FlockMemberInfo* currMember = *itMember;
		if (glm::length(currMember->entity->velocity) > 0.0f)
		{
			forward = glm::normalize(currMember->entity->velocity);
		}

		cohesion += currMember->entity->position;
		//separation += currMember->entity->position;
		for (int i = 0; i < mFlockMembers.size(); i++)
		{
			if (glm::abs(glm::distance(currMember->entity->position, mFlockMembers[i]->entity->position)) > 0.0f) 
			{
				separation += glm::normalize(currMember->entity->position - mFlockMembers[i]->entity->position);
			}
		}
		//alignment += forward;
		if (glm::length(separation) != 0) {
			separation = glm::normalize(separation);
		}
	}
}

void Flock::CalculateSteering(void)
{
	CalculateVectors();

	glm::vec3 steering = glm::vec3(0.0f);
	for (std::vector<FlockMemberInfo*>::iterator itMember = mFlockMembers.begin();
		itMember != mFlockMembers.end(); itMember++)
	{
		CalculateAlignment(*itMember);
		GetSteeringFor(*itMember, steering);

		int breakpoint = 0;

		(*itMember)->entity->accel = steering;
		//(*itMember)->entity->accel += steering;
		(*itMember)->entity->bIsUpdatedByPhysics = true;
	}
}

void Flock::GetSteeringFor(FlockMemberInfo* member, glm::vec3 &flockSteering)
{
	glm::vec3 forward = glm::vec3(0.0f);
	if (glm::length(member->entity->velocity) > 0.0f)
	{
		forward = glm::normalize(member->entity->velocity);
	}
	float sizeMinusOne = mFlockMembers.size() - 1;

	if (mFlockMembers.size() < 2)
	{
		flockSteering = forward;
	}

	// Calcuale cohesion vector
	glm::vec3 cohesionVec = glm::vec3(0.0f);
	if (sizeMinusOne > 0)
	{
		cohesionVec = glm::normalize(((cohesion - member->entity->position) / sizeMinusOne) - member->entity->position) * cohesionWeight;
	}
	else
	{
		cohesionVec = member->entity->position;
	}

	//std::cout << cohesionVec.x << "//" << cohesionVec.y << "//" << cohesionVec.z << std::endl;

	// Remove the current position from the separation vector to exclude the 
	// current flock member from the sum of positions.
	glm::vec3 separationVec = (separation - member->entity->position);
	if (sizeMinusOne > 0)
	{
		separationVec /= sizeMinusOne;
		separationVec = -glm::normalize(member->entity->position - separationVec) * separationWeight;
	}

	// Remove the current alignment from the separation vector to exclude the
	// current flock member from the sum of alignments;
	glm::vec3 alignmentVec = (alignment - forward);
	if (sizeMinusOne > 0)
	{
		alignmentVec /= sizeMinusOne;
	}

	if (glm::length(alignmentVec) > 0.0f)
	{
		alignmentVec = glm::normalize(alignmentVec) * alignmentWeight;
	}

#ifdef LOG_SYSTEMS
	printf("CohesionVec Mag=%0.4f\n", glm::length(cohesionVec));
	printf("separationVec Mag=%0.4f\n", glm::length(separationVec));
	printf("alignmentVec Mag=%0.4f\n", glm::length(alignmentVec));
#endif

	// calculate the final steering direction
	// These three vectors should add up to a normalized vector, assuming
	// the weight values add up to 1.
	flockSteering = cohesionVec + separationVec + alignmentVec;

	if (glm::length(flockSteering) == 0)
	{
		flockSteering = forward;
		//std::cout << flockSteering.x << "!!!" << flockSteering.y << "!!!" << flockSteering.z << std::endl;
	}

	glm::vec3 currentForward = member->entity->getQOrientation() * glm::vec3(0.0f, 0.0f, 1.0f);
	if (glm::length(forward) != 0)
	{
		float angle = glm::orientedAngle(glm::normalize(currentForward), glm::normalize(forward), glm::vec3(0.0f, 1.0f, 0.0f));
		//glm::quat look = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), pathSegment, glm::vec3(0.0f, 1.0f, 0.0f));

		glm::quat model = glm::rotate(glm::mat4(1.0f), (angle), glm::vec3(0.0, 1.0, 0.0));
		//aiObjects[i]->velocity = glm::vec3(0.0f);
		member->entity->adjMeshOrientationQ(model);
	}
}

Flock::FlockMemberInfo::FlockMemberInfo(cMeshObject* entity)
	: entity(entity)
{

}
Flock::FlockMemberInfo::~FlockMemberInfo(void)
{
}
