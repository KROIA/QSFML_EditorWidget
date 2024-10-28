<!-- omit in toc -->
# QSFML EditorWidget
- [About](#about)
- [Features](#features)
- [Installation](#installation)
    - [Dependencies](#dependencies)
- [How to use](#how-to-use)
    - [Learn more about the most important classes:](#learn-more-about-the-most-important-classes)




## About
> <img src="https://raw.githubusercontent.com/wiki/KROIA/QSFML_EditorWidget/Images/Icons/bookshelf.png" alt="QT_cmake_library_template"  width="40" style="vertical-align:middle;"> This library was created using the [library template](https://github.com/KROIA/QT_cmake_library_template)

This library is used to create simulations or simple games. It doesn't have all the advanced features that a modern game engine supports, but it is designed to be simple and easy. The idea behind this project is to have a tool that can be used to create simulation environments for AI-based or physics simulations.


<table>
<tr>
<td>
<div align="center">
    <img src="https://raw.githubusercontent.com/wiki/KROIA/QSFML_EditorWidget/Images/Gravity.gif" > 
</div>
</td>
<td>
<div align="center">
    <img src="https://raw.githubusercontent.com/wiki/KROIA/QSFML_EditorWidget/Images/Lightsim.gif" > 
</div>
</td>
</tr>
<tr>
<td>
<div align="center">
    <img src="https://raw.githubusercontent.com/wiki/KROIA/QSFML_EditorWidget/Images/Pendulum.gif" > 
</div>
</td>
<td>
<div align="center">
    <img src="https://raw.githubusercontent.com/wiki/KROIA/QSFML_EditorWidget/Images/CollisionExample.gif" > 
</div>
</td>
</tr>
    <td>
<div align="center">
    <img src="https://raw.githubusercontent.com/wiki/KROIA/QSFML_EditorWidget/Images/TileMap.gif" > 
</div>
</td>
<td>
<div align="center">
    <img src="https://raw.githubusercontent.com/wiki/KROIA/QSFML_EditorWidget/Images/PathFinder.gif" > 
</div>
</td>
</tr>
</table>


## Features
* Easy integration into a QT widget
* GameObjects organized in a tree structure
* Multi-threaded GameObject updates for complex scenes (if enabled)
* GameObjects can be customized without creating an inherited class
* Components allow you to add special functionality to a GameObject.
* Collision detection for polygon shapes using Collider component.
* Multiple camera views can be used in one scene.
* Predefined Components ready to use.
* Disable specific parts of the update loop for all or individual objects to speed up the engine
* [Dear Im Gui](https://github.com/ocornut/imgui) compatible
* Built in performance [profiler](https://github.com/yse/easy_profiler)

## Installation
#### Dependencies

* <img src="https://raw.githubusercontent.com/wiki/KROIA/QSFML_EditorWidget/Images/Icons/arrows.png" alt="Easy Profiler"  width="40" style="vertical-align:middle;"> [easy_profiler](https://github.com/yse/easy_profiler.git) to enable performace profiling<br>
* <img src="https://raw.githubusercontent.com/wiki/KROIA/QSFML_EditorWidget/Images/Icons/chat.png" alt="Logger"  width="40" style="vertical-align:middle;"> [Logger](https://github.com/KROIA/Logger.git) to create log messages to the console<br>
* <img src="https://raw.githubusercontent.com/wiki/KROIA/QSFML_EditorWidget/Images/Icons/sfml-icon-small.png" alt="SFML"  width="40" style="vertical-align:middle;"> [SFML](https://github.com/SFML/SFML.git) as graphics library<br>
* <img src="https://raw.githubusercontent.com/wiki/KROIA/QSFML_EditorWidget/Images/Icons/QT.png" alt="QT Framework"  width="40" style="vertical-align:middle;"> [Qt Framework](https://www.qt.io/download-dev) and [Qt VisualStudio Tools](https://marketplace.visualstudio.com/items?itemName=TheQtCompany.QtVisualStudioTools2022)<br>

##### Optional
* <a href="https://github.com/ocornut/imgui"><img src="https://raw.githubusercontent.com/wiki/KROIA/QSFML_EditorWidget/Images/Icons/ui-design.png" alt="SFML"  width="40" style="vertical-align:middle;"></a> [ImGui](https://github.com/ocornut/imgui) as additional UI extention<br>
* <a href="https://github.com/electronicarts/EASTL"><img src="https://raw.githubusercontent.com/wiki/KROIA/QSFML_EditorWidget/Images/Icons/ea.png" alt="SFML"  width="40" style="vertical-align:middle;"></a> [EA STL](https://github.com/electronicarts/EASTL) faster container tamplates<br>
* <a href="https://github.com/Auburn/FastNoiseLite"><img src="https://raw.githubusercontent.com/wiki/KROIA/QSFML_EditorWidget/Images/Icons/wurfel.png" alt="SFML"  width="40" style="vertical-align:middle;"></a> [FastNoiseLite](https://github.com/Auburn/FastNoiseLite) a fast portable noise library <br>

Download this repository and open the root CMakeLists.txt Project with Visual Studio.<br>
After CMake has configured the Project, press **build->install**.<br>
This will build all targets and copies the nessesarry QT dll's to the build directory. 

## How to use
### Quickstart
``` C++ 
#include <QApplication>
#include "QSFML_EditorWidget.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    // Create a QWidget to show the Scene in
    QWidget screenWidget;
    screenWidget.resize(500, 500);

    // Create the scene that holds all GameObjects
    QSFML::SceneSettings settings;
    settings.timing.frameTime = 0.03; // ms -> ~60 FPS
    QSFML::Scene scene(&screenWidget, settings);	

    // Create a editor GameObject, it creates a simple grid and manages 
    // camera movement.
    QSFML::Objects::DefaultEditor* editor = new QSFML::Objects::DefaultEditor("Editor", sf::Vector2f(1000, 800));
    scene.addObject(editor); // Add the object to the scene

    // Create a empty GameObject
    QSFML::Objects::GameObject* object = new QSFML::Objects::GameObject("MyObject");

    // Add a custom update lambda function to that empty GameObject
    object->addUpdateFunction([](QSFML::Objects::GameObject& obj)
    {
        // obj is a ref to it self, rotate it self a bit
        obj.rotate(obj.getDeltaT() * 50.f);
    });
    
    // Add a custom draw lambda function to the GameObject
    object->addDrawFunction([](const QSFML::Objects::GameObject& obj, sf::RenderTarget& target, sf::RenderStates states)
    {
        // Create a square to visualize the rotating object
        sf::RectangleShape shape(sf::Vector2f(400, 400));
        shape.setPosition(-200, -200);
        shape.setFillColor(sf::Color::Red);
        target.draw(shape, states);
    });

    // Add the GameObject to the scene
    scene.addObject(object);

    // Start the updat loop of the scene
    scene.start();
    screenWidget.show();
    return a.exec();
}
```

<div align="center">
    <img src="https://raw.githubusercontent.com/wiki/KROIA/QSFML_EditorWidget/Images/QuickStart.gif" > 
</div>

#### Visit the tutorials:

> <img src="https://raw.githubusercontent.com/wiki/KROIA/QSFML_EditorWidget/Images/Icons/gears.png" alt="Icon"  width="30" style="vertical-align:middle;"> **[Scene](https://github.com/KROIA/QSFML_EditorWidget/wiki/Scene)**<br>
>> <img src="https://raw.githubusercontent.com/wiki/KROIA/QSFML_EditorWidget/Images/Icons/car.png" alt="Icon"  width="30" style="vertical-align:middle;"> **[GameObject](https://github.com/KROIA/QSFML_EditorWidget/wiki/GameObject)**<br>
>>> <img src="https://raw.githubusercontent.com/wiki/KROIA/QSFML_EditorWidget/Images/Icons/Components.png" alt="Icon"  width="30" style="vertical-align:middle;"> **[Component](https://github.com/KROIA/QSFML_EditorWidget/wiki/Component)**<br>

> <img src="https://raw.githubusercontent.com/wiki/KROIA/QSFML_EditorWidget/Images/Icons/chat.png" alt="Logger"  width="30" style="vertical-align:middle;"> **[Logging to console](https://github.com/KROIA/QSFML_EditorWidget/wiki/Logging)**<br>

> <img src="https://raw.githubusercontent.com/wiki/KROIA/QSFML_EditorWidget/Images/Icons/arrows.png" alt="Profiler"  width="40" style="vertical-align:middle;"> **[Profiling the application](https://github.com/KROIA/QSFML_EditorWidget/wiki/Profiling)**<br>

> <a href="https://github.com/KROIA/QSFML_EditorWidget/wiki/ImGuiIntegration"><img src="https://raw.githubusercontent.com/wiki/KROIA/QSFML_EditorWidget/Images/Icons/ui-design.png" alt="ImGui"  width="30" style="vertical-align:middle;"></a> **[Im Gui Integration](https://github.com/KROIA/QSFML_EditorWidget/wiki/ImGuiIntegration)**<br>

> <a href="https://github.com/KROIA/QSFML_EditorWidget/wiki/Profiling"><img src="https://raw.githubusercontent.com/wiki/KROIA/QSFML_EditorWidget/Images/Icons/ea.png" alt="EA STL"  width="30" style="vertical-align:middle;"></a> **[Using EA STL](https://github.com/KROIA/QSFML_EditorWidget/wiki/eastlIntegration)**<br>

