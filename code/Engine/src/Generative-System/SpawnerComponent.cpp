#include "Generative-System/SpawnerComponent.h"


SpawnerComponent::SpawnerComponent(const char* songPath, glm::vec3 originPos, unsigned int entitiesCount)
{

	this->originPos = originPos;
	inactivePos = glm::vec3(-100);
	this->orbDistance = originPos.z;
	this->entitiesCount = entitiesCount;
	this->orbsSpawned = 0;
	this->songPath = songPath;
	spawnTimer=0;

	handOrbModel=new Model("res/content/models/orb2/orb2.fbx", new MaterialAsset("res/content/materials/color_green.json"));
	footOrbModel= new Model("res/content/models/orb1/orb1.fbx");
	badOrbModel= new Model("res/content/models/orbWrong/orbWrong.fbx");
	drinkModel= new Model("res/content/models/kieliszki/drink1/drink1.fbx");
	ringModel= new Model("res/content/models/orb2/obrys2.fbx", new MaterialAsset("res/content/materials/color.json"));
}

void SpawnerComponent::start()
{
	init();
	SongAnalizer::parseSong(spawnAfter, songPath, songData);
	SongAnalizer::testparseSong(spawnAfter, songPath, songData);
	AudioManager::getInstance().playSound(songPath,1);

}
void SpawnerComponent::update()
{

	spawnTimer -= deltaTime;
	if (spawnTimer < 0 && songDataIndex < songData.size()) {
        switch (songData[songDataIndex].type) {
		case sampleType::BASS:
			xRH = 1 * songData[songDataIndex].bass.x;
			yRH = 1.5 * songData[songDataIndex].bass.y;
			xLH = -1 * songData[songDataIndex].bass.y;
			yLH = 1.5 * songData[songDataIndex].bass.x;

			xRF = 1 * songData[songDataIndex].bass.x;
			yRF = -2 * songData[songDataIndex].bass.y;
			xLF = -1 * songData[songDataIndex].bass.y;
			yLF = -2 * songData[songDataIndex].bass.x;

			break;
		case sampleType::MID:

			xRH = 1.55 * songData[songDataIndex].mid.x;
			yRH = 0.16 * songData[songDataIndex].mid.y;
			xLH = -1.55 * songData[songDataIndex].mid.y;
			yLH = 0.16 * songData[songDataIndex].mid.x;

			xRF = 0.8 * songData[songDataIndex].mid.x;
			yRF = -0.8 * songData[songDataIndex].mid.y;
			xLF = -0.8 * songData[songDataIndex].mid.y;
			yLF = -0.8 * songData[songDataIndex].mid.x;

			break;

		case sampleType::CLAP:

			xRH = 1.1 * songData[songDataIndex].high.x;
			yRH = -0.5 * songData[songDataIndex].high.y;
			xLH = -1.1 * songData[songDataIndex].high.y;
			yLH = -0.5 * songData[songDataIndex].high.x;

			xRF = 0.2 * songData[songDataIndex].high.x;
			yRF = -2.25 * songData[songDataIndex].high.y;
			xLF = -0.2 * songData[songDataIndex].high.y;
			yLF = -2.25 * songData[songDataIndex].high.x;

			break;


		case sampleType::SKIP:
			xRH =0;
			yRH =0;
			xLH =0;
			yLH =0;

			xRF =0;
			yRF =0;
			xLF =0;
			yLF =0;

			break;
        }

		// raczki
		if (xRH > 0 && xRH < 2.0f && yRH > 0 && yRH < 2.5f) 
		{
			if (hasXPercentChance(20))
				spawn<BadOrb>(glm::vec3(xRH, yRH, originPos.z));
			else
				spawn<HandOrb>(glm::vec3(xRH, yRH, originPos.z));
		}
		
		if (xLH < 0 && xLH > -2.0f && yLH > 0 && yLH < 2.5f) 
		{
			if (hasXPercentChance(20))
				spawn<BadOrb>(glm::vec3(xLH, yLH, originPos.z));
			else
				spawn<HandOrb>(glm::vec3(xLH, yLH, originPos.z));
		}

		



		// nozki
		if (xRF > 0 && xRF < 2.0f && yRF < 0 && yRF > -2.5f) 
		{
			if (hasXPercentChance(20))
				spawn<BadOrb>(glm::vec3(xRF, yRF, originPos.z));
			else
				spawn<FootOrb>(glm::vec3(xRF, yRF, originPos.z));
		}
		
		if (xLF < 0 && xLF > -2.0f && yLF < 0 && yLF > -2.5f) 
		{
			if (hasXPercentChance(20))
				spawn<BadOrb>(glm::vec3(xLF, yLF, originPos.z));
			else
				spawn<FootOrb>(glm::vec3(xLF, yLF, originPos.z));
		}

        if (orbsSpawned > 100) 
		{
            spawn<Drink>(glm::vec3(-1, 1, originPos.z));
			orbsSpawned = 0;
			spawnAfter -= freqIncrement;
        }
        
        songDataIndex++;
        spawnTimer = spawnAfter;

        if (!(songDataIndex < songData.size())) songDataIndex = 0;



	}

	for (Collectable* entity : entitiesActive) {
		if (entity->position.z < originPos.z -orbDistance - 1.5) {
			AudioManager::getInstance().playSound("res/content/sounds/effects/fail1.mp3", 0.4);
			deactiveEntity(entity);
		}
		if (!entity->active) {
			deactiveEntity(entity);
		}
	}
	originPos.z += forwardSpeed * deltaTime;

}

void SpawnerComponent::init()
{
	for (int i = 0; i < entitiesCount; i++)
	{
		Collectable* handOrb = new HandOrb("handOrb", inactivePos, new Model(*handOrbModel));
		Collectable* footOrb = new FootOrb("footOrb", inactivePos, new Model(*footOrbModel));
		Collectable* badOrb = new BadOrb("badOrb", inactivePos, new Model(*badOrbModel));
		Collectable* ring = new Ring("ring", inactivePos, new Model("res/content/models/orb2/obrys2.fbx", new MaterialAsset("res/content/materials/color.json")));

		parentEntity->addChild(handOrb);
		parentEntity->addChild(footOrb);
		parentEntity->addChild(badOrb);
		parentEntity->addChild(ring);

		entitiesInactive.push_back(handOrb);
		entitiesInactive.push_back(footOrb);
		entitiesInactive.push_back(badOrb);
		entitiesInactive.push_back(ring);
	}

	Collectable* drink1 = new Drink("drink", inactivePos, new Model(*drinkModel));
	Collectable* drink2 = new Drink("drink", inactivePos, new Model(*drinkModel));
	parentEntity->addChild(drink1);
	parentEntity->addChild(drink2);
	entitiesInactive.push_back(drink1);
	entitiesInactive.push_back(drink2);


}
void SpawnerComponent::deactiveEntity(Collectable* ent) {
	ent->active = false;
	for (Entity* e : ent->getChildren()) {
		Collectable* child = dynamic_cast<Collectable*>(e);
		child->active = false;
		child->getTransform()->setPosition(inactivePos);
		child->getComponent<Model>()->getMaterial()->SetVec4("color", glm::vec4(1, 1, 1, 1));
		ent->removeChild(child);
	}
	ent->getTransform()->setPosition(inactivePos);
	entitiesInactive.push_back(ent);
	entitiesActive.erase(std::remove(entitiesActive.begin(), entitiesActive.end(), ent), entitiesActive.end());
}
void SpawnerComponent::activateEntity(Collectable* ent) {
	ent->active = true;
	entitiesActive.push_back(ent);
	entitiesInactive.erase(std::remove(entitiesInactive.begin(), entitiesInactive.end(), ent), entitiesInactive.end());
}

template<typename T>
void SpawnerComponent::spawn(glm::vec3 newPos) {
	if (newPos.x == 0 || newPos.y == 0) return;
	for (Collectable* entity : entitiesInactive) {
		if (typeid(*entity) == typeid(T)){
			entity->position = newPos;
			activateEntity(entity);
			Collectable* ring = spawnRing(glm::vec3(newPos.x,newPos.y,originPos.z-orbDistance-0.5));
			entity->addChild(ring);
			orbsSpawned++;
			break;
		}
	}
}
Collectable* SpawnerComponent::spawnRing(glm::vec3 newPos) {
	for (Collectable* entity : entitiesInactive) {
		if (typeid(*entity) == typeid(Ring)) {
			entity->position = newPos;
			activateEntity(entity);
			return entity;
		}
	}
}