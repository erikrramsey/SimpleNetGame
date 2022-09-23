#pragma once
namespace sng {

class Renderer {
public:
	virtual ~Renderer() = default;
	virtual void begin() = 0;
	virtual void end() = 0;
	virtual void drawQuad(float x, float y) = 0;
private:
};

}