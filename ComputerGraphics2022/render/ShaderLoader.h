#pragma once

#include "./Shader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

struct IDebugRWProtocol {
    virtual void write(const std::string& msg) = 0;
};

struct ConsoleRW : public IDebugRWProtocol {
    void write(const std::string& msg) override { std::cout << msg; }
};

class ShaderLoader {

public:
    ShaderLoader(IDebugRWProtocol* pRWProtocol = new ConsoleRW()) : rvProtocol_(pRWProtocol) {};
    static ShaderLoader& singleton(); 
    Shader load(const std::string& path);
private:
    IDebugRWProtocol* rvProtocol_ = nullptr;

public:
    ShaderLoader(const ShaderLoader&&) = delete;
    ShaderLoader(ShaderLoader&&) = delete;
    void operator=(const ShaderLoader&) = delete;
};