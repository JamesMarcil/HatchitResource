/**
**    Hatchit Engine
**    Copyright(c) 2015-2016 ThirdDegree
**
**    GNU Lesser General Public License
**    This file may be used under the terms of the GNU Lesser
**    General Public License version 3 as published by the Free
**    Software Foundation and appearing in the file LICENSE.LGPLv3 included
**    in the packaging of this file. Please review the following information
**    to ensure the GNU Lesser General Public License requirements
**    will be met: https://www.gnu.org/licenses/lgpl.html
**
**/

#pragma once

#include <ht_resource.h>
#include <ht_bitfield.h>

namespace Hatchit
{
	namespace Resource
	{
		class HT_API RootLayout : public FileResource<RootLayout>
		{
		public:

            enum class ShaderVisibility
            {
                UNKNOWN,
                ALL,
                VERTEX,
                TESS_CONTROL,
                TESS_EVAL,
                GEOMETRY,
                FRAGMENT
            };

			enum Flags
			{
				LAYOUT_FLAG_NONE = 0,
				LAYOUT_ALLOW_INPUT_ASSEMLBER_INPUT_LAYOUT = 1 << 1,
				LAYOUT_DENY_VERTEX_SHADER_ROOT_ACCESS = 1 << 2,
				LAYOUT_DENY_TESS_CONTROL_SHADER_ROOT_ACCESS = 1 << 3,
				LAYOUT_DENY_TESS_EVAL_SHADER_ROOT_ACCESS = 1 << 4,
				LAYOUT_DENY_GEOMETRY_SHADER_ROOT_ACCESS = 1 << 5,
				LAYOUT_DENY_FRAGMENT_SHADER_ROOT_ACCESS = 1 << 6,
				LAYOUT_ALLOW_STREAM_OUTPUT = 1 << 7
			};

			struct Range
			{
				enum class Type
				{
					UKNOWN,
					CONSTANT_BUFFER,
					SHADER_RESOURCE,
					UNORDERED_ACCESS,
					SAMPLER
				};

				Type		type;
				uint32_t	numDescriptors;
				uint32_t	baseRegister;
				uint32_t	registerSpace;
			};

			struct DescriptorTable
			{
				uint32_t			rangeCount;
				std::vector<Range>	ranges;
			};

			struct Constant
			{
				uint32_t shaderRegister;
				uint32_t registerSpace;
				uint32_t valueCount;
			};

			struct Descriptor
			{
				uint32_t shaderRegister;
				uint32_t registerSpace;
			};

            struct Sampler
            {
                struct Immutable
                {
                    uint32_t            shaderRegister;
                    uint32_t            registerSpace;
                    ShaderVisibility    visibility;
                };

                enum AddressMode
                {
                    WRAP,
                    CLAMP,
                    BORDER,
                    MIRROR,
                    MIRROR_ONCE
                };

                enum CompareOperation
                {
                    COMPARE_OP_NEVER = 0,
                    COMPARE_OP_LESS,
                    COMPARE_OP_EQUAL,
                    COMPARE_OP_LESS_EQUAL,
                    COMPARE_OP_GREATER,
                    COMPARE_OP_NOT_EQUAL,
                    COMPARE_OP_GREATER_EQUAL,
                    COMPARE_OP_ALWAYS
                };

                enum FilterMode
                {
                    NEAREST,
                    BILINEAR
                };
                
                enum BorderColor
                {
                    COLOR_TRANSPARENT_BLACK,
                    COLOR_OPAQUE_BLACK,
                    COLOR_OPAQUE_WHITE
                };

                enum ColorSpace
                {
                    GAMMA,
                    LINEAR
                };

                struct Address
                {
                    AddressMode u;
                    AddressMode v;
                    AddressMode w;
                };

                struct Filter
                {
                    FilterMode min;
                    FilterMode mag;
                };

                Address             address;
                Filter              filter;
                float               mipLODBias;
                float               minLOD;
                float               maxLOD;
                uint32_t            maxAnisotropy;
                CompareOperation    compareOp;
                BorderColor         borderColor;
                Immutable           immutable;
            };

			struct Parameter
			{
				struct Data
				{
					DescriptorTable table;
					Constant		constant;
					Descriptor	    descriptor;
				};

				enum class Type
				{
					UNKNOWN,
					TABLE,
					CONSTANT,
					CONSTANT_BUFFER,
					SHADER_RESOURCE,
					UNORDERED_ACCESS
				};

				

				Type			        type;
				ShaderVisibility		visibility;
				Data			        data;
			};

			RootLayout(std::string ID, const std::string& fileName);

			virtual ~RootLayout() = default;


			uint32_t						GetParameterCount() const;
			Core::BitField<Flags>			GetDescriptorFlags() const;
			const std::vector<Parameter>&	GetParameters() const;
            const std::vector<Sampler>&     GetSamplers() const;


		private:
			uint32_t				m_parameterCount;
			std::vector<Parameter>	m_parameters;
            std::vector<Sampler>    m_samplers;
			Core::BitField<Flags>   m_flags;


			Flags					    FlagFromString(std::string s);
            Sampler::FilterMode         SamplerFilterModeFromString(std::string s);
            Sampler::AddressMode        SamplerAddressModeFromString(std::string s);
            Sampler::ColorSpace         SamplerColorSpaceFromString(std::string s);
            Sampler::CompareOperation   SamplerCompareOpFromString(std::string s);
            Sampler::BorderColor        SamplerBorderColorFromString(std::string s);
			ShaderVisibility	        ParameterVisibilityFromString(std::string s);
			Parameter::Type			    ParameterTypeFromString(std::string s);
			Range::Type				    RangeTypeFromString(std::string s);
		};

		using RootLayoutHandle = Core::Handle<const RootLayout>;
	}
}