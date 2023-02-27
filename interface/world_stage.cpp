#include "world_stage.hpp"
#include "../infrastructure/logger.hpp"
#include "../world/debug_box.hpp"

WorldStage::WorldStage() : viewport_(Size(-1, -1)), camera_(nullptr), ground_(nullptr), shader_program_(nullptr),
                           collision_delegate_(nullptr)
{
#if DISPLAY_COLLIDERS
    collider_display_ = new DebugBox("colliderDisplay", PositionedBox(0, 0, 0, 0, 0, 0));
    collider_display_->setStage(this);
    collider_display_->load();
#endif
}

void WorldStage::viewportSizeChanged(Size size)
{
    viewport_ = size;
    glViewport(0, 0, viewport_.width(), viewport_.height());
}

void WorldStage::render(unsigned int time)
{
    applyPhysics(time);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader_program_->use();

    if(camera_ == nullptr)
    {
        log() - Critical < "No camera set";
        return;
    }

    camera_->setStageViewport(shader_program_);

    if(ground_ == nullptr)
    {
        log() - Critical < "Ground not set";
    }
    else ground_->render();

    for (Object *object : objects_)
    {
        object->render();
    }

#if DISPLAY_COLLIDERS
    for(PositionedBox box : collider_boxes_)
    {
        collider_display_->setBox(box);
        collider_display_->render();
    }
#endif
}

void WorldStage::addObject(Object *object)
{
    objects_.push_back(object);
    object->setStage(this);
}

void WorldStage::removeObject(Object *object)
{
    for (long unsigned int i = 0; i < objects_.size(); ++i)
    {
        if(objects_[i] == object)
        {
            objects_.erase(objects_.begin() + i);
            objects_[i]->setStage(nullptr);
            delete objects_[i];
        }
    }
}

void WorldStage::onAppearing()
{
    shader_program_ = new ShaderProgram(":/shaders/vertex.vert", ":/shaders/fragment.frag");
    for (Object *object : objects_)
    {
        object->load();
    }
}


void WorldStage::onDisappearing()
{
    for(Object *object : objects_)
    {
        object->unload();
    }
}

void WorldStage::setCamera(Camera *camera)
{
    camera_ = camera;
    camera_->setViewport(&viewport_);
}

ShaderProgram *WorldStage::shaderProgram()
{
    return shader_program_;
}

void WorldStage::handleEvent(Event *event)
{
    if(event->type() == KeyPressEventType || event->type() == KeyReleaseEventType || event->type() == MouseMoveEventType)
    {
        if(input_delegate_ == nullptr) log() - Debug < "No input delegate set, ignoring input event";
        else input_delegate_(this, event);
    }
}

void WorldStage::setGround(Ground *ground)
{
    ground_ = ground;
    ground_->setShaderProgram(&shader_program_);
}

void WorldStage::applyPhysics(unsigned int time)
{
#if DISPLAY_COLLIDERS
    collider_boxes_.clear();
#endif
    for(const auto &obj : objects_)
    {
        std::optional<Rotation> rot = obj->motionVector().grabbedRotation();
        Rotation physics_rot;
        if(rot.has_value()) physics_rot = obj->rotation() + rot.value();
        else physics_rot = obj->rotation();

        Vec3 velocity = obj->motionVector().velocity();
        Vec3 distance = (velocity * ((float)time / 100));
        Mat4 rotation_mat = Mat4(1);
        rotation_mat = glm::rotate(rotation_mat, glm::radians(physics_rot.roll()), Vec3(0, 1, 0));
        distance = Vec3(rotation_mat * Vec4(distance, 1));
        PositionedBox aabb = interface::axisAlignedBoundingBox(obj->position() + distance, physics_rot, obj->boundingBox());

#if DISPLAY_COLLIDERS
        collider_boxes_.push_back(aabb);
#endif

        for(const auto &collider : objects_)
        {
            if(collider == obj) continue;
            std::optional<Rotation> collider_rot = collider->motionVector().grabbedRotation();
            Rotation collider_physics_rot;
            if(collider_rot.has_value()) collider_physics_rot = collider->rotation() + collider_rot.value();
            else collider_physics_rot = collider->rotation();
            PositionedBox collider_aabb = interface::axisAlignedBoundingBox(collider->position(), collider_physics_rot,
                                                                            collider->boundingBox());

            if(interface::checkCollision(aabb.x(), aabb.x() + aabb.width(), collider_aabb.x(), collider_aabb.x() + collider_aabb.width())
               && interface::checkCollision(aabb.y(), aabb.y() + aabb.height(), collider_aabb.y(), collider_aabb.y() + collider_aabb.height())
               && interface::checkCollision(aabb.z(), aabb.z() + aabb.depth(), collider_aabb.z(), collider_aabb.z() + collider_aabb.depth()))
            {
                if(collision_delegate_(this, obj, collider)) return;
            }
        }

        obj->setPosition(obj->position() + distance);
    }

    if(camera()->motionVector().has_value())
    {
        std::optional<Rotation> rot = camera()->motionVector()->grabbedRotation();
        Rotation physics_rot;
        if (rot.has_value()) physics_rot = camera()->rotation() + rot.value();
        else
        {
            physics_rot = camera()->rotation();
            std::optional<Rotation> mvec_rot = camera()->motionVector()->rotation();
            if(mvec_rot.has_value())
                physics_rot = physics_rot + (mvec_rot.value() * ((float)time / 100));
        }

        Vec3 velocity = camera()->motionVector()->velocity();
        Vec3 distance = (velocity * ((float) time / 100));
        Mat4 rotation_mat = Mat4(1);
        rotation_mat = glm::rotate(rotation_mat, glm::radians(physics_rot.roll()), Vec3(0, 1, 0));
        distance = Vec3(rotation_mat * Vec4(distance, 1));
        camera()->setPosition(camera()->position() + distance);
        camera()->setRotation(physics_rot);
    }
}

WorldStage::~WorldStage()
{
#if DISPLAY_COLLIDERS
    collider_display_->unload();
    delete collider_display_;
#endif
    for (Object *object : objects_)
    {
        delete object;
    }
    delete camera_;
    delete shader_program_;
}

void WorldStage::setCollisionDelegate(bool (*collision_delegate)(WorldStage *stage, Object *, Object *))
{
    collision_delegate_ = collision_delegate;
}

void WorldStage::setInputDelegate(void (*input_delegate)(WorldStage *, Event *))
{
    input_delegate_ = input_delegate;
}

Camera *WorldStage::camera()
{
    return camera_;
}

Object *WorldStage::findObjectByName(const std::string &name) const
{
    auto iter = std::find_if(objects_.begin(), objects_.end(), [name](Object *obj) { return obj->name() == name; });
    if(iter == std::end(objects_)) return nullptr;
    return *iter;
}
