#pragma once

#include <string>

namespace Framework
{
	namespace Renderer
	{
		class GBuffer
		{
		public:
			virtual void Init() = 0;
			virtual void Free() = 0;

			virtual void Bind() = 0;
			virtual void Unbind() = 0;
			virtual void AddAttachment(int aLevel, int aInternalFormat, 
										int aWidth, int aHeight,
										int aBorder, int aFormat,
										int aType, int aAttachment,
										std::string aName) = 0;

			const unsigned int& GetID() { return m_ID; }
			static GBuffer* Create();

		protected:
			unsigned int m_ID;
		};
	}
}