#include "HudWidget.h"

#include "MiscUtils.h"

HudWidget* HudWidget::create(void)
{
	HudWidget *ret = new HudWidget();
	if (ret && ret->init())
	{
		ret->autorelease();
		return ret;
	}

	delete ret;
	return nullptr;
}

HudWidget::HudWidget(void)
{
}

HudWidget::~HudWidget(void)
{
}

bool HudWidget::init(void)
{
	_widget = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("ui_hud/ui_hud.ExportJson");

	cocos2d::ui::Button *zoomin, *zoomout, *editor;
	zoomin = dynamic_cast<cocos2d::ui::Button *>(_widget->getChildByName("ZoomIn"));
	zoomout = dynamic_cast<cocos2d::ui::Button *>(_widget->getChildByName("ZoomOut"));
	editor = dynamic_cast<cocos2d::ui::Button *>(_widget->getChildByName("Editor"));

	if (!zoomin) {Log::Instance().writeError("Failed to get ui widget.");}
	if (!zoomout) {Log::Instance().writeError("Failed to get ui widget.");}
	if (!editor) {Log::Instance().writeError("Failed to get ui widget.");}

	zoomin->addTouchEventListener(CC_CALLBACK_2(HudWidget::DesctopButtonsHandler, this));
	zoomin->setTag((int)Element::PC_ZOOM_IN);
	zoomout->addTouchEventListener(CC_CALLBACK_2(HudWidget::DesctopButtonsHandler, this));
	zoomout->setTag((int)Element::PC_ZOOM_OUT);
	editor->addTouchEventListener(CC_CALLBACK_2(HudWidget::DesctopButtonsHandler, this));
	editor->setTag((int)Element::PC_EDITOR);

	if (!Utils::IsPlatformDesktop()) {
		zoomin->setVisible(false);
		zoomout->setVisible(false);
		editor->setVisible(false);
	}

	addChild(_widget);

	return true;
}

void HudWidget::AcceptMessage(const Message &message)
{
}

void HudWidget::HidePanels(void)
{
}

void HudWidget::ShowPanels(void)
{
}

void HudWidget::DesctopButtonsHandler(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType eventType)
{
	if (eventType == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		cocos2d::Node *node = dynamic_cast<cocos2d::Node *>(sender);
		if (node)
		{
			Element type = (Element)node->getTag();
			std::string name;
			switch (type) {
			case Element::PC_ZOOM_IN: name = "ScrollZoomIn"; break;
			case Element::PC_ZOOM_OUT: name = "ScrollZoomOut"; break;
			case Element::PC_EDITOR: name = "OpenEditor"; break;
			}

			if (!name.empty()) {
				MessageManager::Instance().PutMessage(Message(name));
			}
		}
		else
		{
			Log::Instance().writeWarning("Faield to cast from Ref.");
		}
	}
}
