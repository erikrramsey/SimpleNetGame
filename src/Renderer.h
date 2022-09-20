#pragma once
namespace sng {

class Renderer {
public:
	virtual ~Renderer() = default;
	virtual void draw() = 0;
private:
};

}