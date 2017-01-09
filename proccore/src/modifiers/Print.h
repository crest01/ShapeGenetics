#ifndef INCLUDED_MODIFIERS_PRINT
#define INCLUDED_MODIFIERS_PRINT

#pragma once

#include "DualBuild.h"
#include "scope/Scope.h"

#include "ShapeName.h"

#include <template_utils.h>

#if BUILD_CUDA
#include <stdio.h>
#else
#include <iostream>
#endif

namespace PGG
{
	namespace Modifiers
	{
		template<class NextModifier>
		struct PrintShape
		{
			template<class TShape, class TScope>
			DUAL_BUILD_FUNCTION
			static void apply(const Scope::ScopedShape<TShape, TScope>& scopedShape, int outputId = 0)
			{
#if BUILD_CUDA
				printf("%s ",Shapes::name<TShape>());
#else
				std::cout << Shapes::name<TShape>() << " ";
#endif
				NextModifier::apply(scopedShape, outputId);
			}
		};

		template<class NextModifier>
		struct PrintPosition
		{
			template<class TShape, class TScope>
			DUAL_BUILD_FUNCTION
				static void apply(const Scope::ScopedShape<TShape, TScope>& scopedShape, int outputId = 0)
			{
#if BUILD_CUDA
					printf("pos(%f,%f,%f) ", scopedShape.scope.matrix._14, scopedShape.scope.matrix._24, scopedShape.scope.matrix._34);
#else
					std::cout << "pos(" << scopedShape.scope.matrix._14 << "," << scopedShape.scope.matrix._24 << "," << scopedShape.scope.matrix._34 << ") ";
#endif
					NextModifier::apply(scopedShape, outputId);
				}
		};

		template<class NextModifier>
		struct PrintSize
		{
			template<class TShape, class TScope>
			DUAL_BUILD_FUNCTION static
				typename enable_if<Shapes::Traits::HasSizeVector<TShape>::value == 3>::type
				apply(const Scope::ScopedShape<TShape, TScope>& scopedShape, int outputId = 0)
			{
#if BUILD_CUDA
				printf("size(%f,%f,%f) ", scopedShape.shape.size.x, scopedShape.shape.size.y, scopedShape.shape.size.z);
#else
				std::cout << "size(" << scopedShape.shape.size.x << "," <<  scopedShape.shape.size.y << "," << scopedShape.shape.size.z << ") ";
#endif
				NextModifier::apply(scopedShape, outputId);
			}

			template<class TShape, class TScope>
			DUAL_BUILD_FUNCTION static
				typename enable_if<Shapes::Traits::HasSizeVector<TShape>::value == 2>::type
				apply(const Scope::ScopedShape<TShape, TScope>& scopedShape, int outputId = 0)
			{
#if BUILD_CUDA
				printf("size(%f,%f) ", scopedShape.shape.size.x, scopedShape.shape.size.y);
#else
				std::cout << "size(" << scopedShape.shape.size.x << "," << scopedShape.shape.size.y << ") ";
#endif
				NextModifier::apply(scopedShape, outputId);
			}

			template<class TShape, class TScope>
			DUAL_BUILD_FUNCTION static
				typename enable_if<Shapes::Traits::HasSizeVector<TShape>::value == 1>::type
				apply(const Scope::ScopedShape<TShape, TScope>& scopedShape, int outputId = 0)
			{
#if BUILD_CUDA
				printf("size(%f) ", scopedShape.shape.size.xy);
#else
				std::cout << "size(" << scopedShape.shape.size << ") ";
#endif
				NextModifier::apply(scopedShape, outputId);
			}
		};

		template<char c, class NextModifier>
		struct PrintChar
		{
			template<class TShape, class TScope>
			DUAL_BUILD_FUNCTION
				static void apply(const Scope::ScopedShape<TShape, TScope>& scopedShape, int outputId = 0)
			{
#if BUILD_CUDA
					printf(c);
#else
					std::cout << c;
#endif
					NextModifier::apply(scopedShape, outputId);
				}
		};

		template<class NextModifier>
		using PrintShapeInfo = PrintShape<PrintPosition<PrintSize< PrintChar<'\n', NextModifier> > > >;
	}
}
#endif //INCLUDED_MODIFIERS_PRINT