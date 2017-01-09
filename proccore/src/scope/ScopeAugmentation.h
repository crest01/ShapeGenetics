#ifndef INCLUDED_SHAPES_AUGMENTATION
#define INCLUDED_SHAPES_AUGMENTATION

#pragma once

#include "DualBuild.h"

namespace PGG
{
	namespace Scope
	{


		template<int Sublayer, typename... Layers>
		struct TypeQuery;

		template<typename Layer, typename... RemainingLayers>
		struct TypeQuery<0, Layer, RemainingLayers...>
		{
			typedef Layer Type;
		};

		template<int Sublayer, typename Layer, typename... RemainingLayers>
		struct TypeQuery<Sublayer, Layer, RemainingLayers...>
		{
			typedef typename TypeQuery<Sublayer - 1, RemainingLayers...>::Type Type;
		};

		template<int Sublayer, class TAugmentation, class Layer, typename... RemainingLayers>
		struct AugmentationAccess
		{
			DUAL_BUILD_FUNCTION
			static typename TypeQuery<Sublayer - 1, RemainingLayers...>::Type const& access(const TAugmentation& augmentation)
			{
				return augmentation.remainingLayers.template access<Sublayer - 1>();
			}

			DUAL_BUILD_FUNCTION
			static typename TypeQuery<Sublayer - 1, RemainingLayers...>::Type& access(TAugmentation& augmentation)
			{
				return augmentation.remainingLayers.template access<Sublayer - 1>();
			}
		};

		template<class TAugmentation, class Layer, typename... RemainingLayers>
		struct AugmentationAccess<0, TAugmentation, Layer, RemainingLayers...>
		{
			DUAL_BUILD_FUNCTION
			static const Layer& access(const TAugmentation& augmentation)
			{
				return augmentation.val;
			}

			DUAL_BUILD_FUNCTION
			static Layer& access(TAugmentation& augmentation)
			{
				return augmentation.val;
			}
		};

		template<typename... Layers>
		struct Augmentation;

		template<typename Layer, typename... RemainingLayers>
		struct Augmentation<Layer, RemainingLayers...>
		{
			Layer val;
			typedef Augmentation<RemainingLayers...> Sublayers;
			Sublayers remainingLayers;

			DUAL_BUILD_FUNCTION Augmentation() = default;
			DUAL_BUILD_FUNCTION Augmentation(const Layer& val, const RemainingLayers & ... remaining) : val(val), remainingLayers(remaining...) { }

			template<int Sublayer>
			DUAL_BUILD_FUNCTION
			typename TypeQuery<Sublayer, Layer, RemainingLayers...>::Type& access()
			{
				return AugmentationAccess<Sublayer, Augmentation, Layer, RemainingLayers...>::access(*this);
			}

			template<int Sublayer>
			DUAL_BUILD_FUNCTION
			const typename TypeQuery<Sublayer, Layer, RemainingLayers...>::Type& access() const
			{
				return AugmentationAccess<Sublayer, Augmentation, Layer, RemainingLayers...>::access(*this);
			}
		};



		template<typename Layer>
		struct Augmentation<Layer>
		{
			Layer val;

			DUAL_BUILD_FUNCTION Augmentation() = default;
			DUAL_BUILD_FUNCTION Augmentation(const Layer& val) : val(val) { }

			template<int Sublayer>
			DUAL_BUILD_FUNCTION
			typename TypeQuery<Sublayer, Layer>::Type const& access() const
			{
				return AugmentationAccess<Sublayer, Augmentation, Layer>::access(*this);
			}

			template<int Sublayer>
			DUAL_BUILD_FUNCTION
			typename TypeQuery<Sublayer, Layer>::Type& access()
			{
				return AugmentationAccess<Sublayer, Augmentation, Layer>::access(*this);
			}
		};



	}
}

#endif //INCLUDED_SHAPES_AUGMENTATION
