#ifndef SCENE_H
#define SCENE_H

class Scene
{
public:
	Scene() {}
	~Scene() {}

	virtual void Init() = 0;
	virtual void Update(double dt,double mx, double my) = 0;
	virtual void Render() = 0;
	virtual void Exit() = 0;
};

#endif