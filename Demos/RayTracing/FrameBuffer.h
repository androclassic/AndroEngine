#pragma once
#include <vector>
#include<Windows.h>
#include<algorithm>
#include "Camera.h"

class CFrameBuffer
{
public:
	CFrameBuffer( const int iWidth, const int iHeight );
	~CFrameBuffer();

	void Clear();
	void Render(const std::vector<andro::Hitable*>& objects);


	const unsigned int* GetFrameBuffer() const;
	int GetWidth() const { return m_iWidth; }
	int GetHeight() const { return m_iHeight; }

	int GetValue(int index) { return m_FramebufferArray[index]; }
	
private:
	andro::Vector3 get_color(const andro::ray& ray, const std::vector<andro::Hitable*>& objects);


	std::vector<unsigned int> m_FramebufferArray;
	int m_iWidth, m_iHeight;
	Camera m_camera;
};
