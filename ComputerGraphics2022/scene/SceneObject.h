#pragma once


#include "./ISceneObject.h"
#include <memory>
#include "../render/Texture.h"
#include "../render/Model.h"
#include "../utils/assimpParser.h"

#define parsFlags EModelParserFlags::TRIANGULATE \
        | EModelParserFlags::GEN_SMOOTH_NORMALS   \
        | EModelParserFlags::CALC_TANGENT_SPACE    \

class SceneObject : public ISceneObject {

public:
    SceneObject(const math::vec3& pos, const math::vec3& scale,
        const spin& spin, std::shared_ptr<CRawModel> model) :
        pos(pos), scale_(scale), spin_(spin), model_(model) {}

    SceneObject() : pos(0.f), scale_(1.f), spin_(math::vec3(0.f), 0), model_(std::make_shared<CRawModel>()) {}

    virtual void draw() const override {
        
        for (auto& m : this->model_->meshes)
            glDrawElements(GL_TRIANGLES, m->GetIndexesCount(), GL_UNSIGNED_INT, 0);
        
    }

    virtual math::vec3 getPos() const override {
        return pos;
    }

    virtual void setPose(const math::vec3& pose) override {
        pos = pose;
    }

    /*isn't right to create another behavior for scale etc ?*/
    virtual void loadTexture(const std::string& path) override {
        texture = TextureLoader().load(path);
    }

    virtual void setTexture(const Texture& textur) override {
        texture = textur;
    }

    virtual Texture getTexture() const override {
        return texture;
    }

    virtual void loadModel(const std::string& path) override {
        AssimpParser().LoadModel(path, model_, parsFlags);
        
    }

    virtual void setModel(std::shared_ptr<CRawModel> model) {
        model_ = model;
    }

    virtual std::shared_ptr<CRawModel> getModel() override {
        return model_;
    }

    virtual std::string getName() const override{
        return name_;
    }

    virtual void setName(const std::string& name) override {
        name_ = name;
    }

    virtual void setScale(const math::vec3& scale) {
        scale_ = scale;
    }

    virtual math::vec3 getScale() const {
        return scale_;
    }

    virtual void rotate(const math::vec3& spin, float degree) {
        if (degree > 360)
            degree = 360;
        else if (degree < -360)
            degree = -360;
        spin_ = { spin, degree };
    }

    virtual spin getRotate() const {
        return spin_;
    }

    void bind() override
    {
        texture.bind(0);
        //this->textureNorm->bind(1);
        for (auto& m : model_->meshes)
            m->Bind();
    }

    void unbind()   override
    {
        texture.unbind();
        //textureNorm->unbind(1);
        for (auto& m : model_->meshes)
            m->Unbind();
    }

private:
    math::vec3 pos;
    math::vec3 scale_;
    spin spin_;
    std::string name_;
    std::shared_ptr<CRawModel> model_;
    Texture texture;
    Texture textureNorm;
};