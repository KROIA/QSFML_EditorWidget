#include "ExampleScene.h"

using namespace QSFML;
using namespace QSFML::Objects;

/*
   This tutorial creates a empty GameObject that displays a ImGui window.

   What you will learn:
   - How to use ImGui in a GameObject

*/
void ExampleScene::setupTutorial_5(QSFML::Scene* scene)
{
	scene->getSceneLogger().logInfo("Tutorial 5 started");


	// Instantiate a GameObject with the name "MyObject"
    QSFML::Objects::GameObject* object = new QSFML::Objects::GameObject("MyObject");

	// Add a custom draw function to display a ImGui window
	object->addDrawFunction([](const GameObject& obj, sf::RenderTarget&, sf::RenderStates)
	{
#if IMGUI_SFML_LIBRARY_AVAILABLE == 1
		// Draw a demo ImGui window
		ImGui::Begin("Hello, world!");
		ImGui::Text("This is some useful text.");
		ImGui::End();
#endif
	});

	scene->addObject(object);
}