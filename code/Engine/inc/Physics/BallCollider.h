#pragma once
#include "ECS/ScriptableEntity.h"

class BallCollider : public ScriptableEntity {

	void onTriggerEnter(ColliderComponent*) override {
	
	};
	virtual void onTriggerStay(ColliderComponent*) {
	
	};
	virtual void onTriggerExit(ColliderComponent*) {
	
	};

};