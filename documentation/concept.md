# Concept

The library consists of 3 core element types:

- **[Canvas](Canvas.md)**
- **[CanvasObject](CanvasObject.md)**
- **[Component](Component.md)**

The Canvas contains the [SFML render window](https://www.sfml-dev.org/documentation/2.6.1/classsf_1_1RenderWindow.php). It also controlls the update logic for all objects.
Its basicly the engine of the whole thing.
The Canvas is derived from the [QWidget](https://doc.qt.io/qt-5/qwidget.html). This makes it possible to integrate the canvas very easy in a QT ui.




