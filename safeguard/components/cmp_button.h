#pragma once

#include <ecm.h>
#include "cmp_text.h"
#include "cmp_sprite.h"

class ButtonComponent : public Component
{
private:
	bool _isHighlited = false;
	std::shared_ptr<ShapeComponent> _shapeCmp;
	std::shared_ptr<TextComponent> _textCmp;
	std::string _buttonType;

public:
	ButtonComponent() = delete;
	explicit ButtonComponent(Entity* p, std::shared_ptr<ShapeComponent>, std::shared_ptr<TextComponent>);
	void update(double dt) override;
	void render() override {}
	void setHighlight(bool, bool force = false);
	bool isSelected();
	void setButtonType(std::string type);
	std::string getButtonType();
	bool isHighlighted();
};