<!-- omit in toc -->
# GameObject

- [Hirarchy](#hirarchy)
- [Predefined GameObjects](#predefined-gameobjects)
- [Create custom GameObjects](#create-custom-gameobjects)


## Hirarchy
<div style="text-align: center;">
    <img src="Images/ObjectHirarchyDM.png#gh-dark-mode-only" alt="ObjectHirarchy" width="400"/>
    <img src="Images/ObjectHirarchyLM.png#gh-light-mode-only" alt="ObjectHirarchy" width="400"/>
</div>

A **GameObject** is the base class for any object that gets added to the Scene.
GameObjects can contain multiple [Components](Component.md) and child GameObjects.
A GameObject will have a `Transform` Component by default when it gets instantiated. The Transform Component is needed to be able to move, rotate and scale a GameObject.


## Predefined GameObjects
* BackgroundGrid
* CameraController
* CameraWindow
* DefaultEditor
* LineChart
* RuntimeInfo
* VisibleCamera

## Create custom GameObjects
