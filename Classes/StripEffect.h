#ifndef STRIP_EFFECT_H
#define STRIP_EFFECT_H

#include <cocos2d.h>

#include "Spline.h"

class StripEffect : public cocos2d::Node, public cocos2d::TextureProtocol
{
public:
	static StripEffect* create(const std::string &texture);
	static StripEffect* create(const Utils::Spline<cocos2d::Vec3> &path,
		                       const std::string &textureName,
							   int numQuads = 20,
						       float begin = 1.0f,
						       float end = 0.0f);
	void clearStrip(void);
	void setStripGeometry(const Utils::Spline<cocos2d::Vec3> &path,
					      int numQuads = 20,
						  float begin = 1.0f,
						  float end = 0.0f);

	virtual void draw(cocos2d::Renderer *renderer,
		              const cocos2d::Mat4 &transform,
					  uint32_t flags) override;
		
	virtual cocos2d::Texture2D* getTexture(void) const override;
	virtual void setTexture(cocos2d::Texture2D *texture) override;
	virtual void setTexture(const std::string &name);

	virtual const cocos2d::BlendFunc& getBlendFunc(void) const override;
	virtual void setBlendFunc(const cocos2d::BlendFunc &blendFunc) override;
	virtual void setColor(const cocos2d::Color4F &color);

	virtual GLubyte getOpacity(void) const override;
	virtual void setOpacity(GLubyte opacity) override;
	virtual void setOpacityModifyRGB(bool value) override;
	virtual bool isOpacityModifyRGB(void) const override;

protected:
	StripEffect(void);
	virtual ~StripEffect(void);

	bool initWithTexture(const std::string &textureName);
	bool initWithSplinePath(const Utils::Spline<cocos2d::Vec3> &path,
		                    const std::string &textureName,
							int numQuads,
						    float begin,
						    float end);
protected:
	void DrawStrip(const cocos2d::Mat4 &transform, uint32_t flags);

	cocos2d::Texture2D *_texture;
	cocos2d::BlendFunc _blendFunc;
	cocos2d::CustomCommand _drawStripCommand;
	cocos2d::Color4F _color;

	std::vector<cocos2d::V3F_C4B_T2F_Quad> _quads;
	Utils::Spline<cocos2d::Vec3> _path;

	float _alphaCoeff;
	bool _refreshVbo;
	GLuint _vbo;
	
private:
	CC_DISALLOW_COPY_AND_ASSIGN(StripEffect);
};

#endif