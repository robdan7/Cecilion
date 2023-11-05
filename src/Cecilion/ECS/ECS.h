#pragma once

#include <memory>
#include <utility>
#include <typeinfo>
#include <Utils/Sparse_set.h>
#include <cstdint>
#include <typeindex>
#include <unordered_map>
#include <algorithm>
#include "Component_storage.h"
#include "Filter.h"
#include "Config.h"
#include <Utils/allocator/Sparse_table.h>
#include <Event/System_events.h>
#include "Component_iterator.h"
#include <Utils/Uuid.h>
#include <yaml-cpp/yaml.h>
#include <Utils/Serializable.h>
//#include "Entity.h"
//#include "Component.h"
namespace Cecilion {
    class I_Component;

    class I_Component_ref;

    class ECS;

    struct Entity_ref;

    class Entity_source;

    class I_Component;


    template<class C>
    class Component_ref;

    /**
     * This is the global entity component system. A lot of inspiration has been taken from
     * ENTT on GitHub and other various sources on the web. It actually works though.
     */
    class ECS {
        friend Entity_ref;
    private:

        typedef Cecilion::I_Component_ref (*DeserializeComponentFunc)(ECS *, const YAML::Node&, const Entity_ref &);

        typedef std::map<std::string, DeserializeComponentFunc> SerializableComponentRegistry;

        inline static SerializableComponentRegistry &getSerializableComponentRegistry() {
            static SerializableComponentRegistry reg;
            return reg;
        }

        class I_Entity_storage {
        public:
            virtual I_Component *unsafe_get(std::size_t ID) = 0;

            virtual bool has_ID(std::size_t ID) = 0;

            virtual bool try_delete(std::size_t ID) = 0;

            virtual ~I_Entity_storage() = default;
        };

        /**
         * The only purpose of this class is to properly send events when components are deleted.
         * We don't know what type of components to delete when deleting an entity, so the
         * event must be posted from a sparse table. Otherwise we would need one event type for all
         * components, even if not all component iterators are required to refresh their cache every time
         * an entity is deleted.
         * @tparam C
         */
        template<class C, std::enable_if<std::is_base_of<I_Component, C>::value>::type * = nullptr>
        class Entity_storage
                : public I_Entity_storage, public Sparse_table<ECS_ENTITY_SIZE, ECS_PAGE_BYTES, ECS_OFFSET_TYPE, C> {
        public:
            I_Component *unsafe_get(std::size_t ID) override {
                if (this->has_ID(ID)) {
                    return &this->operator[](ID);
                }
                return nullptr;
            }

            bool has_ID(std::size_t ID) override {
                return Sparse_table<ECS_ENTITY_SIZE, ECS_PAGE_BYTES, ECS_OFFSET_TYPE, C>::has_ID(ID);
            }

            bool try_delete(std::size_t ID) override {
                return Sparse_table<ECS_ENTITY_SIZE, ECS_PAGE_BYTES, ECS_OFFSET_TYPE, C>::try_delete(ID);
            }

            void delete_entry(const size_t &ID) override {
                Event_system::post<Events::Component_event<C>>(false);
                Sparse_table<ECS_ENTITY_SIZE, ECS_PAGE_BYTES, ECS_OFFSET_TYPE, C>::delete_entry(ID);
            }

            bool try_delete(const size_t &ID) override {
                if (this->has_ID(ID)) {
                    this->delete_entry(ID);
                    return true;
                }
                return false;
            }
        };


        template<class T, std::enable_if<std::is_base_of<Cecilion::Serializable, T>::value>::type * = nullptr, std::enable_if<std::is_base_of<Cecilion::I_Component, T>::value>::type * = nullptr>
        static void register_serializable_component_type(const std::string &name) {
            DeserializeComponentFunc func = ECS::add_component<T>;
            getSerializableComponentRegistry().insert(SerializableComponentRegistry::value_type(name, func));
        }

    public:
        using Entity_ID = ECS_ENTITY_SIZE;


        template<class T, std::enable_if<std::is_base_of<Cecilion::Serializable, T>::value>::type* = nullptr, std::enable_if<std::is_base_of<Cecilion::I_Component, T>::value>::type* = nullptr>
        class Serializable_component_entry {
        public:
            static Serializable_component_entry<T>& Instance(const std::string& name) {
                static Serializable_component_entry<T> inst(name);
                return inst;
            }
            private:
               Serializable_component_entry(const std::string &name) {
                   ECS::register_serializable_component_type<T>(name);
               }
        };


#define REGISTER_SERIALIZABLE_COMPONENT(TYPE) \
        template<class T> \
        class Serializable_component_registration;   \
        template<>                                      \
        class Serializable_component_registration<TYPE> {\
        private:                                          \
            static const ::Cecilion::ECS::Serializable_component_entry<TYPE>& reg;\
        }; \
        const ::Cecilion::ECS::Serializable_component_entry<TYPE>& Serializable_component_registration<TYPE>::reg = ::Cecilion::ECS::Serializable_component_entry<TYPE>::Instance(Cecilion::type(typeid(TYPE)));



        ECS(const ECS &other) = delete;

        ECS(ECS &&other) = delete;

        ECS() = default;

        ~ECS();

        Entity_ref create_entity();

        /**
         * TODO Placeholder function for deserialize entity.
         * @param yaml
         * @return
         */
        Entity_ref create_entity(const YAML::Node& yaml);

        void delete_entity(Entity_ID ID);
        //void clear_storage();

        /**
         * Deserialize a component iterator
         * @tparam Components
         * @return
         */
        template<typename... Components>
        Component_iterator<Components...> create_iterator() {
            // ORVOX_ASSERT(this, "Could not find an active ECS context"); TODO Error
            return {this->assign_storage<Components>()...};
        }
    private:
        template<typename C, std::enable_if<std::is_base_of<I_Component, C>::value>::type * = nullptr>
        C &get_component(const ECS_ENTITY_SIZE &entity) {
            // ORVOX_ASSERT(ID, "Tried to access invalid entity ID");
            // ORVOX_ASSERT(this, "Could not find an active ECS context"); TODO Error
            if (this->m_component_storage.count(typeid(C)) == 0) {
                // TODO Log message.
                // ORVOX_ERROR("Could not fetch component of type {0}", typeid(C).name());
                //throw std::invalid_argument("Entity system does not contain any entities of this type");
                // TODO Error
            }
            Entity_storage<C> *container = static_cast<Entity_storage<C> *>(this->m_component_storage[typeid(C)]);
            // ORVOX_ASSERT(container->has_ID(ID), "Tried to acces invalid component");
            if (!container->has_ID(entity)) {
                // TODO Error
            }
            return (*container)[entity];
        }

        template<typename C, std::enable_if<std::is_base_of<I_Component, C>::value>::type * = nullptr, typename... Args>
        Component_ref<C>
        add_component(const ECS_ENTITY_SIZE &entity, const Entity_ref &entity_reference, Args... args) {
            //ORVOX_ASSERT(ID, "Tried to access invalid entity ID");
            // ORVOX_ASSERT(this, "Could not find an active ECS context"); TODO Error
            if (this->m_component_storage.count(typeid(C)) == 0) {
                this->m_component_storage[typeid(C)] = new Entity_storage<C>();
            }

            if (this->m_component_storage[typeid(C)]->has_ID(entity)) {
                // TODO Error
            }

            // ORVOX_TRACE("Emplaced {0} component for ID {1}", typeid(C).name(), ID); TODO Error
            Event_system::post<Events::Component_event<C>>(true);
            static_cast<Entity_storage<C> *>(this->m_component_storage[typeid(C)])->emplace_entry(entity,
                                                                                                  entity_reference,
                                                                                                  std::forward<Args>(
                                                                                                          args)...);
            return Component_ref<C>(entity_reference);
        }

        template<class C>
        static I_Component_ref add_component(ECS *ecs, const YAML::Node& node, const Entity_ref &entity_ref);


        /**
         * TODO Placeholder deserialize function for component C.
         * @tparam C
         * @param yaml
         * @param entity
         * @return
         */
        I_Component_ref add_component(const YAML::Node& yaml, const Entity_ref& entity_reference);

        template<typename C>
        void pop_component(const Entity_ID &ID) {
            // ORVOX_ASSERT(ID, "Tried to access invalid entity ID");
            // ORVOX_ASSERT(this, "Could not find an active ECS context");
            if (this->m_component_storage.count(typeid(C)) == 0) {
                // ORVOX_ERROR("Could not delete component of type {0}", typeid(C).name());
                throw std::invalid_argument("Entity system does not contain any entities of this type");
            }
            Entity_storage<C> *container = static_cast<Entity_storage<C> *>(this->m_component_storage[typeid(C)]);
            if (!container->try_delete(ID)) {
                // ORVOX_ERROR("Tried to delete {0} from entity {1}, but no component was found!", typeid(C).name(), ID);
                return;
            }
        }

        void pop_component(const Entity_ID &entity, std::type_index type) {
            if (this->m_component_storage.count(type) == 0) {
                // ORVOX_ERROR("Could not delete component of type {0}", typeid(C).name());
                throw std::invalid_argument("Entity system does not contain any entities of this type");
            }
            auto container = this->m_component_storage[type];
            if (!container->try_delete(entity)) {
                // ORVOX_ERROR("Tried to delete {0} from entity {1}, but no component was found!", typeid(C).name(), ID);
                return;
            }
        }

        template<class C>
        bool has_component(const Entity_ID &ID) {
            // ORVOX_ASSERT(ID, "Tried to access invalid entity ID");
            // ORVOX_ASSERT(ID, "Tried to access invalid entity ID");
            if (this->m_component_storage.count(typeid(C)) == 0) {
                return false;
            }
            Entity_storage<C> *container = static_cast<Entity_storage<C> *>(this->m_component_storage[typeid(C)]);
            return container->has_ID(ID);
        }

        template<typename C>
        Sparse_table<ECS_ENTITY_SIZE, ECS_PAGE_BYTES, ECS_OFFSET_TYPE, C> *assign_storage() {
            if (this->m_component_storage.count(typeid(C)) == 0) {
                this->m_component_storage[typeid(C)] = new Entity_storage<C>();
                // ORVOX_TRACE("Assigned component storage for {0}", typeid(Component).name());
            }

            return static_cast<Entity_storage<C> *>(this->m_component_storage[typeid(C)]);
        }

    private:

        std::unordered_map<std::type_index, I_Entity_storage *> m_component_storage;

        // TODO This has the possibility to fill up.
        //std::vector<Entity_ID> m_freelist;

        //std::vector<Entity_ID> m_delete_list;
        //Entity_ID m_allocated_entities = ECS_NULL_ENTITY+1;

        Same_arena_allocator<ECS_PAGE_BYTES, ECS_OFFSET_TYPE, Entity_source> m_entities;
    };



//#include "Entity.h"

    struct Entity_source {
        bool operator==(std::nullptr_t _) const {
            return this->m_entity_ID == ECS_NULL_ENTITY;
        }

        bool operator!=(std::nullptr_t _) const {
            return this->m_entity_ID != ECS_NULL_ENTITY;
        }

        bool operator==(const Entity_source& other) const;
        bool operator!=(const Entity_source& other) const;

        ECS_ENTITY_SIZE m_entity_ID = ECS_NULL_ENTITY;
        std::size_t m_refs = 0;
    };

    struct Entity_ref {
        friend class ECS;
        friend class I_Component_ref;
    public:
        using Entity_ID = ECS_ENTITY_SIZE;

        Entity_ref(Entity_ref&& other) noexcept;

        Entity_ref(const Entity_ref& other) noexcept;

        Entity_ref(){}

        Entity_ref& operator=(const Entity_ref& other);

        Entity_ref& operator=(Entity_ref&& obj)  noexcept;

        Entity_ref& operator=(std::nullptr_t&& _) {
            if (this->p_source != nullptr && this->p_source->operator!=(nullptr)) {
                this->p_source->m_refs --;
            }
            this->p_source = nullptr;
            this->p_ecs = nullptr;
            return *this;
        }

        ~Entity_ref();

        void Destroy();

        bool operator==(const Entity_ref &rhs) const;

        bool operator==(const std::nullptr_t& _) const;

        bool operator!=(const Entity_ref &rhs) const {
            return this->p_source != rhs.p_source;
        }

        bool operator!=(const std::nullptr_t& _) const;

        Entity_source& operator->();


        template<typename C, std::enable_if<std::is_base_of<I_Component, C>::value>::type * = nullptr>
        Component_ref<C> get_component() {
            return Component_ref<C>(*this);
        }

        template<typename C, std::enable_if<std::is_base_of<I_Component, C>::value>::type * = nullptr>
        Component_ref<C> add_component() {
            return this->p_ecs->add_component<C>(this->id(),*this);
        }

        I_Component_ref add_component(const YAML::Node& node);

        template<typename C, std::enable_if<std::is_base_of<I_Component, C>::value>::type * = nullptr>
        bool has_component() {
            return this->p_ecs->has_component<C>(this->id());
        }

        template<typename C, std::enable_if<std::is_base_of<I_Component, C>::value>::type * = nullptr>
        void remove_component() {
            this->p_ecs->pop_component<C>(this->id());
        }

    private:
        template<class C, std::enable_if<std::is_base_of<I_Component, C>::value>::type * = nullptr>
        C &get_component_source() {
            return this->p_ecs->get_component<C>(this->p_source->m_entity_ID);
        }

        [[nodiscard]] const Entity_ID& id() const;

        explicit Entity_ref(ECS* ecs, Entity_source* source);

        Entity_source* p_source = nullptr;
        ECS* p_ecs = nullptr;

    };

    class I_Component {
        friend class ECS;
        friend class I_Component_ref;
    public:
        I_Component(I_Component&& other) noexcept;
        I_Component& operator=(I_Component&& other)  noexcept;
        ~I_Component() = default;

        bool operator ==(const std::nullptr_t & _);

        Entity_ref& entity();

        void Destroy();

        template<class C>
        Component_ref<C> get_component();

        template<class C>
        Component_ref<C> add_component();

        template<class C>
        bool has_component();
        //I_Component_ref parse_component(const YAML::Node& node);

    protected:
        explicit I_Component(const Entity_ref& ref);
        Entity_ref m_entity;
        std::size_t m_refs = 0;
    };

    template<class... dependencies>
    class I_Dependency_component : public I_Component {
    protected:
        I_Dependency_component(const Entity_ref& ref): I_Component(ref) {
            ((this->check_dependency<dependencies>())||...);
        }

    private:
        template<class T>
        void check_dependency() {
            if (!this->has_component<T>()) {
                this->add_component<T>();
            }
        }
    };

    class I_Component_ref {
    public:
        Entity_ref& entity() {
            return this->m_entity;
        }
    protected:
        I_Component_ref(){}
        template<class C>
        C& get_component_source() {
            return this->m_entity.get_component_source<C>();
        }
        template<class C, std::enable_if<std::is_base_of<I_Component, C>::value>::type * = nullptr>
        std::size_t add_source_ref() {
            return ++ static_cast<I_Component&>(this->get_component_source<C>()).m_refs;
        }

        template<class C, std::enable_if<std::is_base_of<I_Component, C>::value>::type * = nullptr>
        std::size_t subtract_source_ref() {
            return -- static_cast<I_Component&>(this->get_component_source<C>()).m_refs;
        }
        explicit I_Component_ref(const Entity_ref& entity);
        Entity_ref m_entity;
    };

    template<class C>
    class Component_ref: private I_Component_ref {
    public:
        friend class ECS;
        friend class Entity_ref;

        Component_ref(){}

        ~Component_ref() {
            if (this->m_entity != nullptr) {
                this->template subtract_source_ref<C>();
            }
        }

        Component_ref& operator=(const std::nullptr_t& _) {
            if (this->m_entity != nullptr) {
                ~Component_ref<C>();
                this->m_entity = nullptr;
            }
        }

        C &operator->() {
            if (this->operator==(nullptr)) {
                // TODO Error
                throw std::exception();
            }
            return this->template get_component_source<C>();
        }
        bool operator==(std::nullptr_t _) {
            return this->m_entity == nullptr || this->template get_component_source<C>() == nullptr;
        }

        bool operator!=(std::nullptr_t _) {
            return this->m_entity != nullptr;
        }

        bool operator==(const Component_ref<C>& other) {
            return this->m_entity == other.m_entity;
        }

        template<class T>
        Component_ref<T> add_component() {
            return this->m_entity.template add_component<T>();
        }

        template<class T>
        Component_ref<T> get_component() {
            return this->m_entity.template get_component<T>();
        }

        template<typename T>
        bool has_component() {
            return this->m_entity.template has_component<T>();
        }
    private:
        explicit Component_ref(const Entity_ref &entity): I_Component_ref(std::forward<const Entity_ref>(entity)){
            this->template add_source_ref<C>();
        }

    };

    template<class C>
    I_Component_ref ECS::add_component(ECS *ecs, const YAML::Node& node, const Entity_ref &entity_ref) {
        if (!node.IsDefined() || node.IsNull() || !node.IsMap()) {
            // TODO Better error
            throw std::runtime_error("Invalid Yaml type when parsing ECS component");
        }

        if (node[Cecilion::Serializable::s_type_declaration]) {
            Component_ref<C> component = ecs->add_component<C>(entity_ref.id(),entity_ref);
            component.operator->() = node;
            return static_cast<I_Component_ref>(component);
        } else {
            // TODO Better error
            throw std::runtime_error("could not find type declaration in yaml while deserializing component");
        }
    }

    template<class C>
    Component_ref<C> I_Component::add_component() {
        return this->m_entity.add_component<C>();
    }

    template<class C>
    Component_ref<C> I_Component::get_component() {
        return this->m_entity.get_component<C>();
    }

    template<class C>
    bool I_Component::has_component() {
        return this->m_entity.has_component<C>();
    }
}


