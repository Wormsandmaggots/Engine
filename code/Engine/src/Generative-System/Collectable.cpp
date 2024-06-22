#include "Generative-System/Collectable.h"

Collectable::Collectable(const std::string name, const glm::vec3& position, Model* model) :Entity(name) {
	this->collider = new ColliderComponent();
	this->model = model;
	collider->setParent(this);
	collider->start();
	collider->getCollider()->getColliderShape()->setRadius(0.6);
	this->position = position;
	this->getTransform()->setPosition(position);
	Entity::addComponent(model);
	this->addComponent(collider);
}

void Collectable::setChildInactive() {
	if (getChildren()[0] != nullptr) {
		getChildren()[0]->active = false;
	}
}

void Collectable::addComponent(Component* c)
{
	Entity::addComponent(c);

	if (strcmp(typeid(*c).name(), typeid(ColliderComponent).name()) == 0)
	{
		Collider* addedCollider = dynamic_cast<ColliderComponent*>(c)->getCollider();

		addedCollider->setOnCollision([this](ColliderComponent* collider) { onTriggerStay(collider); });
		addedCollider->setOnCollisionStart([this](ColliderComponent* collider) { onTriggerEnter(collider); });
		addedCollider->setOnCollisionExit([this](ColliderComponent* collider) { onTriggerExit(collider); });
	}

}
