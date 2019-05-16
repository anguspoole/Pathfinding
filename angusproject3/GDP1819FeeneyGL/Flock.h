#ifndef FLOCK_H
#define FLOCK_H

#include "cMeshObject.h"
#include <vector>

#include <glm/glm.hpp>

enum FormationType
{
	NONE,
	CIRCLE,
	V,
	SQUARE,
	LINE,
	TWO_ROWS
};

class Flock
{
public:
	struct FlockMemberInfo
	{
		FlockMemberInfo(cMeshObject* entity);
		~FlockMemberInfo(void);

		cMeshObject* entity;
	};


	Flock(void);
	Flock(float cohesionWeight, float separationWeight, float alignmentWeight);
	virtual ~Flock(void);

	void AddFlockMember(cMeshObject* entity);
	void RemoveFlockMember(cMeshObject* entity);

	void AdjustWeights(float alignAmt, float cohAmt, float sepAmt);

	void CalculateSteering(void);

	std::vector<FlockMemberInfo*> mFlockMembers;

private:
	void CalculateAlignment(FlockMemberInfo* member);
	void CalculateVectors(void);
	void GetSteeringFor(FlockMemberInfo* member, glm::vec3 &flockSteering);

	glm::vec3 cohesion = glm::vec3(0.0f);
	glm::vec3 separation = glm::vec3(0.0f);
	glm::vec3 alignment = glm::vec3(0.0f);

	float cohesionWeight = 0.0f;
	float alignmentWeight = 0.0f;
	float separationWeight = 0.0f;
};

#endif
