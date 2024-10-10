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

This library is used to create simulations or simple games. It doesn't have all the advanced features that a modern game engine supports, but it is designed to be simple and easy. The idea behind this project is to have a tool that can be used to create simulation environments for AI-based simulations.

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


## Installation
#### Dependencies

* <img src="https://raw.githubusercontent.com/wiki/KROIA/QSFML_EditorWidget/Images/Icons/arrows.png" alt="Easy Profiler"  width="40" style="vertical-align:middle;"> [easy_profiler](https://github.com/yse/easy_profiler.git) to enable performace profiling<br>
* <img src="https://raw.githubusercontent.com/wiki/KROIA/QSFML_EditorWidget/Images/Icons/chat.png" alt="Logger"  width="40" style="vertical-align:middle;"> [Logger](https://github.com/KROIA/Logger.git) to create log messages to the console<br>
* <img src="https://raw.githubusercontent.com/wiki/KROIA/QSFML_EditorWidget/Images/Icons/sfml-icon-small.png" alt="SFML"  width="40" style="vertical-align:middle;"> [SFML](https://github.com/SFML/SFML.git) as graphics library<br>
* <img src="https://raw.githubusercontent.com/wiki/KROIA/QSFML_EditorWidget/Images/Icons/QT.png" alt="QT Framework"  width="40" style="vertical-align:middle;"> [Qt Framework](https://www.qt.io/download-dev) and [Qt VisualStudio Tools](https://marketplace.visualstudio.com/items?itemName=TheQtCompany.QtVisualStudioTools2022)<br>

Download this repository and open the CMake Project with Visual Studio.<br>
After CMake has configured the Project, press **build->install**.<br>
This will build all targets and copies the nessesarry QT dll's to the build directory. 

## How to use
#### Learn more about the most important classes:

> <img src="https://raw.githubusercontent.com/wiki/KROIA/QSFML_EditorWidget/Images/Icons/gears.png" alt="Icon"  width="30" style="vertical-align:middle;"> **[Scene](https://github.com/KROIA/QSFML_EditorWidget/wiki/Scene)**<br>

> <img src="https://raw.githubusercontent.com/wiki/KROIA/QSFML_EditorWidget/Images/Icons/car.png" alt="Icon"  width="30" style="vertical-align:middle;"> **[GameObject](https://github.com/KROIA/QSFML_EditorWidget/wiki/GameObject)**<br>

> <img src="https://raw.githubusercontent.com/wiki/KROIA/QSFML_EditorWidget/Images/Icons/Components.png" alt="Icon"  width="30" style="vertical-align:middle;"> **[Component](https://github.com/KROIA/QSFML_EditorWidget/wiki/Component)**<br>
