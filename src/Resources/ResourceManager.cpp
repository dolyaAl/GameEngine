#include "ResourceManager.h"
#include "../Renderer/ShaderProgram.h"

#include<sstream>
#include<fstream>
#include<iostream>

ResourceManager::ResourceManager(const std::string& executablePath)
{
    size_t found = executablePath.find_last_of("/\\");
    m_path = executablePath.substr(0, found);
}

std::shared_ptr<Renderer::ShaderProgram> ResourceManager::loadShaders(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath)
{
    std::string vertexString = getFileString(vertexPath);
    if (vertexString.empty())
    {
        std::cout << "No vertex shader!" << std::endl;
        return nullptr;
    }
    std::string fragmentString = getFileString(fragmentPath);
    if (fragmentString.empty())
    {
        std::cout << "No fragment shader!" << std::endl;
        return nullptr;
    }

    std::shared_ptr<Renderer::ShaderProgram>& newShader = m_ShaderPrograms.emplace(shaderName, std::make_shared<Renderer::ShaderProgram>(vertexString, fragmentString)).first->second;
    if (newShader->isCompiled())
    {
        return newShader;
    }
    
    std::cout << "Can't load shadeer program: \n" << "Vertex: " << vertexPath << "\n" << "Fragment: " << fragmentPath << std::endl;

    return nullptr;
}

std::shared_ptr<Renderer::ShaderProgram> ResourceManager::getShaderProgram(const std::string& shaderName)
{
    ShaderProgramsMap::const_iterator it = m_ShaderPrograms.find(shaderName);
    if (it != m_ShaderPrograms.end())
    {
        return it->second;
    }
    std::cout << "Can't find shader program: " << shaderName << std::endl;
    return nullptr;
}

std::string ResourceManager::getFileString(const std::string& relativeFilePath) const
{
    std::ifstream f;
    f.open(m_path + "/" + relativeFilePath.c_str(), std::ios::in | std::ios::binary);
    if (!f.is_open())
    {
        std::cout << "Failed to open file: " << relativeFilePath << std::endl;
        return std::string{};
    }
    
    std::stringstream buffer;
    buffer << f.rdbuf();
    return buffer.str();
}