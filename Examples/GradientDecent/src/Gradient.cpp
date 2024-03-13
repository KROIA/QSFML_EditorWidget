#include "Gradient.h"



static double f(sf::Vector2f p);
static double dfx(sf::Vector2f p);
static double dfy(sf::Vector2f p);
 
static double dfxx(sf::Vector2f p);
static double dfxy(sf::Vector2f p);
static double dfyx(sf::Vector2f p);
static double dfyy(sf::Vector2f p);

static sf::Vector2f grad(sf::Vector2f p);

Gradient::Gradient(const std::string& name,
	CanvasObject* parent)
	: QObject()
	, CanvasObject(name, parent)
{
	m_pixelPainter = new QSFML::Components::PixelPainter();
	sf::Vector2u size(1000, 1000);
	m_pixelPainter->setPixelCount(size);
	m_pixelPainter->setPixelSize(1);
	m_pixelPainter->clear();

	addComponent(m_pixelPainter);

	/*m_lines = new Lines("lines");
	addComponent(m_lines);
	m_lines->setColor(sf::Color::Blue);*/

	size_t lineCount = 1000;
	m_lines.reserve(lineCount);
	for (size_t i = 0; i < lineCount; ++i)
	{
		sf::Vector2f start = QSFML::Utilities::RandomEngine::getVector(sf::Vector2f(-5, -5), sf::Vector2f(7, 7));
		Instance instance;
		instance.pos = start;
		instance.line = new Lines("lines");
		instance.line->setColor(sf::Color::Blue);
		instance.line->addPoint(getWorldCoord(instance.pos));
		addComponent(instance.line);
		m_lines.push_back(instance);
		//setStart(start);
		//update();
	}



	sf::Vector2f offset(-(float)size.x / 200.f, -(float)size.y / 200.f);
	for (unsigned int i = 0; i < size.x; ++i)
	{
		for(unsigned int j=0; j< size.y; ++j)
		{
			sf::Vector2f pos = sf::Vector2f((float)i / 100.f, (float)j / 100.f) + offset;
			drawPixel(sf::Vector2u(i,j), f(pos));
		}
	}
}

void Gradient::setStart(const sf::Vector2f& start)
{
	m_start = start;
	m_currentPos = start;
	//m_lines->clear();
	//m_lines->addPoint(getWorldCoord(m_currentPos));
}
void Gradient::update()
{
	static int counter = 0;
	counter++;
	if(counter > 100)
	{
		counter = 0;
		for (size_t i = 0; i < m_lines.size(); ++i)
		{
			Instance& instance = m_lines[i];
			instance.pos = QSFML::Utilities::RandomEngine::getVector(sf::Vector2f(-5, -5), sf::Vector2f(7, 7));
			instance.line->clear();
			instance.line->addPoint(getWorldCoord(instance.pos));
		}
	}
	for (size_t i = 0; i < m_lines.size(); ++i)
	{
		Instance& instance = m_lines[i];
		instance.pos = decend(instance.pos, 0.01f);
		//instance.pos = newtenDecend(instance.pos);
		instance.line->addPoint(getWorldCoord(instance.pos));
	}
}
sf::Vector2f Gradient::decend(sf::Vector2f p, float amount)
{
	sf::Vector2f dir = grad(p);
	p -= dir * amount;
	return p;
}
sf::Vector2f Gradient::newtenDecend(sf::Vector2f p)
{
	sf::Vector2f dir = grad(p);
	p -= sf::Vector2f(1.f / dfxx(p) * dir.x, 1.f / dfyy(p) * dir.y);
	return p;
}

void Gradient::drawPixel(sf::Vector2u point, double value)
{
	m_pixelPainter->setPixel(point, getColor(value));
}
sf::Color Gradient::getColor(double value)
{
	// Color fade depending on value
	sf::Color color1 = sf::Color::Red;
	sf::Color color2 = sf::Color::Green;
	sf::Color color;
	value *= 0.05;
	color.r = (sf::Uint8)(color1.r + (color2.r - color1.r) * value);
	color.g = (sf::Uint8)(color1.g + (color2.g - color1.g) * value);
	color.b = (sf::Uint8)(color1.b + (color2.b - color1.b) * value);
	if(color.r < 5)
	{
		color.r = 0;
		color.g = 0;
		color.b = 0;
	}
	color.a = 255;
	return color;
}
sf::Vector2u Gradient::getPixelCoord(sf::Vector2f point)
{
	sf::Vector2u size = m_pixelPainter->getPixelCount();
	sf::Vector2u coord;
	float scale = 100;
	coord.x = (point.x * scale + size.x / 2.f);
	coord.y = (point.y * scale + size.y / 2.f);
	if(coord.x >= size.x)
	{
		coord.x = size.x-1;
	}
	if (coord.y >= size.y)
	{
		coord.y = size.y-1;
	}
	return coord;

}

sf::Vector2f Gradient::getWorldCoord(sf::Vector2f point)
{
	sf::Vector2u size = m_pixelPainter->getPixelCount();
	sf::Vector2f coord;
	float scale = 100;
	coord.x = (point.x * scale + size.x / 2.f);
	coord.y = (point.y * scale + size.y / 2.f);
	if (coord.x >= size.x)
	{
		coord.x = size.x - 1;
	}
	if (coord.y >= size.y)
	{
		coord.y = size.y - 1;
	}
	return coord;

}

COMPONENT_IMPL(Gradient::Lines);

Gradient::Lines::Lines(const std::string& name)
	: Drawable(name)
	//, m_thickness(1)
	, m_color(sf::Color::Green)
{

}
Gradient::Lines::Lines(const Lines& other)
	: Drawable(other)
	//, m_thickness(other.m_thickness)
	, m_color(other.m_color)
{
	//updateShape();
}


void Gradient::Lines::addPoint(const sf::Vector2f& pos)
{
	m_vertices.push_back(pos);
	m_vertices.back().color = m_color;
	//updateShape();
}

void Gradient::Lines::clear()
{
	m_vertices.clear();
}

/*void Gradient::Lines::setThickness(float thickness)
{
	m_thickness = thickness;
	updateShape();
}
float Gradient::Lines::getThickness() const
{
	return m_thickness;
}*/
void Gradient::Lines::setColor(const sf::Color& color)
{
	m_color = color;
	updateColor();
}
const sf::Color& Gradient::Lines::getColor() const
{
	return m_color;
}

/*void Gradient::Lines::updateShape()
{
	sf::Vector2f direction = m_end - m_begin;
	float dist = sqrt(direction.x * direction.x + direction.y * direction.y);
	if (dist == 0)
		return;
	sf::Vector2f unitDirection = direction / dist;
	sf::Vector2f unitPerpendicular(-unitDirection.y, unitDirection.x);

	sf::Vector2f offset = (m_thickness / 2.f) * unitPerpendicular;

	m_relativeVertices[0].position = m_begin + offset;
	m_relativeVertices[1].position = m_end + offset;
	m_relativeVertices[2].position = m_end - offset;
	m_relativeVertices[3].position = m_begin - offset;
}*/
void Gradient::Lines::updateColor()
{
	for (int i = 0; i < m_vertices.size(); ++i)
		m_vertices[i].color = m_color;
}

void Gradient::Lines::draw(sf::RenderTarget& target,
	sf::RenderStates) const
{
	target.draw(m_vertices.data(), m_vertices.size(), sf::LineStrip);
}





double f(sf::Vector2f p)
{
	double x1 = p.x;
	double x2 = p.y;
	double x13 = x1 * x1 * x1;
	double x14 = x1 * x13;
	double x23 = x2 * x2 * x2;
	double x24 = x2 * x23;

	return 0.25 * x14 + x13 - 17.f / 4.f * x1 * x1 - 6 * x1 + 0.2 * x24 + 6.f / 5.f * x23 + 89;
}
double dfx(sf::Vector2f p)
{
	double x1 = p.x;
	double x2 = p.y;
	double x12 = x1 * x1;
	double x13 = x1 * x12;
	double x22 = x2 * x2;
	double x23 = x2 * x22;

	return x13 + 3 * x12 - 17.f / 2.f * x1 - 6;
}
double dfy(sf::Vector2f p)
{
	double x1 = p.x;
	double x2 = p.y;
	double x12 = x1 * x1;
	double x13 = x1 * x12;
	double x22 = x2 * x2;
	double x23 = x2 * x22;

	return 4.f / 5.f * x23 + 18.f / 5.f * x22;
}

double dfxx(sf::Vector2f p)
{
	double x1 = p.x;
	double x2 = p.y;
	double x12 = x1 * x1;

	return 3 * x12 + 6 * x1 - 17.f / 2.f;
}
double dfxy(sf::Vector2f p)
{
	return 0;
}
double dfyx(sf::Vector2f p)
{
	return 0;
}
double dfyy(sf::Vector2f p)
{
	return 12.f / 5.f * p.y * p.y + 36.f / 5.f * p.y;
}

sf::Vector2f grad(sf::Vector2f p)
{
	sf::Vector2f g;
	g.x = dfx(p);
	g.y = dfy(p);
	return g;
}
