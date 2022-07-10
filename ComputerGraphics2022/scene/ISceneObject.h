#pragma once
#include "../render/common.h"
#include "../render/Texture.h"
#include "../render/Model.h"
#include <string>


struct spin
{
    math::vec3 spin_;
    float deg_;

    spin() = default;
    spin(const math::vec3& v, float d) :
        spin_(v), deg_(d) {}
};

struct ISceneObject {
    virtual void draw() const = 0;
    virtual math::vec3 getPos() const = 0;
    virtual void setPose(const math::vec3& pose) = 0;
    /*isn't right to create another behavior for scale etc ?*/
    virtual void loadTexture(const std::string& path) = 0;
    virtual void setTexture(const Texture& texture) = 0;
    virtual Texture getTexture() const = 0;
    virtual void loadModel(const std::string& path) = 0;
    virtual void setModel(std::shared_ptr<CRawModel>) = 0;
    virtual std::shared_ptr<CRawModel> getModel() = 0;

    virtual std::string getName() const = 0;
    virtual void setName(const std::string& name) = 0;
    virtual void setScale(const math::vec3& scale) = 0;
    virtual math::vec3 getScale() const = 0;
    virtual void rotate(const math::vec3& spin, float degree) = 0;
    virtual spin getRotate() const = 0;
    virtual void bind() = 0;
    virtual void unbind() = 0;

};