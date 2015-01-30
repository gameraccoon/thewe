#include "StripEffect.h"

#include "MiscUtils.h"

StripEffect::StripEffect(void)
	: _texture(nullptr)
	, _blendFunc(cocos2d::BlendFunc::ALPHA_NON_PREMULTIPLIED)
	, _color(cocos2d::Color4F::WHITE)
	, _vbo(0)
	, _refreshVbo(true)
	, _alphaCoeff(1.0f)
{
	glGenBuffers(1, &_vbo);
}

StripEffect::~StripEffect(void)
{
	if (_vbo != 0) {
		glDeleteBuffers(1, &_vbo);
		_vbo = 0;
	}

	CC_SAFE_RELEASE(_texture);
}

StripEffect* StripEffect::create(const std::string &texture)
{
	StripEffect *ret = new StripEffect();
	if (ret && ret->initWithTexture(texture))
	{
		ret->autorelease();
		return ret;
	}

	delete ret;
	return nullptr;
}

StripEffect* StripEffect::create(const Utils::Spline<cocos2d::Vec3> &path,
		                         const std::string &textureName, int numQuads,
						         float begin, float end)
{
	StripEffect *ret = new StripEffect();
	if (ret && ret->initWithSplinePath(path, textureName, numQuads, begin, end))
	{
		ret->autorelease();
		return ret;
	}

	delete ret;
	return nullptr;
}

bool StripEffect::initWithTexture(const std::string &textureName)
{
	setGLProgramState(cocos2d::GLProgramState::getOrCreateWithGLProgramName(cocos2d::GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR));
	setTexture(textureName);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);

	glEnableVertexAttribArray(cocos2d::GLProgram::VERTEX_ATTRIB_POSITION);
    glVertexAttribPointer(cocos2d::GLProgram::VERTEX_ATTRIB_POSITION, 3,
		GL_FLOAT, GL_FALSE,
		sizeof(cocos2d::V3F_C4B_T2F),
		(GLvoid *)offsetof(cocos2d::V3F_C4B_T2F, vertices));
    
    glEnableVertexAttribArray(cocos2d::GLProgram::VERTEX_ATTRIB_COLOR);
    glVertexAttribPointer(cocos2d::GLProgram::VERTEX_ATTRIB_COLOR, 4,
		GL_UNSIGNED_BYTE, GL_TRUE,
		sizeof(cocos2d::V3F_C4B_T2F),
		(GLvoid *)offsetof(cocos2d::V3F_C4B_T2F, colors));
    
    glEnableVertexAttribArray(cocos2d::GLProgram::VERTEX_ATTRIB_TEX_COORD);
    glVertexAttribPointer(cocos2d::GLProgram::VERTEX_ATTRIB_TEX_COORD, 2,
		GL_FLOAT, GL_FALSE,
		sizeof(cocos2d::V3F_C4B_T2F),
		(GLvoid *)offsetof(cocos2d::V3F_C4B_T2F, texCoords));

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return true;
}

bool StripEffect::initWithSplinePath(const Utils::Spline<cocos2d::Vec3> &path,
		                             const std::string &textureName, int numQuads = 20,
						             float begin = 1.0f, float end = 0.0f)
{
	//Node::setPosition(cocos2d::Vec2::ZERO);
	//setAnchorPoint(cocos2d::Vec2::ZERO);
    //ignoreAnchorPointForPosition(true);

	setGLProgramState(cocos2d::GLProgramState::getOrCreateWithGLProgramName(cocos2d::GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR));
	setTexture(textureName);
	setStripGeometry(path, numQuads, begin, end);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cocos2d::V3F_C4B_T2F_Quad)*_quads.size(), &(*_quads.begin()), GL_STREAM_DRAW);

	glEnableVertexAttribArray(cocos2d::GLProgram::VERTEX_ATTRIB_POSITION);
    glVertexAttribPointer(cocos2d::GLProgram::VERTEX_ATTRIB_POSITION, 3,
		GL_FLOAT, GL_FALSE,
		sizeof(cocos2d::V3F_C4B_T2F),
		(GLvoid *)offsetof(cocos2d::V3F_C4B_T2F, vertices));
    
    glEnableVertexAttribArray(cocos2d::GLProgram::VERTEX_ATTRIB_COLOR);
    glVertexAttribPointer(cocos2d::GLProgram::VERTEX_ATTRIB_COLOR, 4,
		GL_UNSIGNED_BYTE, GL_TRUE,
		sizeof(cocos2d::V3F_C4B_T2F),
		(GLvoid *)offsetof(cocos2d::V3F_C4B_T2F, colors));
    
    glEnableVertexAttribArray(cocos2d::GLProgram::VERTEX_ATTRIB_TEX_COORD);
    glVertexAttribPointer(cocos2d::GLProgram::VERTEX_ATTRIB_TEX_COORD, 2,
		GL_FLOAT, GL_FALSE,
		sizeof(cocos2d::V3F_C4B_T2F),
		(GLvoid *)offsetof(cocos2d::V3F_C4B_T2F, texCoords));

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return true;
}

void StripEffect::clearStrip(void)
{
	_path.Clear();
	_quads.clear();
}

void StripEffect::setStripGeometry(const Utils::Spline<cocos2d::Vec3> &path, int numQuads, float begin, float end)
{
	clearStrip();
	_path = path;
	_quads.resize(numQuads);

	for (std::size_t q = 0; q < _quads.size(); ++q)
	{
		float t1 = (float)q / (float)_quads.size();
		float t2 = (float)(q+1) / (float)_quads.size();

		cocos2d::V3F_C4B_T2F_Quad &quad = _quads[q];

		// 1. position

		cocos2d::Vec3 quad_p1 = _path.GetGlobalFrame(t1);
		cocos2d::Vec3 quad_p2 = _path.GetGlobalFrame(t2);
		cocos2d::Vec3 basis = quad_p2 - quad_p1;
		cocos2d::Vec2 dir = cocos2d::Vec2(basis.x, basis.y).getNormalized();

		if (q == 0) {
			cocos2d::Vec2 dir0 = dir.rotateByAngle(cocos2d::Vec2::ZERO,  Math::PI*0.5f) * quad_p1.z*0.5f;
			cocos2d::Vec2 dir1 = dir.rotateByAngle(cocos2d::Vec2::ZERO, -Math::PI*0.5f) * quad_p1.z*0.5f;

			quad.bl.vertices.x = quad_p1.x + dir0.x;
			quad.bl.vertices.y = quad_p1.y + dir0.y;
			quad.bl.vertices.z = 0.0f;

			quad.br.vertices.x = quad_p1.x + dir1.x;
			quad.br.vertices.y = quad_p1.y + dir1.y;
			quad.br.vertices.z = 0.0f;

			quad.br.texCoords = cocos2d::Tex2F(0.0f, t1);
			quad.bl.texCoords = cocos2d::Tex2F(1.0f, t1);
			quad.tl.texCoords = cocos2d::Tex2F(0.0f, t2);
			quad.tr.texCoords = cocos2d::Tex2F(1.0f, t2);

			quad.bl.colors.r = 255 * _color.r;
			quad.bl.colors.g = 255 * _color.g;
			quad.bl.colors.b = 255 * _color.b;
			quad.bl.colors.a = 255 * _color.a * _alphaCoeff;

			quad.br.colors.r = 255 * _color.r;
			quad.br.colors.g = 255 * _color.g;
			quad.br.colors.b = 255 * _color.b;
			quad.br.colors.a = 255 * _color.a * _alphaCoeff;

		} else {
			quad.bl = _quads[q-1].tl;
			quad.br = _quads[q-1].tr;
		}

		cocos2d::Vec2 dir0 = dir.rotateByAngle(cocos2d::Vec2::ZERO,  Math::PI*0.5f) * quad_p2.z*0.5f;
		cocos2d::Vec2 dir1 = dir.rotateByAngle(cocos2d::Vec2::ZERO, -Math::PI*0.5f) * quad_p2.z*0.5f;

		quad.tl.vertices.x = quad_p2.x + dir0.x;
		quad.tl.vertices.y = quad_p2.y + dir0.y;
		quad.tl.vertices.z = 0.0f;

		quad.tr.vertices.x = quad_p2.x + dir1.x;
		quad.tr.vertices.y = quad_p2.y + dir1.y;
		quad.tr.vertices.z = 0.0f;

		// 2. tex coord

		quad.tl.texCoords = cocos2d::Tex2F(0.0f, t2);
		quad.tr.texCoords = cocos2d::Tex2F(1.0f, t2);

		// 3. color

		quad.tl.colors.r = 255 * _color.r;
		quad.tl.colors.g = 255 * _color.g;
		quad.tl.colors.b = 255 * _color.b;
		quad.tl.colors.a = 255 * _color.a * _alphaCoeff;

		quad.tr.colors.r = 255 * _color.r;
		quad.tr.colors.g = 255 * _color.g;
		quad.tr.colors.b = 255 * _color.b;
		quad.tr.colors.a = 255 * _color.a * _alphaCoeff;
	}

	_refreshVbo = true;
}

void StripEffect::DrawStrip(const cocos2d::Mat4 &transform, uint32_t flags)
{
	if (_quads.empty()) {
		return;
	}

	using namespace cocos2d;

	getGLProgram()->use();
	getGLProgram()->setUniformsForBuiltins(transform);

	GL::enableVertexAttribs(cocos2d::GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX);
	GL::blendFunc( _blendFunc.src, _blendFunc.dst );
    GL::bindTexture2D(_texture->getName());

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);

	if (_refreshVbo) {
		glBufferData(GL_ARRAY_BUFFER, sizeof(cocos2d::V3F_C4B_T2F_Quad)*_quads.size(), &(*_quads.begin()), GL_STREAM_DRAW);
		_refreshVbo = false;
	}

	GLsizei ps_offset = offsetof(cocos2d::V3F_C4B_T2F, vertices);
	GLsizei uv_offset = offsetof(cocos2d::V3F_C4B_T2F, texCoords);
	GLsizei cr_offset = offsetof(cocos2d::V3F_C4B_T2F, colors);
	GLsizei vertex_stride = sizeof(cocos2d::V3F_C4B_T2F);

	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, vertex_stride, (const GLvoid *)ps_offset);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, vertex_stride, (const GLvoid *)uv_offset);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, vertex_stride, (const GLvoid *)cr_offset);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)(_quads.size()*4));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, _quads.size()*4);
    CHECK_GL_ERROR_DEBUG();
}

void StripEffect::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags)
{
	_drawStripCommand.init(_globalZOrder);
	_drawStripCommand.func = CC_CALLBACK_0(StripEffect::DrawStrip, this, transform, flags);
	renderer->addCommand(&_drawStripCommand);
}
		
cocos2d::Texture2D* StripEffect::getTexture(void) const
{
	return _texture;
}

void StripEffect::setTexture(cocos2d::Texture2D *texture)
{
	if (_texture != texture)
	{
		CC_SAFE_RETAIN(texture);
		CC_SAFE_DELETE(_texture);
		_texture = texture;
	}
}

void StripEffect::setTexture(const std::string &name)
{
	cocos2d::Texture2D *tex = cocos2d::Director::getInstance()->getTextureCache()->addImage(name);
	setTexture(tex);
}

const cocos2d::BlendFunc& StripEffect::getBlendFunc(void) const
{
	return _blendFunc;
}

void StripEffect::setBlendFunc(const cocos2d::BlendFunc &blendFunc)
{
	_blendFunc = _blendFunc;
}

void StripEffect::setColor(const cocos2d::Color4F &color)
{
	_color = color;
}

GLubyte StripEffect::getOpacity(void) const
{
	return 255 * _alphaCoeff;
}

void StripEffect::setOpacity(GLubyte opacity)
{
	_alphaCoeff = 255.0f / (float)opacity;
}

void StripEffect::setOpacityModifyRGB(bool value)
{
	CC_UNUSED_PARAM(value);
}

bool StripEffect::isOpacityModifyRGB(void) const
{
	return false;
}
