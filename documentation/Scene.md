# Scene

- [Scene](#scene)
  - [Hirarchy](#hirarchy)
  - [Update loop](#update-loop)


## Hirarchy

<div style="display: inline-block; vertical-align: middle;">
A Scene is the engine of the library. It handles all the objects and updating stuff. <br>
The picture shows the Hirarchy of Objects inside a Scene.  <br>
The topmost GameObjects in the tree hirarchy are called the root objects. <br>
</div>
<div style="display: inline-block; vertical-align: middle;">
    <img src="Images/Hirarchy.png" alt="Hirarchy" width="400"/>
</div>


## Update loop


<div style="display: inline-block; vertical-align: middle;">
    The Scene is updaded using a QTimer, the update loop is shown in the picture above.
</div>
<div style="display: inline-block; vertical-align: middle;">
    <img src="Images/UpdateLoopSingle.png" alt="UpdateLoopSingle" width="200"/>
</div>


| A Scene is the engine of the library. It handles all the objects and updating stuff. The picture shows the Hirarchy of Objects inside a Scene. The topmost GameObjects in the tree hirarchy are called the root objects. | ![Image](Images/HirarchyDM.png#gh-dark-mode-only) ![Image](Images/HirarchyLM.png#gh-light-mode-only) |
|:-------------------------|-----------------------------------------|