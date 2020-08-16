#ifndef BASIC_H
#define BASIC_H

namespace CGRA350 {

class Basic
{
public:
	static Basic* getInstance();

private:
	Basic();
	virtual ~Basic();
	Basic(const Basic&);
	Basic& operator = (const Basic&);

private:
	static Basic* _instance;
};

} // namespace CGRA350

#endif // BASIC_H