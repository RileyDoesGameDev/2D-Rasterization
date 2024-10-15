#include "PostProcess.h"
#include <algorithm>
#include <cstdint>
#include "MathUtils.h"

namespace PostProcess
{
	void Invert(std::vector<color_t>& buffer)
	{

		std::for_each(buffer.begin(), buffer.end(), [](color_t& color)
			{
				color.r = 255 - color.r;
				color.g = 255 - color.g;
				color.b = 255 - color.b;
			});

	}
	void Monochrome(std::vector<color_t>& buffer)
	{

		std::for_each(buffer.begin(), buffer.end(), [](color_t& color)
			{

				int gray = (color.r + color.g + color.b)/3;
				color.r = gray;
				color.g = gray;
				color.b = gray;
				
			});
	}
	void ColorBalance(std::vector<color_t>& buffer, int ro, int go, int bo)
	{


		std::for_each(buffer.begin(), buffer.end(), [ro, go , bo](color_t& color)
			{

				
				color.r = std::min(std::max(color.r + ro, 0), 255);
				color.g = std::min(std::max(color.g + go, 0), 255);
				color.b = std::min(std::max(color.b + bo, 0), 255);

			});


	}
	void Brightness(std::vector<color_t>& buffer, int brightness)
	{
		std::for_each(buffer.begin(), buffer.end(), [brightness](color_t& color)
			{

				
				color.r = std::min(255, std::max(0, color.r + brightness));
				color.g = std::min(255, std::max(0, color.g + brightness));
				color.b = std::min(255, std::max(0, color.b + brightness));
				

			});
	}
	void Noise(std::vector<color_t>& buffer, uint8_t noise)
	{

		std::for_each(buffer.begin(), buffer.end(), [noise](color_t& color)
			{

				int offset = (rand() % ((noise * 2) + 1)) - noise;
				color.r += offset;
				color.g += offset;
				color.b += offset;

			});


	}
	void Threshold(std::vector<color_t>& buffer, uint8_t threshold)
	{
		std::for_each(buffer.begin(), buffer.end(), [threshold](color_t& color)
			{
				int average = (color.r + color.g + color.b) / 3;

				if (average > threshold)
				{
					color.r = 0;
					color.g = 0;
					color.b = 0;
				}
				else if (average < threshold)
				{
					color.r = 255;
					color.g = 255;
					color.b = 255;
				}
			
			});
	}
	void Posterize(std::vector<color_t>& buffer, uint8_t levels)
	{
		uint8_t level = 255 / levels;
		std::for_each(buffer.begin(), buffer.end(), [level](color_t& color)
			{

				color.r = (color.r/ level) * level;
				color.g = (color.g/ level) * level;
				color.b = (color.b/ level) * level;

			});
	}
	void Alpha(std::vector<color_t>& buffer, uint8_t alpha)
	{
		std::for_each(buffer.begin(), buffer.end(), [alpha](color_t& color)
			{

				color.a = alpha;

			});
	}
	void BoxBlur(std::vector<color_t>& buffer, int width, int height)
	{
		std::vector<color_t> source = buffer;
		
		int k[3][3] =
		{
			{ 1, 1, 1},
			{ 1, 1, 1},
			{ 1, 1, 1}
		};
		for (int i = 0; i < buffer.size(); i++)
		{
			int x = i % width;
			int y = i / width;

			if (x < 1 || x + 1 >= width ||y < 1 || y + 1 >= height) continue;

			int r = 0;
			int g = 0;
			int b = 0;

			for (int iy = 0; iy < 3; iy++)
			{
				for (int ix = 0; ix < 3; ix++)
				{
					color_t pixel = source[(x+ ix - 1) + (y + iy - 1) * width];
					int weight = k[iy][ix];

					r += pixel.r * weight;
					g += pixel.g * weight;
					b += pixel.b * weight;

				}
			}
		color_t& color = buffer[i];
		color.r = static_cast<uint8_t>(r / 9);
		color.g = static_cast<uint8_t>(g / 9);
		color.b = static_cast<uint8_t>(b / 9);
		}

	}
	void GaussianBlur(std::vector<color_t>& buffer, int width, int height)
	{
		std::vector<color_t> source = buffer;

		int k[3][3] =
		{
			{ 1, 2, 1},
			{ 2, 4, 2},
			{ 1, 2, 1}
		};
		for (int i = 0; i < buffer.size(); i++)
		{
			int x = i % width;
			int y = i / width;

			if (x < 1 || x + 1 >= width || y < 1 || y + 1 >= height) continue;

			int r = 0;
			int g = 0;
			int b = 0;

			for (int iy = 0; iy < 3; iy++)
			{
				for (int ix = 0; ix < 3; ix++)
				{
					color_t pixel = source[(x + ix - 1) + (y + iy - 1) * width];
					int weight = k[iy][ix];

					r += pixel.r * weight;
					g += pixel.g * weight;
					b += pixel.b * weight;

				}
			}
			color_t& color = buffer[i];
			color.r = static_cast<uint8_t>(r / 16);
			color.g = static_cast<uint8_t>(g / 16);
			color.b = static_cast<uint8_t>(b / 16);
		}
	}
	void Sharpen(std::vector<color_t>& buffer, int width, int height)
	{
		std::vector<color_t> source = buffer;

		int k[3][3] =
		{
			{ 0, -1, 0},
			{ -1, 5, -1},
			{ 0, -1, 0}
		};
		for (int i = 0; i < buffer.size(); i++)
		{
			int x = i % width;
			int y = i / width;

			if (x < 1 || x + 1 >= width || y < 1 || y + 1 >= height) continue;

			int r = 0;
			int g = 0;
			int b = 0;

			for (int iy = 0; iy < 3; iy++)
			{
				for (int ix = 0; ix < 3; ix++)
				{
					color_t pixel = source[(x + ix - 1) + (y + iy - 1) * width];
					int weight = k[iy][ix];

					r += pixel.r * weight;
					g += pixel.g * weight;
					b += pixel.b * weight;

				}
			}
			color_t& color = buffer[i];
			color.r = static_cast<uint8_t>(Clamp(r, 0, 255));
			color.g = static_cast<uint8_t>(Clamp(g, 0, 255));
			color.b = static_cast<uint8_t>(Clamp(b, 0, 255));
		}
			
	}

	void EdgeDetection(std::vector<color_t>& buffer, int width, int height)
	{
		// Copy original buffer to source
		std::vector<color_t> source = buffer;

		// Sobel kernels
		int16_t kh[3][3] = {
			{ 1, 0, -1 },
			{ 2, 0, -2 },
			{ 1, 0, -1 }
		};

		int16_t kv[3][3] = {
			{ -1, -2, -1 },
			{  0,  0,  0 },
			{  1,  2,  1 }
		};

		// Define a non-zero threshold
		uint16_t threshold = 100;  // Adjust this value as needed

		// Iterate through the pixels (ignoring the boundary)
		for (int y = 1; y < height - 1; ++y) {
			for (int x = 1; x < width - 1; ++x) {
				int h = 0, v = 0;

				// Apply Sobel kernels over the neighborhood
				for (int iy = -1; iy <= 1; ++iy) {
					for (int ix = -1; ix <= 1; ++ix) {
						int sourceIndex = (x + ix) + (y + iy) * width;

						// Use average of RGB for grayscale processing
						int gray = (source[sourceIndex].r + source[sourceIndex].g + source[sourceIndex].b) / 3;

						h += gray * kh[iy + 1][ix + 1];
						v += gray * kv[iy + 1][ix + 1];
					}
				}

				// Compute gradient magnitude
				uint16_t m = (uint16_t)sqrt((h * h) + (v * v));

				// Apply threshold
				m = (m > threshold) ? m : 0;

				// Optionally scale the value
				m = (uint16_t)(m * 1.5);  // Example scaling factor

				// Clamp value between 0 and 255
				uint8_t c = (m > 255) ? 255 : m;

				// Write the edge-detected value to all color channels (grayscale output)
				int bufferIndex = x + y * width;
				buffer[bufferIndex].r = c;
				buffer[bufferIndex].g = c;
				buffer[bufferIndex].b = c;
			}
		}
	}



	void Emboss(std::vector<color_t>& buffer, int width, int height)
	{
		std::vector<color_t> source = buffer;

		// Emboss kernel
		int16_t k[3][3] = {
			{ -2, -1, 0 },
			{ -1,  1, 1 },
			{  0,  1, 2 }
		};

		// Iterate over the image pixels, excluding the border
		for (int y = 1; y < height - 1; ++y) {
			for (int x = 1; x < width - 1; ++x) {
				int r = 0, g = 0, b = 0;

				// Apply the 3x3 kernel over the current pixel's neighborhood
				for (int iy = -1; iy <= 1; ++iy) {
					for (int ix = -1; ix <= 1; ++ix) {
						int pixelIndex = (x + ix) + (y + iy) * width;
						const color_t& pixel = source[pixelIndex];

						// Get kernel weight
						int weight = k[iy + 1][ix + 1];

						// Apply the kernel to each color channel
						r += pixel.r * weight;
						g += pixel.g * weight;
						b += pixel.b * weight;
					}
				}

				// Normalize the values to be within the valid color range [0, 255]
				r = std::min(255, std::max(0, r + 128));  // Add 128 to create the raised effect
				g = std::min(255, std::max(0, g + 128));
				b = std::min(255, std::max(0, b + 128));

				// Set the new color values in the output buffer
				int bufferIndex = x + y * width;
				buffer[bufferIndex].r = static_cast<uint8_t>(r);
				buffer[bufferIndex].g = static_cast<uint8_t>(g);
				buffer[bufferIndex].b = static_cast<uint8_t>(b);
			}
		}
	}

}
