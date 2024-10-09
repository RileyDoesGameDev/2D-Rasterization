#include "PostProcess.h"


namespace PostProcess
{
	void Invert(std::vector<color_t>& buffer)
	{
		//for (auto& color : buffer)
		//{
		//	color.r = 255 - color.r;
		//	color.g = 255 - color.g;
		//	color.b = 255 - color.b;
		//}


		std::for_each(buffer.begin(), buffer.end(), [](color_t& color)
			{
				color.r = 255 - color.r;
				color.g = 255 - color.g;
				color.b = 255 - color.b;
			});

	}
}
