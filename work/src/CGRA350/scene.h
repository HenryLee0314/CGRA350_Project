#ifndef scene_h
#define scene_h

namespace CGRA350 {

class Scene
{
public:
	static Scene* getInstance();

	void render();

	void renderGUI();

private:
	Scene();
	virtual ~Scene();
	Scene(const Scene&);
	Scene& operator = (const Scene&);

private:
	static Scene* _instance;

};

} // namespace CGRA350

#endif // scene_h