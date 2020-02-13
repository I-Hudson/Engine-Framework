#pragma once

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
			virtual void AddAttachment() = 0;

			static GBuffer* Create();
		};
	}
}