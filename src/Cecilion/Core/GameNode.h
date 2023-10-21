#pragma once
#include <ECS/Config.h>
#include <ECS/Entity.h>
#include <ECS/Component.h>
namespace Cecilion {
    class GameNode: I_Component {
    public:
        GameNode(const GameNode&) = delete;
        GameNode& operator=(const GameNode&) = delete;
        GameNode() = delete;


        bool operator==(const GameNode &rhs) const;
        bool operator!=(const GameNode &rhs) const;
        bool operator==(std::nullptr_t _);
        bool operator!=(std::nullptr_t _);

        GameNode& Instantiate() {

        }

        /**
         * Add a script to this game node.
         * @tparam Component
         * @return
         */
        /*
       template<typename Component, typename std::enable_if<std::is_base_of<BehaviourScript, Component>::value>>
       Component& add_component();
       */
    protected:

        explicit GameNode(const Cecilion::Entity_ref& entity, const GameNode* parent);

        const Cecilion::Entity_ref& entity() {
            return this->m_entity;

        }

    public:
        void setParent(GameNode *mParent);

    private:
        virtual void start(){}
        virtual void active(){}
        virtual void inactive(){}
        virtual void update(){}
        virtual void destroy(){}

        void addChild(GameNode& node);

        const GameNode* p_parent = nullptr;
    };
}