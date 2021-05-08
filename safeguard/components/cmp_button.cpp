#include "cmp_button.h"
#include "engine.h"
#include <SFML/Window/Mouse.hpp>



using namespace std;
using namespace sf;

ButtonComponent::ButtonComponent(Entity* p, shared_ptr<ShapeComponent> s, shared_ptr<TextComponent> t)
	: _shapeCmp(s), _textCmp(t), Component(p)
{
	// Set the initial style
	setHighlight(false, true);
}

void ButtonComponent::update(double dt)
{
	// Highlight button if mouse hovers shape
	auto mousePos = Engine::GetWindow().mapPixelToCoords(Mouse::getPosition(Engine::GetWindow()));
	if (_shapeCmp->getShape().getGlobalBounds().contains(mousePos))
	{
		setHighlight(true);
	}
	else
	{
		setHighlight(false);
	}
}

void ButtonComponent::setHighlight(bool h, bool force)
{
	if (h != _isHighlited || force)
	{
		_isHighlited = h;
		if (h) // Highlithed button
		{
			if (_buttonType == "menu") {
				_shapeCmp->getShape().setFillColor(Color::Color(245, 234, 166, 180));
			}
			else {
				_shapeCmp->getShape().setFillColor(Color::Yellow);
			}			
		}
		else
		{
			// Non highlithed button
			if (_buttonType == "menu") {
				_shapeCmp->getShape().setFillColor(Color::Color(219, 201, 158, 100));
			}
			else {
				_shapeCmp->getShape().setFillColor(Color::Color(130, 130, 130, 130));
			}
			
		}
	}
}

bool ButtonComponent::isSelected()
{
	if (_isHighlited)
	{
		
		if (Mouse::isButtonPressed(Mouse::Left))
		{
			return true;
		}

	}
	return false;
}

void ButtonComponent::setButtonType(std::string type)
{
	this->_buttonType = type;
}

std::string ButtonComponent::getButtonType()
{
	return this->_buttonType;
}

bool ButtonComponent::isHighlighted()
{
	return this->_isHighlited;
}
