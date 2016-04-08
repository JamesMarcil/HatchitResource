/**
**    Hatchit Engine
**    Copyright(c) 2015 Third-Degree
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

#include <ht_material_resource.h>

namespace Hatchit
{
    namespace Resource
    {
        using namespace Core;

        Material::Material(std::string ID, const std::string& fileName) : FileResource<Material>(std::move(ID))
        {
            nlohmann::json json;
            std::ifstream jsonStream(Path::Value(Path::Directory::Materials) + fileName);

            if (jsonStream.is_open())
            {
                jsonStream >> json;

                JsonExtractString(json, "Pipeline", m_pipelinePath);
                JsonExtractString(json, "RenderPass", m_renderPassPath);

                // Extract ShaderVariables
                nlohmann::json shaderVariables = json["ShaderVariables"];
                std::string name;
                std::string type;

                for (unsigned i = 0; i < shaderVariables.size(); i++)
                {
                    JsonExtractString(shaderVariables[i], "Name", name);
                    JsonExtractString(shaderVariables[i], "Type", type);

                    if (type == "INT" || type == "Int")
                    {
                        uint32_t value;
                        JsonExtractUint32(shaderVariables[i], "Value", value);
                        m_shaderVariables[name] = new IntVariable(value);
                    }
                    else if (type == "FLOAT" || type == "Float")
                    {
                        float value;
                        JsonExtractFloat(shaderVariables[i], "Value", value);
                        m_shaderVariables[name] = new FloatVariable(value);
                    }
                    else if (type == "DOUBLE" || type == "Double")
                    {
                        double value;
                        JsonExtractDouble(shaderVariables[i], "Value", value);
                        m_shaderVariables[name] = new DoubleVariable(value);
                    }
                    else if (type == "FLOAT2" || type == "Float2")
                    {
                        nlohmann::json jsonVec = shaderVariables[i]["Value"];
                        Math::Vector2 vec = Math::Vector2(jsonVec[0], jsonVec[1]);
                        m_shaderVariables[name] = new Float2Variable(vec);
                    }
                    else if (type == "FLOAT3" || type == "Float3")
                    {
                        nlohmann::json jsonVec = shaderVariables[i]["Value"];
                        Math::Vector3 vec = Math::Vector3(jsonVec[0], jsonVec[1], jsonVec[2]);
                        m_shaderVariables[name] = new Float3Variable(vec);
                    }
                    else if (type == "FLOAT4" || type == "Float4")
                    {
                        nlohmann::json jsonVec = shaderVariables[i]["Value"];
                        Math::Vector4 vec = Math::Vector4(jsonVec[0], jsonVec[1], jsonVec[2], jsonVec[3]);
                        m_shaderVariables[name] = new Float4Variable(vec);
                    }
                    else if (type == "MATRIX4" || type == "Matrix4")
                    {
                        nlohmann::json jsonMat = shaderVariables[i]["Value"];
                        Math::Matrix4 mat = Math::Matrix4(jsonMat[0], jsonMat[1], jsonMat[2], jsonMat[3],
                            jsonMat[4], jsonMat[5], jsonMat[6], jsonMat[7],
                            jsonMat[8], jsonMat[9], jsonMat[10], jsonMat[11],
                            jsonMat[12], jsonMat[13], jsonMat[14], jsonMat[15]);
                        m_shaderVariables[name] = new Matrix4Variable(mat);
                    }
                }

                //Extract texture paths
                nlohmann::json textures = json["Textures"];
                for (size_t i = 0; i < textures.size(); i++)
                    m_texturePaths.push_back(textures[i]);

                jsonStream.close();
            }
            else
            {
                DebugPrintF("ERROR: Could not generate stream to JSON file -> %s", Path::Value(Path::Directory::Materials) + fileName);
            }
        }

        const std::string& Material::GetPipelinePath() const { return m_pipelinePath; }
        const std::string& Material::GetRenderPassPath() const { return m_renderPassPath; }
        std::map<std::string, ShaderVariable*> Material::GetShaderVariables() const { return m_shaderVariables; }
        std::vector<std::string> Material::GetTexturePaths() const { return m_texturePaths; }
    }
}