#include "sandbox.h"
#include "ui_sandbox.h"

#include "DrawableVector.h"
#include "AABBDisplayer.h"
#include "SandboxObject.h"
#include "MouseCollider.h"
#include "Car.h"
#include "Tractor.h"
#include <iostream>

#include <QTimer>
#include <QCloseEvent>
#include <QDebug>
#include <QScreen>

using namespace QSFML;
using namespace QSFML::Objects;

void addLineChart(Scene* scene);
void addMouseCollider(Scene* scene);
void addShape(Scene* scene);
void addPerlinNoise(Scene* scene);
void addNastedRotatingVector(Scene* scene);
void addCar(Scene* scene,
    const sf::ContextSettings& settings,
    QWidget* qparent);

void shaderTest(Scene* scene);


SandBox::SandBox(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SandBox)
{
    ui->setupUi(this);

    m_scene_1 = nullptr;
    m_scene_2 = nullptr;
    
    if(true)
    {
        SceneSettings settings;
        //settings.layout.autoAjustSize = false;
        settings.layout.fixedSize = sf::Vector2u(300,100);
        settings.contextSettings.antialiasingLevel = 8;
        settings.timing.frameTime = 0.02;
        //settings.updateControlls.enableMultithreading = false;
        //settings.updateControlls.enablePaintLoop = false;
        //settings.updateControlls.enableEventLoop = false;
        //settings.updateControlls.enableUpdateLoop = false;
        m_scene_1 = new Scene(ui->SceneWidget_1,settings);
        //auto* cam2 = new Objects::CameraWindow("CustomCamera2", ui->secondCamera_frame);
        auto* cam3 = new Objects::CameraWindow(settings.contextSettings, "CustomCamera3", ui->thirtCamera_frame);
        DefaultEditor *defaultEditor = new DefaultEditor();

		//Objects::CameraController* camController2 = new Objects::CameraController();
		Objects::CameraController* camController3 = new Objects::CameraController();
		//cam2->addChild(camController2);
		cam3->addChild(camController3);
        //m_scene_1->addObject(cam2);
        m_scene_1->addObject(cam3);

		Objects::GameObject* zeroPoint = new Objects::GameObject();
		Components::PointPainter* pointPainter = new Components::PointPainter();
		pointPainter->setColor(sf::Color::Red);
		pointPainter->setRadius(5);
		zeroPoint->addComponent(pointPainter);
		pointPainter->addPoint(sf::Vector2f(0, 0));
		m_scene_1->addObject(zeroPoint);


        








        //VectorDisplayer *m_vecDisplay = new VectorDisplayer();

        m_scene_1->addObject(defaultEditor);
        //m_scene_1->addObject(m_vecDisplay);
        //m_scene->addObject(grid);
        //qDebug() << defaultEditor->toString().c_str();
        //qDebug() << m_vecDisplay->toString().c_str();

        //SandboxObject *sbObj = new SandboxObject();
        //m_scene_1->addObject(sbObj);

        m_pointPainter = new QSFML::Components::PointPainter();
        QSFML::Objects::GameObjectPtr GameObject = new QSFML::Objects::GameObject();
        GameObject->addComponent(m_pointPainter);
        m_scene_1->addObject(GameObject);
        GameObject->setEnabled(false);

        shaderTest(m_scene_1);
		addNastedRotatingVector(m_scene_1);

        QTimer* timer = new QTimer(this);

        connect(timer, &QTimer::timeout, this, &SandBox::onTimerFinished);
        timer->start(1000);

		
        m_scene_1->start();
    }
    
    if(true)
    {
        SceneSettings settings;
        //settings.layout.autoAjustSize = false;
        settings.layout.fixedSize = sf::Vector2u(300,100);
        settings.contextSettings.antialiasingLevel = 8;
        settings.timing.frameTime = 0.02;
        //settings.timing.frameTime = 0;
       //settings.updateControlls.enableMultithreading = false;
       //settings.updateControlls.enablePaintLoop = false;
       //settings.updateControlls.enableEventLoop = false;
       //settings.updateControlls.enableUpdateLoop = false;
        m_scene_2 = new Scene(ui->SceneWidget_2,settings);

        DefaultEditor *defaultEditor = new DefaultEditor("Editor",sf::Vector2f(2000,2000));
        defaultEditor->setRenderLayer(RenderLayer::layer_0);
        defaultEditor->setPosition(-sf::Vector2f(1000,1000));
        defaultEditor->getCamera()->setMaxMovingBounds(sf::FloatRect(-1000, -1000, 2000, 2000));

        VectorDisplayer *m_vecDisplay = new VectorDisplayer();

        //auto* cam2 = new Objects::CameraWindow(settings.contextSettings, "CustomCamera2", ui->secondCamera_frame);
        //Objects::CameraController* camController2 = new Objects::CameraController();
        //cam2->addChild(camController2);
        //m_scene_2->addObject(cam2);


        //m_scene_2->addObject(obj);
        //m_scene_2->addObject(m_vecDisplay);
        m_scene_2->addObject(defaultEditor);


        GameObjectPtr obj = new GameObject("MyObject");
        Components::Collider* collider = new Components::Collider();
        collider->setVertecies(
            {
				sf::Vector2f(0,0),
				sf::Vector2f(100,0),
				sf::Vector2f(100,50),
				sf::Vector2f(120,70),
				sf::Vector2f(100,60),
				sf::Vector2f(0,100)
			});
        obj->addComponent(collider);
        obj->addComponent(collider->createPainter());

        Components::MouseFollower* mouseFollower = new Components::MouseFollower();
        obj->addComponent(mouseFollower);
        Components::Text* text = new Components::Text();
        text->setText("Hello World");
        text->setCharacterSize(50);
        connect(mouseFollower, &Components::MouseFollower::mousePosChanged, [obj, text](const sf::Vector2f& worldPos, const sf::Vector2i& pixelPos)
            {
				//qDebug() << "MousePosChanged: " << worldPos.x << " " << worldPos.y;
				obj->setPosition(worldPos);
                text->setEnabled(!text->isEnabled());

			});
       
        obj->addComponent(text);

        GameObjectPtr child = new GameObject("Child");
        child->setPosition(sf::Vector2f(100, 100));
        Components::Shape* shape = new Components::Shape();
        shape->setPoints(
            {
				sf::Vector2f(0,0),
				sf::Vector2f(160,0),
				sf::Vector2f(100,100),
				sf::Vector2f(0,100)
			});
        shape->setFillColor(sf::Color::Green);
        shape->setFill(true);
        child->addComponent(shape);
        obj->addChild(child);




        

        m_scene_2->addObject(obj);
        



        addLineChart(m_scene_2);
        addMouseCollider(m_scene_2);
        addShape(m_scene_2);
        addPerlinNoise(m_scene_2);
        addCar(m_scene_2, settings.contextSettings, ui->secondCamera_frame);

        m_scene_2->applyObjectChanges();

        std::cout << obj->toString() << "\n";
        m_scene_2->start();
    }

    QSFML::Utilities::Ray func1(sf::Vector2f(0, 0), sf::Vector2f(0, 1));
    sf::Vector2f point = sf::Vector2f(2, 1);
    float factor = func1.getShortestDistanceFactor(point);
    sf::Vector2f pos = func1.getPoint(factor);
    float distance = func1.getShortestDistance(point);
    std::cout << "PosFactor: " << factor << " pos: " << pos.x << " " << pos.y << " distance: "<< distance<<"\n";
    
    
    
    
    
}

SandBox::~SandBox()
{
    delete m_scene_1;
    delete m_scene_2;
    delete ui;
}

void SandBox::onTimerFinished()
{
    QSFML::vector<sf::Vector2f> points;
    
    for (size_t i = 0; i < 20; ++i)
    {
        points.push_back(QSFML::Utilities::RandomEngine::getVector({ 0,0 }, { 100,100 }));
    }
    m_pointPainter->setPoints(points);
}

void SandBox::closeEvent(QCloseEvent* event)
{
    if (m_scene_1)
        m_scene_1->stop();
    if (m_scene_2)
        m_scene_2->stop();
    //Scene::stopEventLoop();
    event->accept();
}


void addLineChart(Scene* scene)
{
    LineChart* m_chart = new LineChart();

    m_chart->setOrigin(sf::Vector2f(0, 0));
    m_chart->setMaxDataPoints(100);
    m_chart->setSize(sf::Vector2f(200, 100));
    m_chart->setPosition(sf::Vector2f(300, 100));
    m_chart->setRotation(20);
    m_chart->addUpdateFunction([m_chart](GameObject&) {
        static float t = 0;
        t += 0.1;
        m_chart->addDataPoint(std::sin(t) * 50);
        m_chart->setScale(std::sin(t) * 0.5 + 1, std::sin(t) * 0.5 + 1);

        });
    scene->addObject(m_chart);
}
void addMouseCollider(Scene* scene)
{
    MouseCollider* mouseCollider = new MouseCollider("MOUSE_COLLIDER");
    scene->addObject(mouseCollider);
}
void addShape(Scene* scene)
{

    QSFML::Objects::GameObjectPtr obj = new QSFML::Objects::GameObject();
    QSFML::Components::Shape* shape = new QSFML::Components::Shape("TestShape");
    QSFML::Utilities::Ray* testRay = new QSFML::Utilities::Ray(sf::Vector2f(0, 0), sf::Vector2f(1, 1));
    QSFML::Components::LinePainter* linePainter = new QSFML::Components::LinePainter();
    linePainter->ignoreTransform(true);
    obj->setPosition(sf::Vector2f(0, 0));
    //delete testRay->createRayPainter();




    shape->setPoints(
        {
            sf::Vector2f(0,0),
            sf::Vector2f(100,0),
            sf::Vector2f(100,100),
            sf::Vector2f(0,100)
        });
    shape->setPosition(sf::Vector2f(200, 200));

    //sf::Transform t = shape->getTransform();
   // t.translate(obj->getPosition());
    //shape->setTransform(t);
    obj->addUpdateFunction([shape, testRay, obj, linePainter](GameObject&)
        {
            //sf::Transform t = shape->getTransform();
            //t.rotate(obj->getDeltaT()*300);
            //shape->setTransform(t);
            //shape->rotate(obj->getDeltaT() * 300);

            testRay->setDirection(obj->getMouseWorldPosition() - testRay->getPosition());
            //testRay->setDirection(sf::Vector2f(1,1));
            testRay->normalize();
            float d1;
            size_t d2;
            testRay->raycast(*shape, d1, d2);

            GameObjectPtr mouseFollowerObj = obj->getFirstObjectGlobal("MOUSE_COLLIDER");
            if (mouseFollowerObj)
            {
                sf::Vector2f pos = mouseFollowerObj->getPosition();
                linePainter->setPoints(obj->getPosition(), pos);
                linePainter->setColor(sf::Color::Green);
            }
            else
            {
                linePainter->setPoints(obj->getPosition(), obj->getMouseWorldPosition());
                linePainter->setColor(sf::Color::Red);
            }
        });
    obj->setRenderLayer(RenderLayer::layer_2);
    shape->setFillColor(sf::Color::Red);
    shape->setOutlineColor(sf::Color::Blue);
    //shape->setOutlineThickness(5);

    shape->setFill(true);
    obj->addComponent(linePainter);
    //


    obj->addComponent(testRay->createRayPainter());
    obj->addComponent(shape);

    scene->addObject(obj);
}
void addPerlinNoise(Scene* scene)
{
    GameObjectPtr obj = new GameObject();

    sf::Vector2u size(100, 100);
    QSFML::Utilities::PerlinNoise* perlinNoise = new QSFML::Utilities::PerlinNoise(0);
    QSFML::Components::PixelPainter* pixelPainter = new QSFML::Components::PixelPainter();
    pixelPainter->setPixelCount(size);
    pixelPainter->setPixelSize(2);
    pixelPainter->setPosition(sf::Vector2f(0, 0));


    for (size_t x = 0; x < size.x; ++x)
    {
        for (size_t y = 0; y < size.y; ++y)
        {
            float value = perlinNoise->noise((float)x, (float)y, 2, sf::Vector2u(20, 20));
            value += perlinNoise->noise((float)x, (float)y, 8, sf::Vector2u(10, 10));
            if (value < -1)
                value = -1;
            else if (value > 1)
                value = 1;

            value = (value + 1) / 2;


            sf::Color color = QSFML::Color::lerpCubic({ sf::Color::Red, sf::Color::Green, sf::Color::Blue }, value);
            pixelPainter->setPixel(sf::Vector2u(x, y), color);
        }
    }

    //obj->setUpdateFunction([obj, perlinNoise, pixelPainter]()
    //    {
    //
    //	});
    obj->setRenderLayer(RenderLayer::layer_2);
    obj->addComponent(pixelPainter);
    //obj->setPosition(sf::Vector2f(300, 100));
    pixelPainter->setPosition(sf::Vector2f(300, 100));
    scene->addObject(obj);
}
void addNastedRotatingVector(Scene* scene)
{
    GameObjectPtr root = new GameObject("RotatingVector");

    GameObjectPtr _root = root;

    float length = 500;
    for (int i = 0; i < 30; ++i)
    {
        GameObjectPtr child = new AABBDisplayer(length / (i + 1), "RotatingVector");
        //root->addChild(child);
        if (i > 0)
        {
            child->setPosition(sf::Vector2f(length / i, 0));
        }
        _root->addChild(child);
        _root = child;
    }
    root->setPosition(sf::Vector2f(400, 300));
    scene->addObject(root);
    root->updateObjectChanges();
    qDebug() << root->toString().c_str();
}
void addCar(Scene* scene,
    const sf::ContextSettings& settings,
    QWidget* qparent)
{
    //Tractor* tractor = new Tractor(settings, qparent);
    //scene->addObject(tractor);
    Car* car = new Car(settings, qparent);
    scene->addObject(car);

    //QSFML::vector< Objects::CameraWindow*> cams = car->getChildsRecusrive<Objects::CameraWindow>();

}
void shaderTest(Scene* scene)
{
    std::string shaderStr =
        "uniform vec2 u_resolution; // The resolution of the screen           \n"
        "//uniform vec2 u_offset;     // Offset for panning                   \n"
        "//uniform float u_zoom;      // Zoom level                           \n"
        "//uniform int u_maxIterations; // Maximum number of iterations       \n"
        "uniform vec2 rectPosition;                                           \n"
        "                                                                     \n"
        "                                                                     \n"
        "void main() {                                                        \n"
        "    //vec2 u_resolution = vec2(500,500);                             \n"
        "                                                                     \n"
        "    vec2 st = (gl_FragCoord.xy-rectPosition.xy) / u_resolution.xy;                     \n"
        "    vec3 color = vec3(0.0);                                          \n"
        "                                                                     \n"
        "    // bottom-left                                                   \n"
        "    vec2 bl = smoothstep(0.0, 0.05, st);                              \n"
        "    float pct = bl.x * bl.y;                                         \n"
        "                                                                     \n"
        "    // top-right                                                     \n"
        "    vec2 tr = smoothstep(0., 0.05, 1.0 - st);                         \n"
        "    pct *= tr.x * tr.y;                                              \n"
        "                                                                     \n"
        "    color = mix(vec3(0,1,0), vec3(0,0,1), pct);                                               \n"
        "                                                                     \n"
        "    gl_FragColor = vec4(color, 1-pct);                                 \n"
        "}                                                                    \n";

	GameObjectPtr obj = new GameObject();
	sf::Shader* shader = new sf::Shader();
	shader->loadFromMemory(shaderStr, sf::Shader::Fragment);

	obj->addDrawFunction([shader](const GameObject& obj, sf::RenderTarget& target, sf::RenderStates states)
		{
            auto camera = obj.getCurrentRenderCamera();
            float areaScale = 0.5;
			QSFML::Utilities::AABB viewRect(camera->getThisCameraViewRect());
            
            sf::RectangleShape rectangle;
            rectangle.setSize(viewRect.getSize() * areaScale);
           // rectangle.setSize(sf::Vector2f(100,100));
			rectangle.setPosition(camera->getThisCameraMouseWorldPosition());

            
            sf::Vector2f pixelPos = sf::Vector2f(camera->getThisCameraMousePosition()) * camera->getThisCameraDpiScale().x;
            pixelPos -= sf::Vector2f(camera->getInThisCameraScreenSpace({ viewRect.TL().x,viewRect.TL().y + rectangle.getSize().y })) * camera->getThisCameraDpiScale().x;
            pixelPos.y = -pixelPos.y;
            
	
			rectangle.setFillColor(sf::Color::White);
            // Pass uniforms to the shader
            shader->setUniform("u_resolution", sf::Vector2f(camera->getThisCameraSize())* areaScale); // Window size as resolution
            //shader->setUniform("u_resolution", sf::Vector2f(camera->getInThisCameraScreenSpace(rectangle.getSize()))); // Window size as resolution
            shader->setUniform("rectPosition", pixelPos); // Position of the rectangle

			target.draw(rectangle, shader);
		});

	scene->addObject(obj);
}


