#include "Generative-System/SpawnerComponent.h"


SpawnerComponent::SpawnerComponent(std::string songPath, glm::vec3 originPos, unsigned int entitiesCount)
{

	this->originPos = originPos;
	inactivePos = glm::vec3(-100);
	this->orbDistance = originPos.z;
	this->entitiesCount = entitiesCount;
	this->orbsSpawned = 0;
	this->songPath = songPath;
	spawnTimer=0;

	handOrbModel =new Model("res/content/models/orbUP/orbUP.fbx");
	footOrbModel = new Model("res/content/models/orbDOWN/orbDown.fbx");
	badOrbModel= new Model("res/content/models/orbX/orbX.fbx");
	drinkModel= new Model("res/content/models/kieliszki/drink1/drink1_re.fbx");
	ringXModel = new Model("res/content/models/orb2/obrys2.fbx", new MaterialAsset("res/content/materials/color.json"));
	ringUPModel = new Model("res/content/models/orb2/obrys2.fbx", new MaterialAsset("res/content/materials/color.json"));
	ringDOWNModel= new Model("res/content/models/orb2/obrys2.fbx", new MaterialAsset("res/content/materials/color.json"));
}

void SpawnerComponent::start()
{
	SongAnalizer::parseSong(spawnAfter, pathToSong, songData);
	SongAnalizer::testparseSong(spawnAfter, pathToSong, songData);
	//AudioManager::getInstance().playSound(songPath,1);
    //i put this here so played song will be in sync wit spawning orbs
    AudioManager::getInstance().playThisSound("bicik",pathToSong, 1.0f);
    AudioManager::getInstance().playThisSong("bicik");

}


void SpawnerComponent::update()
{
    //i put this here so played song will be in sync wit spawning orbs
    AudioManager::getInstance().playThisSong("bicik");

	if (!started) {
		start();
		started = true;
	}


	spawnTimer -= deltaTime;
	if (spawnTimer < 0 && songDataIndex < songData.size()) {

		/*cout << "Type: " << songData[songDataIndex].type << endl;
		cout << "Bass: " << songData[songDataIndex].bass.x << " " << songData[songDataIndex].bass.y << endl;
		cout << "Mid: " << songData[songDataIndex].mid.x << " " << songData[songDataIndex].mid.y << endl;
		cout << "High: " << songData[songDataIndex].high.x << " " << songData[songDataIndex].high.y << endl << endl;*/

        switch (songData[songDataIndex].type) {
		case sampleType::BASS:
			xRH = 1 * songData[songDataIndex].bass.x;
			yRH = 1.5 * songData[songDataIndex].bass.y;
			xLH = -1 * songData[songDataIndex].bass.y;
			yLH = 1.5 * songData[songDataIndex].bass.x;

			xRF = 1 * songData[songDataIndex].bass.y;
			yRF = -2 * songData[songDataIndex].bass.x;
			xLF = -1 * songData[songDataIndex].bass.x;
			yLF = -2 * songData[songDataIndex].bass.y;

			break;
		case sampleType::MID:

			xRH = 1.55 * songData[songDataIndex].mid.x;
			yRH = 0.16 * songData[songDataIndex].mid.y;
			xLH = -1.55 * songData[songDataIndex].mid.y;
			yLH = 0.16 * songData[songDataIndex].mid.x;

			xRF = 0.8 * songData[songDataIndex].mid.y;
			yRF = -0.8 * songData[songDataIndex].mid.x;
			xLF = -0.8 * songData[songDataIndex].mid.x;
			yLF = -0.8 * songData[songDataIndex].mid.y;

			break;

		case sampleType::CLAP:

			xRH = 1.1 * songData[songDataIndex].high.x;
			yRH = -0.5 * songData[songDataIndex].high.y;
			xLH = -1.1 * songData[songDataIndex].high.y;
			yLH = -0.5 * songData[songDataIndex].high.x;

			xRF = 0.2 * songData[songDataIndex].high.y;
			yRF = -2.25 * songData[songDataIndex].high.x;
			xLF = -0.2 * songData[songDataIndex].high.x;
			yLF = -2.25 * songData[songDataIndex].high.y;

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
		if (xRH > -0.5 && xRH < 2.0f && yRH > -0.5 && yRH < 2.5f)
		{
			checkAndAdjustForOverlap(xRH, yRH, xLH, yLH, orbRadius);
			checkAndAdjustForOverlap(xRH, yRH, xLF, yLF, orbRadius);
			checkAndAdjustForOverlap(xRH, yRH, xRF, yRF, orbRadius);

			if (hasXPercentChance(badOrbChance))
				spawn<BadOrb>(glm::vec3(xRH, yRH, originPos.z));
			else
				spawn<HandOrb>(glm::vec3(xRH, yRH, originPos.z));
		}
		
		if (xLH < 0.5 && xLH > -2.0f && yLH > -0.5 && yLH < 2.5f) 
		{
			checkAndAdjustForOverlap(xLH, yLH, xRH, yRH, orbRadius);
			checkAndAdjustForOverlap(xLH, yLH, xLF, yLF, orbRadius);
			checkAndAdjustForOverlap(xLH, yLH, xRF, yRF, orbRadius);
			if (hasXPercentChance(badOrbChance))
				spawn<BadOrb>(glm::vec3(xLH, yLH, originPos.z));
			else
				spawn<HandOrb>(glm::vec3(xLH, yLH, originPos.z));
		}




		// nozki 
		if (xRF > -0.5 && xRF < 2.0f && yRF < 0.5 && yRF > -2.5f)
		{
			float adjustedYRF = std::min(yRF, std::max(yRH, yLH)); 
			checkAndAdjustForOverlap(xRF, yRF, xRH, yRH, orbRadius);
			checkAndAdjustForOverlap(xRF, yRF, xLF, yLF, orbRadius);
			checkAndAdjustForOverlap(xRF, yRF, xLH, yLH, orbRadius);
			if (hasXPercentChance(badOrbChance))
				spawn<BadOrb>(glm::vec3(xRF, adjustedYRF, originPos.z));
			else
				spawn<FootOrb>(glm::vec3(xRF, adjustedYRF, originPos.z));
		}

		if (xLF < 0.5 && xLF > -2.0f && yLF < 0.5 && yLF > -2.5f)
		{
			checkAndAdjustForOverlap(xLF, yLF, xRH, yRH, orbRadius);
			checkAndAdjustForOverlap(xLF, yLF, xRF, yRF, orbRadius);
			checkAndAdjustForOverlap(xLF, yLF, xLH, yLH, orbRadius);
			float adjustedYLF = std::min(yLF, std::max(yRH, yLH));
			if (hasXPercentChance(badOrbChance))
				spawn<BadOrb>(glm::vec3(xLF, adjustedYLF, originPos.z));
			else
				spawn<FootOrb>(glm::vec3(xLF, adjustedYLF, originPos.z));
		}

        if (orbsSpawned > 100) 
		{
            spawn<Drink>(glm::vec3(-1, 1, originPos.z));
			orbsSpawned = 0;
			spawnAfter -= freqIncrement;
        }
        
        spawnTimer = spawnAfter;

		if (!(songDataIndex <= songData.size())) {
		
			cout << "YOU WIN!" << endl;
			//zmiana sceny i odkomentować started zeby przy nastepnej piosence tez sie wywołał start();
			//started = false;
		}
		else {
			songDataIndex++;
		}
	}

	for (Collectable* entity : entitiesActive) {
		if (entity->position.z < originPos.z -orbDistance - 1.5) {

			if (entity->getName() == "badOrb") {
				AudioManager::getInstance().playSound("res/content/sounds/effects/pop1.wav", 0.4);
				score += 100;
				combo += 1;
			}
			if (entity->getName() == "handOrb" || entity->getName() == "footOrb") {
				AudioManager::getInstance().playSound("res/content/sounds/effects/fail1.mp3", 0.4);
				combo = 0;
			}
			deactiveEntity(entity);
		}
		if (!entity->active) {
			deactiveEntity(entity);
		}
	}
    if(deltaTime<1)
	originPos.z += globalVelocity * deltaTime;

}

void SpawnerComponent::init()
{
	for (int i = 0; i < entitiesCount; i++)
	{
		Collectable* handOrb = new HandOrb("handOrb", inactivePos, new Model(*handOrbModel));
		Collectable* footOrb = new FootOrb("footOrb", inactivePos, new Model(*footOrbModel));
		Collectable* badOrb = new BadOrb("badOrb", inactivePos, new Model(*badOrbModel));
		Collectable* handRing = new Ring("handRing", inactivePos, new Model(*ringUPModel));
		Collectable* footRing = new Ring("footRing", inactivePos, new Model(*ringDOWNModel));
		Collectable* badRing = new Ring("badRing", inactivePos, new Model(*ringXModel));
		
		handOrb->getTransform()->rotate(glm::vec3(180,0,0));
		footOrb->getTransform()->rotate(glm::vec3(180, 0, 0));

		parentEntity->addChild(handOrb);
		parentEntity->addChild(footOrb);
		parentEntity->addChild(badOrb);
		parentEntity->addChild(handRing);
		parentEntity->addChild(footRing);
		parentEntity->addChild(badRing);

		entitiesInactive.push_back(handOrb);
		entitiesInactive.push_back(footOrb);
		entitiesInactive.push_back(badOrb);
		entitiesInactive.push_back(footRing);
		entitiesInactive.push_back(handRing);
		entitiesInactive.push_back(badRing);
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
		//child->getComponent<Model>()->getMaterial()->SetVec4("materialColor", glm::vec4(1, 1, 1, 1));
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
			Collectable* ring;
			if(entity->getName() == "handOrb")
				ring = spawnRing(glm::vec3(newPos.x,newPos.y,originPos.z-orbDistance-0.5),"handRing");
			else if (entity->getName() == "footOrb")
				 ring = spawnRing(glm::vec3(newPos.x, newPos.y, originPos.z - orbDistance - 0.5), "footRing");
			else if (entity->getName() == "badOrb")
				ring = spawnRing(glm::vec3(newPos.x, newPos.y, originPos.z - orbDistance - 0.5), "badRing");
			else
				 ring = spawnRing(glm::vec3(newPos.x, newPos.y, originPos.z - orbDistance - 0.5), "badRing");
			entity->addChild(ring);
			orbsSpawned++;
			break;
		}
	}
}
Collectable* SpawnerComponent::spawnRing(glm::vec3 newPos,std::string ringType) {
	for (Collectable* entity : entitiesInactive) {
		if (typeid(*entity) == typeid(Ring)&& entity->getName() == ringType) {
			entity->position = newPos;
			activateEntity(entity);
			return entity;
		}
	}
}