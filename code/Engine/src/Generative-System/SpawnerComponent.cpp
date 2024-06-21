#include "Generative-System/SpawnerComponent.h"


SpawnerComponent::SpawnerComponent(const char* songPath, glm::vec3 originPos, unsigned int entitiesCount=14)
{

	this->originPos = originPos;
	vanishTreshold = glm::vec3(originPos.x, originPos.y, originPos.z - 6.5);
	playerPos = glm::vec3(originPos.x, originPos.y, originPos.z - 5);
	ringPos = glm::vec3(playerPos.x, playerPos.y, playerPos.y-0.5);
	inactivePos = glm::vec3(-100);
	this->entitiesCount = entitiesCount;
	this->orbsSpawned = 0;
	this->songPath = songPath;
	spawnTimer=0;

	handOrbModel=new Model("res/content/models/orb2/orb2.fbx");
	footOrbModel= new Model("res/content/models/orb1/orb1.fbx");
	badOrbModel= new Model("res/content/models/orbWrong/orbWrong.fbx");
	drinkModel= new Model("res/content/models/kieliszki/drink1/drink1.fbx");
	ringModel= new Model("res/content/models/orb2/obrys2.fbx");
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
        float z = 5;
        switch (songData[songDataIndex].type) {
        case sampleType::BASS:
            // raczki
            
            if (hasXPercentChance(20))
                spawn<BadOrb>(glm::vec3(1 * songData[songDataIndex].bass.x, 1.5 * songData[songDataIndex].bass.y, z));
            else
                spawn<HandOrb>(glm::vec3(1 * songData[songDataIndex].bass.x, 1.5 * songData[songDataIndex].bass.y, z));
            if (hasXPercentChance(20))
                spawn<BadOrb>(glm::vec3(-1 * songData[songDataIndex].bass.y, 1.5 * songData[songDataIndex].bass.x, z));
            else
                spawn<HandOrb>(glm::vec3(-1 * songData[songDataIndex].bass.y, 1.5 * songData[songDataIndex].bass.x, z));


            // nozki
            if (hasXPercentChance(20))
                spawn<BadOrb>(glm::vec3(1, -2, z));
            else
                spawn<FootOrb>(glm::vec3(1, -2, z));
            if (hasXPercentChance(20))
                spawn<BadOrb>(glm::vec3(-1, -2, z));
            else
                spawn<FootOrb>(glm::vec3(-1, -2, z));



            break;
        case sampleType::MID:
            // raczki
            if (hasXPercentChance(20))
                spawn<BadOrb>(glm::vec3(1.55 * songData[songDataIndex].mid.x, 0.16 * songData[songDataIndex].mid.y, z));
            else
                spawn<HandOrb>(glm::vec3(1.55 * songData[songDataIndex].mid.x, 0.16 * songData[songDataIndex].mid.y, z));
            if (hasXPercentChance(20))
                spawn<BadOrb>(glm::vec3(-1.55 * songData[songDataIndex].mid.y, 0.16 * songData[songDataIndex].mid.x, z));
            else
                spawn<HandOrb>(glm::vec3(-1.55 * songData[songDataIndex].mid.y, 0.16 * songData[songDataIndex].mid.x, z));

            // nozki
            if (hasXPercentChance(20))
                spawn<BadOrb>(glm::vec3(0.8, -0.8, z));
            else
                spawn<FootOrb>(glm::vec3(0.8, -0.8, z));
            if (hasXPercentChance(20))
                spawn<BadOrb>(glm::vec3(-0.8, -0.8, z));
            else
                spawn<FootOrb>(glm::vec3(-0.8, -0.8, z));

            break;
        case sampleType::CLAP:
            // raczki
            if (hasXPercentChance(20)) 
                spawn<BadOrb>(glm::vec3(1.1 * songData[songDataIndex].high.x, -0.5 * songData[songDataIndex].high.y, z));
            else 
                spawn<HandOrb>(glm::vec3(1.1 * songData[songDataIndex].high.x, -0.5 * songData[songDataIndex].high.y, z));
            if (hasXPercentChance(20)) 
                spawn<BadOrb>(glm::vec3(-1.1 * songData[songDataIndex].high.y, -0.5 * songData[songDataIndex].high.x, z));
            else
                spawn<HandOrb>(glm::vec3(-1.1 * songData[songDataIndex].high.y, -0.5 * songData[songDataIndex].high.x, z));
            // nozki
            if (hasXPercentChance(20))
                spawn<BadOrb>(glm::vec3(0.2, -2.25, z));
            else
                spawn<FootOrb>(glm::vec3(0.2, -2.25, z));
            if (hasXPercentChance(20))
                spawn<BadOrb>(glm::vec3(-0.2, -2.25, z));
            else
                spawn<FootOrb>(glm::vec3(-0.2, -2.25, z));
            break;
        case sampleType::SKIP:
            break;
        }

        if (orbsSpawned > 50) {
            spawn<Drink>(glm::vec3(-1, 1, 6));
			orbsSpawned = 0;
        }

        
        songDataIndex++;
        spawnTimer = spawnAfter;

        if (!(songDataIndex < songData.size())) songDataIndex = 0;

	}

	for (Collectable* entity : entitiesActive) {
		if (entity->position.z < vanishTreshold.z) {
			AudioManager::getInstance().playSound("res/content/sounds/effects/fail1.mp3", 0.4);
			deactiveEntity(entity);
		}
		if (!entity->active) {
			deactiveEntity(entity);
		}
	}
}

void SpawnerComponent::init()
{
	for (int i = 0; i < entitiesCount; i++)
	{
		Collectable* handOrb = new HandOrb("handOrb", inactivePos, new Model(*handOrbModel));
		Collectable* footOrb = new FootOrb("footOrb", inactivePos, new Model(*footOrbModel));
		Collectable* badOrb = new BadOrb("badOrb", inactivePos, new Model(*badOrbModel));
		Collectable* ring = new Ring("ring", inactivePos, new Model(*ringModel));

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
	for (Entity* child : ent->getChildren()) {
		child->active = false;
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
	for (Collectable* entity : entitiesInactive) {
		if (typeid(*entity) == typeid(T)){
			entity->position = newPos;
			activateEntity(entity);
			Collectable* ring = spawnRing(glm::vec3(newPos.x,newPos.y,ringPos.z));
			//entity->addChild(ring);
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