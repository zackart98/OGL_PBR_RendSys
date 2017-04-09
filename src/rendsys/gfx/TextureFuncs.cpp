#include <rendsys/gfx/TextureFuncs.hpp>

#include <FreeImage/FreeImage.h>

#include <iostream>

namespace rendsys
{
	namespace detail
	{
		void InitFreeImage( )
		{
			FreeImage_Initialise(true);
		}

		// Load an OpenGL texture from a file
		GLuint LoadGLTexture2D(const std::string& texturePath, GLsizei* texWOut, GLsizei* texHOut,
							   bool genMipMaps)
		{
			(*texWOut) = -1;
			(*texHOut) = -1; 
			
			const char* filename = texturePath.c_str( );

			FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(filename, 0);

			if (fif == FIF_UNKNOWN)
			{
				fif = FreeImage_GetFIFFromFilename(filename);
			}

			if (fif == FIF_UNKNOWN || !FreeImage_FIFSupportsReading(fif))
			{
				std::cerr << "ERROR!::FreeImage::Could_Not_Open_File -- \"" << texturePath << "\""
						  << std::endl
						  << std::endl;
				return GLuint(0);
			}

			FIBITMAP* bmp   = FreeImage_Load(fif, filename);
			int		  bpp   = FreeImage_GetBPP(bmp);
			FIBITMAP* bmp32 = nullptr;

			if (bpp == 32)
			{
				bmp32 = bmp;
			}
			else
			{
				bmp32 = FreeImage_ConvertTo32Bits(bmp);
			}

			GLsizei imgW = FreeImage_GetWidth(bmp32);
			GLsizei imgH = FreeImage_GetHeight(bmp32);

			GLuint texID;
			glGenTextures(1, &texID);
			glBindTexture(GL_TEXTURE_2D, texID);
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgW, imgH, 0, GL_BGRA, GL_UNSIGNED_BYTE, FreeImage_GetBits(bmp32));
				
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				
				if (genMipMaps)
				{
					glGenerateMipmap(GL_TEXTURE_2D);
				}
			}
			glBindTexture(GL_TEXTURE_2D, 0);
			
			(*texWOut) = imgW;
			(*texHOut) = imgH;
			
			FreeImage_Unload(bmp32);
			if (bpp != 32)
			{
				FreeImage_Unload(bmp);
			}
			
			return texID;
		}
	}
}
