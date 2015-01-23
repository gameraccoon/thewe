#ifndef POPUP_TEXT_WITH_ICON_H
#define POPUP_TEXT_WITH_ICON_H

#include <ui/CocosGUI.h>
#include <cocos-ext.h>

#include "EffectsLayer.h"

#include "Vector2.h"
#include "MapProjector.h"

class TextWithIcon;

class PopUpTextWithIcon : public Effect
{
public:
	struct ConstructionInfo
	{
		std::string icon;
		std::string text;
		std::string font;
		Vector2 position;
		float overralScale;
		float iconScale;
		int fontSize;
	};

public:
	PopUpTextWithIcon(const ConstructionInfo &info, MapProjector *projector, bool worldSpace, int zOrder);

	bool init(void) override;
	void update(float dt) override;
	bool IsFinished(void) const;

private:
	MapProjector *_projector;
	TextWithIcon *_textWithIcon;
	ConstructionInfo _descriptor;

	bool _isWorldSpace;
	bool _isFinished;
};

#endif
