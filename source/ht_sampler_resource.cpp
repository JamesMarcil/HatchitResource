#include <json.hpp>
#include <ht_sampler_resource.h>
#include <ht_path_singleton.h>

#include <algorithm>

namespace Hatchit
{
    namespace Resource
    {
		using namespace Core;

        Sampler::Sampler(std::string name) : FileResource<Sampler>(std::move(name)) {
		
		}

        bool Sampler::VInitFromFile(const std::string & filename)
        {
			nlohmann::json json;
			std::ifstream jsonStream(Path::Value(Path::Directory::Samplers) + filename);

			std::string filterModeString;
			std::string wrapModeString;
			std::string colorSpaceString;

			if (jsonStream.is_open())
			{
				jsonStream >> json;

				JsonExtractString(json, "FilterMode", filterModeString);
				JsonExtractString(json, "WrapMode", wrapModeString);
				JsonExtractString(json, "ColorSpace", colorSpaceString);

				jsonStream.close();
			}
			else
			{
				return false;
			}

			//Make everything upper case
			std::transform(filterModeString.begin(), filterModeString.end(), filterModeString.begin(), ::toupper);
			std::transform(wrapModeString.begin(), wrapModeString.end(), wrapModeString.begin(), ::toupper);
			std::transform(colorSpaceString.begin(), colorSpaceString.end(), colorSpaceString.begin(), ::toupper);

			if (filterModeString == "BILINEAR")
				m_filterMode = Sampler::FilterMode::BILINEAR;
			else if (filterModeString == "NEAREST")
				m_filterMode = Sampler::FilterMode::NEAREST;
			else
				m_filterMode = Sampler::FilterMode::BILINEAR;

			if (wrapModeString == "REPEAT")
				m_wrapMode = Sampler::WrapMode::REPEAT;
			else if (wrapModeString == "CLAMP")
				m_wrapMode = Sampler::WrapMode::CLAMP;
			else
				m_wrapMode = Sampler::WrapMode::REPEAT;

			if (colorSpaceString == "LINEAR")
				m_colorSpace = Sampler::ColorSpace::LINEAR;
			else if (colorSpaceString == "GAMMA")
				m_colorSpace = Sampler::ColorSpace::GAMMA;
			else
				m_colorSpace = Sampler::ColorSpace::LINEAR;

            return true;
        }

        Sampler::FilterMode Sampler::GetFilterMode() const { return m_filterMode; }
        Sampler::WrapMode Sampler::GetWrapMode() const { return m_wrapMode; }
        Sampler::ColorSpace Sampler::GetColorSpace() const { return m_colorSpace; }
    }
}


