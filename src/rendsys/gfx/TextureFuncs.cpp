#include <rendsys/gfx/TextureFuncs.hpp>

#include <FreeImage/FreeImage.h>

namespace rendsys
{
	namespace detail
	{
		void InitFreeImage( )
		{
			FreeImage_Initialise(true);
		}
	}
}
