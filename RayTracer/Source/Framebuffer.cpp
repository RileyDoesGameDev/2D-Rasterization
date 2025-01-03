#include "Framebuffer.h"
#include"Renderer.h"
#include "iostream"
#include "MathUtils.h"
#include "Image.h"
#include "Color.h"
Framebuffer::Framebuffer(const Renderer& renderer, int width, int height)
{
	m_width = width;
	m_height = height;
	m_pitch = width * sizeof(color_t);

	m_texture = SDL_CreateTexture(renderer.m_renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, width, height);
	if (m_texture == nullptr)
	{
		std::cerr << "Error initalizing SDL: " << SDL_GetError() << std::endl;
	}
	m_buffer.resize(m_width * m_height);;
	
}

Framebuffer::~Framebuffer()
{
	SDL_DestroyTexture(m_texture);
}

void Framebuffer::Update()
{
	SDL_UpdateTexture(m_texture, NULL, m_buffer.data(), m_pitch);
}

void Framebuffer::Clear(const color_t& color)
{
	std::fill(m_buffer.begin(), m_buffer.end(), color);

}

void Framebuffer::DrawRect(int x, int y, int w, int h, const color_t& color)
{
	if (x + w < 0 || x >= m_width || y + h < 0 || y >= m_height) return;

	int x1 = std::max(x, 0);
	int x2 = std::min(x + w, m_width);
	int y1 = std::max(y, 0);
	int y2 = std::min(y + h, m_height);

	for (int sy = y1; sy < y2; sy++) {
		int index = x1 + sy * m_width;
		std::fill(m_buffer.begin() + index, m_buffer.begin() + (index + x2 - x1), color);

	}
}

void Framebuffer::DrawPoint(int x, int y, const color_t& color)
{
	color_t& dest = m_buffer[x + y * m_width];
	dest = ColorBlend(color, dest);

}

void Framebuffer::DrawPointClip(int x, int y, const color_t& color)
{
	if (x >= m_width || x < 0 || y < 0 || y >= m_height) return;

	color_t& dest = m_buffer[x + y * m_width];
	dest = ColorBlend(color, dest);
}

void Framebuffer::DrawLine(int x1, int y1, int x2, int y2, const color_t& color)
{
	int dx = x2 - x1; //run
	int dy = y2 - y1; //rise

	// y = mx + b
	// m = rise / run

	if (dx == 0) {
		if (y1 > y2) std::swap(y1, y2);
		for (int y = y1; y < y2; y++) {
			m_buffer[x1 + y * m_width] = color;
		}
	}
	else {
		float m = dy / (float)dx;
		// b = y - (m*x) (y intercept)
		float b = y1 - (m * x1);

		// draw line points
		if (std::abs(dx) > std::abs(dy)) {
			for (int x = x1; x <= x2; x++) {

				int y = (int)round((m * x) + b);
				m_buffer[x + y * m_width] = color;
			}
		}
		else {
			for (int y = y1; y <= y2; y++) {

				int x = (int)round((y - b) / m);
				m_buffer[x + y * m_width] = color;
			}
		}
	}
}



void Framebuffer::DrawLineSlope(int x1, int y1, int x2, int y2, const color_t& color)
{
	int dx = x2 - x1;
	int dy = y2 - y1;

	if (dx == 0) // Vertical line case
	{
		if (y1 > y2) std::swap(y1, y2); // Ensure we iterate upwards
		for (int y = y1; y <= y2; y++) {
			m_buffer[x1 + y * m_width] = color; // Vertical line (constant x)
		}
	}
	else // Non-vertical line
	{
		float m = dy / (float)dx; // Slope
		float b = y1 - (m * x1);  // Y-intercept

		if (std::abs(dx) > std::abs(dy)) // Shallow slope
		{
			if (x1 > x2) { // Ensure left-to-right drawing
				std::swap(x1, x2);
				std::swap(y1, y2);
			}
			for (int x = x1; x <= x2; x++) {
				int y = (int)round((m * x) + b);
				m_buffer[x + y * m_width] = color;
			}
		}
		else // Steep slope
		{
			if (y1 > y2) { // Ensure bottom-to-top drawing
				std::swap(x1, x2);
				std::swap(y1, y2);
			}
			for (int y = y1; y <= y2; y++) {
				int x = (int)round((y - b) / m);
				m_buffer[x + y * m_width] = color;
			}
		}
	}
}

void Framebuffer::DrawCircle(int xc, int yc, int x, int y, const color_t& color)
{
	DrawPoint(xc + x, xc + y, color);
	DrawPoint(xc - x, xc + y, color);
	DrawPoint(xc + x, xc - y, color);
	DrawPoint(xc - x, xc - y, color);

	DrawPoint(xc + y, xc + x, color);
	DrawPoint(xc - y, xc + x, color);
	DrawPoint(xc + y, xc - x, color);
	DrawPoint(xc - y, xc - x, color);
}

void Framebuffer::CircleBres(int xc, int yc, int r, const color_t& color)
{
	int x = 0, y = r;
	int d = 3 - 2 * r;
	DrawCircle(xc, yc, x, y, color);
	while (y >= x) {

		// check for decision parameter
		// and correspondingly 
		// update d, y
		if (d > 0) {
			y--;
			d = d + 4 * (x - y) + 10;
		}
		else
			d = d + 4 * x + 6;

		// Increment x after updating decision parameter
		x++;

		// Draw the circle using the new coordinates
		DrawCircle(xc, yc, x, y, color);

	}
}


void Framebuffer::DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, const color_t& color)
{
	DrawLineSlope(x1, y1, x2, y2, color);
	DrawLineSlope(x2, y2, x3, y3, color);
	DrawLineSlope(x3, y3, x1, y1, color);
}

void Framebuffer::DrawLinearCurve(int x1, int y1, int x2, int y2, const color_t& color)
{

	float dt = 1/ 10.0f;
	float t1 = 0;

	for (int i = 0; i < 10; i++)
	{

	int sx1 = Lerp(x1, x2, t1);
	int sy1 = Lerp(y1, y2, t1);
	
	float t2 = t1 + dt;

	int sx2 = Lerp(x1, x2, t2);
	int sy2 = Lerp(y1, y2, t2);
	t1 += dt;
	DrawLineSlope(sx1, sy1, sx2, sy2, color);
	}


}

void Framebuffer::DrawQuadraticCurve(int x1, int y1, int x2, int y2, int x3, int y3, const color_t& color)
{
	float dt = 1 / 10.0f;
	float t1 = 0;

	for (int i = 0; i < 10; i++)
	{
		int sx1, sy1;
		QuadraticPoint(x1, y1, x2, y2, x3, y3, t1, sx1, sy1);
		float t2 = t1 + dt;
		QuadraticPoint(x1, y1, x2, y2, x3, y3, t1, sx1, sy1);
		int sx2 = Lerp(x1, x2, t2);
		int sy2 = Lerp(y1, y2, t2);
		t1 += dt;
		DrawLine(sx1, sy1, sx2, sy2, color);
	}
}

void Framebuffer::DrawCubicCurve(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, const color_t& color)
{
	////float one_minus_t1 = 1 - t;
	//float a1 = one_minus_t1 * one_minus_t1;
	//float b1 = 2 * one_minus_t1 * t;
	//float c1 = t * t;

}


	void Framebuffer::DrawImage(int x, int y, const Image& image)
	{
		// check if off-screen
		
	
		if (x + image.m_width < 0 || x >= m_width || y + image.m_height < 0 || y >= m_height) return;

		for (int iy = 0; iy < image.m_height; iy++)
		{
			int sy = y + iy;
			if (sy < 0 || sy >= m_height) continue;
			for (int ix = 0; ix < image.m_width; ix++)
			{
		
				int sx = x + ix;
			
				if (sx < 0 || sx >= m_width) continue;

		
				color_t color = image.m_buffer[iy * image.m_width + ix];
			
				if (color.a == 0) continue;
			;
			DrawPointClip(sx, sy, color);
				//m_buffer[sy * m_width + sx] = color;
			
		}
	}

}
