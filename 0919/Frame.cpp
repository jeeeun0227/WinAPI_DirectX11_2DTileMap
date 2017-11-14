#include "GameSystem.h"
#include "Frame.h"
#include "Texture.h"

Frame::Frame()
	:_frameDelay(0.0f)
{

}

Frame::~Frame()
{

}

void Frame::Init(Texture *texture, int x, int y, int width, int height, float frameDelay)
{
	_sprite = GameSystem::GetInstance()->GetSprite();
	_texture = texture;

	_width = width;
	_height = height;

	// 소스 이미지에서 출력에 사용될 영역을 지정한다.
	_srcTextureRect.left = x;
	_srcTextureRect.top = y;
	_srcTextureRect.right = x + width;
	_srcTextureRect.bottom = y + height;

	_textureColor = D3DCOLOR_ARGB(255, 255, 255, 255);

	_frameDelay = frameDelay;
}

void Frame::Deinit()
{

}

void Frame::Render()
{
	//  Sprite가 출력 되기 전에 모양을 조정하고자 함
	D3DXVECTOR2 spriteCenter = D3DXVECTOR2((float)_width / 2.0f, (float)_height / 2.0f);		// 스프라이트 백터의 가운데 값
	D3DXVECTOR2 translate = D3DXVECTOR2(_x - (float)_width / 2.0f, _y - (float)_height / 2.0f / 2.0f);		// translate는 게임 툴에서 위치를 나타내는 용어로 주로 사용된다.
	D3DXVECTOR2 scaling = D3DXVECTOR2(1.0f, 1.0f);		// 크기

	// 변환 행렬 생성
	D3DXMATRIX matrix;
	D3DXMatrixTransformation2D(
		&matrix,		// 행렬
		NULL,		// 크기를 조절할 떄, 기준을 왼쪽 상단으로 유지
		0.0f,		// 회전값 ( 없음 )
		&scaling,		// 크기 조정 값
		&spriteCenter,		// 회전 중심
		0.0f,
		&translate		// 위치
	);

	_sprite->SetTransform(&matrix);
	_sprite->Draw(_texture->GetTexture(), &_srcTextureRect, NULL, NULL, _textureColor);
}

void Frame::Relese()
{

}

void Frame::Reset()
{

}

float Frame::GetFrameDelay()
{
	return _frameDelay;
}

void  Frame::SetPosition(float x, float y)
{
	_x = x;
	_y = y;
}