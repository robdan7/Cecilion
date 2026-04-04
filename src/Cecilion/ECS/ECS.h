#pragma once

#include <memory>
#include <utility>
#include <typeinfo>
#include <cstdint>
#include <typeindex>
#include <unordered_map>
#include "Config.h"
#include <Utils/allocator/Sparse_table.h>
#include <Event/System_events.h>
#include "Component_iterator.h"
#include <yaml-cpp/yaml.h>
#include <Utils/Serializable.h>
#include <Utils/Uuid.h>

namespace Cecilion {
    class I_Component;

    class I_Component_ref;

    class ECS;

    class Entity_ref;

    class Entity_metadata;

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
        template<class C, std::enable_if_t<std::is_base_of_v<I_Component, C>> * = nullptr>
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


        template<class T, std::enable_if_t<std::is_base_of_v<Cecilion::Serializable, T>> * = nullptr, std::enable_if_t<std::is_base_of_v<Cecilion::I_Component, T>> * = nullptr>
        static void register_serializable_component_type(const std::string &name);

    public:
        using Entity_ID = ECS_ENTITY_SIZE;


        template<class T, std::enable_if_t<std::is_base_of_v<Cecilion::Serializable, T>>* = nullptr, std::enable_if_t<std::is_base_of_v<Cecilion::I_Component, T>>* = nullptr>
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

        void delete_entity(const Entity_ref &reference);

        template<typename C, std::enable_if_t<std::is_base_of_v<I_Component, C>> * = nullptr>
        C &get_component(const ECS_ENTITY_SIZE &entity) {
            // ORVOX_ASSERT(ID, "Tried to access invalid entity ID");
            // ORVOX_ASSERT(this, "Could not find an active ECS context"); TODO Error
            if (!this->m_component_storage.contains(typeid(C))) {
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

        template<typename C, std::enable_if_t<std::is_base_of_v<I_Component, C>> * = nullptr, typename... Args>
        Component_ref<C>
        add_component(const Entity_ref &entity_reference, Args... args);/* {
            //ORVOX_ASSERT(ID, "Tried to access invalid entity ID");
            // ORVOX_ASSERT(this, "Could not find an active ECS context"); TODO Error
            if (this->m_component_storage.count(typeid(C)) == 0) {
                this->m_component_storage[typeid(C)] = new Entity_storage<C>();
            }

            if (this->m_component_storage[typeid(C)]->has_ID(entity)) {
                // TODO Error
                throw std::runtime_error("Entity system does not contain any entity with this ID");
            }

            // ORVOX_TRACE("Emplaced {0} component for ID {1}", typeid(C).name(), ID); TODO Error
            Event_system::post<Events::Component_event<C>>(true);
            static_cast<Entity_storage<C> *>(this->m_component_storage[typeid(C)])->emplace_entry(entity,
                                                                                                  entity_reference,
                                                                                                  std::forward<Args>(
                                                                                                          args)...);

            const auto id = static_cast<Entity_storage<Entity_metadata> *>(this->m_component_storage[typeid(Entity_metadata)])->emplace(entity_reference);
            return Component_ref<C>(entity_reference);
        }*/

        template<class C>
        static I_Component_ref add_component(ECS *ecs, const YAML::Node& node, const Entity_ref &entity_ref);


        /**
         * TODO Placeholder deserialize function for component C.
         * @tparam C
         * @param yaml
         * @param entity_reference
         * @return
         */
        I_Component_ref add_component(const YAML::Node& yaml, const Entity_ref& entity_reference);

        template<typename C>
        void pop_component(const Entity_ID &ID) {
            // ORVOX_ASSERT(ID, "Tried to access invalid entity ID");
            // ORVOX_ASSERT(this, "Could not find an active ECS context");
            if (!this->m_component_storage.contains(typeid(C))) {
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
            if (!this->m_component_storage.contains(type)) {
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
            if (!this->m_component_storage.contains(typeid(C))) {
                return false;
            }
            Entity_storage<C> *container = static_cast<Entity_storage<C> *>(this->m_component_storage[typeid(C)]);
            return container->has_ID(ID);
        }

        template<typename C>
        Sparse_table<ECS_ENTITY_SIZE, ECS_PAGE_BYTES, ECS_OFFSET_TYPE, C> *assign_storage() {
            if (!this->m_component_storage.contains(typeid(C))) {
                this->m_component_storage[typeid(C)] = new Entity_storage<C>();
                // ORVOX_TRACE("Assigned component storage for {0}", typeid(Component).name());
            }

            return static_cast<Entity_storage<C> *>(this->m_component_storage[typeid(C)]);
        }

        YAML::Node serialize(const Entity_ref& entity) const;


    private:

        std::unordered_map<std::type_index, I_Entity_storage *> m_component_storage;

        // TODO This has the possibility to fill up.
        //std::vector<Entity_ID> m_freelist;

        //std::vector<Entity_ID> m_delete_list;
        //Entity_ID m_allocated_entities = ECS_NULL_ENTITY+1;

    };



    class Entity_ref {
        friend class ECS;
        friend class I_Component_ref;
    public:
        using Entity_ID = ECS_ENTITY_SIZE;

        //YAML::Node serialize() override;

        //Serializable &operator=(const YAML::Node &serializedNode) override;

        Entity_ref(Entity_ref&& other) noexcept;

        Entity_ref(const Entity_ref& other) noexcept;



        Entity_ref& operator=(const Entity_ref& other);

        Entity_ref& operator=(Entity_ref&& obj)  noexcept;

        // TODO Remove this?
        Entity_ref& operator=(std::nullptr_t&& _);

        ~Entity_ref();

        void Destroy() const;

        bool operator==(const Entity_ref &rhs) const;

        bool operator==(const std::nullptr_t& _) const;

        bool operator!=(const Entity_ref &rhs) const {
            return this->m_source != rhs.m_source;
        }

        bool operator!=(const std::nullptr_t& _) const;

        //Entity_source& operator->();


        template<typename C, std::enable_if_t<std::is_base_of_v<I_Component, C>> * = nullptr>
        Component_ref<C> get_component() {
            return Component_ref<C>(*this);
        }

        template<typename C, std::enable_if_t<std::is_base_of_v<I_Component, C>> * = nullptr>
        Component_ref<C> add_component() {
            return this->p_ecs->add_component<C>(*this);
        }

        [[nodiscard]] I_Component_ref add_component(const YAML::Node& node) const;

        template<typename C, std::enable_if_t<std::is_base_of_v<I_Component, C>> * = nullptr>
        bool has_component() const {
            return this->p_ecs->has_component<C>(this->id());
        }

        template<typename C, std::enable_if_t<std::is_base_of_v<I_Component, C>> * = nullptr>
        void remove_component() const {
            this->p_ecs->pop_component<C>(this->id());
        }

    private:
        Entity_ref()= default;

        template<class C, std::enable_if_t<std::is_base_of_v<I_Component, C>> * = nullptr>
        C &get_component_source() {
            return this->p_ecs->get_component<C>(this->id());
        }

        [[nodiscard]] const Entity_ID& id() const;

        explicit Entity_ref(ECS* ecs, const ECS_ENTITY_SIZE &source);

        ECS* p_ecs = nullptr;
        ECS_ENTITY_SIZE m_source = ECS_NULL_ENTITY;
    };

    class I_Component {
        friend class ECS;
        friend class I_Component_ref;
    public:
        I_Component(I_Component&& other) noexcept: m_entity(std::move(other.m_entity)) {
            other.m_entity = nullptr;
        }
        I_Component& operator=(I_Component&& other)  noexcept{
            this->m_entity.operator=(std::move(other.m_entity));
            return *this;
        }
        virtual ~I_Component() = default;

        bool operator ==(const std::nullptr_t & _);

        Entity_ref& entity()  {
            return this->m_entity;
        }

        void Destroy() {
            // TODO This does not seem right
            this->m_entity = nullptr;
        }

        template<class C>
        Component_ref<C> get_component();

        template<class C>
        Component_ref<C> add_component();

        template<class C>
        bool has_component();
        //I_Component_ref parse_component(const YAML::Node& node);

    protected:
        explicit I_Component(const Entity_ref& ref) : m_entity(std::forward<const Entity_ref>(ref)){

        }
        Entity_ref m_entity;
    };

    class Entity_metadata : public I_Component {
    public:
        friend Entity_ref;
        Entity_metadata(const Entity_ref& ref): I_Component(ref) {}

        bool operator==(std::nullptr_t _) const {
            return this->m_entity_ID == ECS_NULL_ENTITY;
        }

        bool operator!=(std::nullptr_t _) const {
            return this->m_entity_ID != ECS_NULL_ENTITY;
        }

        bool operator==(const Entity_metadata& other) const;
        bool operator!=(const Entity_metadata& other) const;
    private:
        ECS_ENTITY_SIZE m_entity_ID = ECS_NULL_ENTITY;
        //std::size_t m_refs = 0;
        Cecilion::Uuid m_uuid = Uuid(); // Holds UUID for serialization and entity traceability.
    };

    template<class... dependencies>
    class I_Dependency_component : public I_Component {
    protected:
        I_Dependency_component(const Entity_ref& ref): I_Component(ref) {
            ((this->check_dependency<dependencies>())||...);
        }

        I_Dependency_component(I_Dependency_component&& other) noexcept: I_Component(std::move(other)) {}

    private:
        template<class T>
        void check_dependency() {
            if (!this->has_component<T>()) {
                this->add_component<T>();
            }
        }
    };

    class I_Component_ref {
        friend class ECS;
    public:
        Entity_ref& entity() {
            return this->m_entity;
        }

        I_Component_ref& operator=(I_Component_ref&& other) noexcept {
            this->m_entity = std::move(other.m_entity);
            return *this;
        }
    protected:
        I_Component_ref(){}

        I_Component_ref(I_Component_ref&& other) noexcept: m_entity(std::move(other.m_entity)) {}

        I_Component_ref(const I_Component_ref& other) = default;

        template<class C>
        C& get_component_source() {
            return this->m_entity.get_component_source<C>();
        }
        /*
        template<class C, std::enable_if_t<std::is_base_of_v<I_Component, C>> * = nullptr>
        std::size_t add_source_ref() {
            return ++ static_cast<I_Component&>(this->get_component_source<C>()).m_refs;
        }*/

        /*
        template<class C, std::enable_if_t<std::is_base_of_v<I_Component, C>> * = nullptr>
        std::size_t subtract_source_ref() {
            return -- static_cast<I_Component&>(this->get_component_source<C>()).m_refs;
        }*/
        explicit I_Component_ref(const Entity_ref& entity);
        Entity_ref m_entity;

    private:
        template<class C>
        static I_Component_ref deserialize_helper(ECS * ecs, const YAML::Node& node, const Entity_ref &entity) {
            return Component_ref<C>(entity);
        }
    };

    template<class C>
    class Component_ref: I_Component_ref {
    public:
        friend class ECS;
        friend class I_Component_ref;
        friend class Entity_ref;

        // TODO Should this be allowed?
        Component_ref()= default;

        ~Component_ref() = default;

        Component_ref(Component_ref&& other) noexcept: I_Component_ref(std::forward<Component_ref>(other.m_entity)) {}

        Component_ref(const Component_ref& other): I_Component_ref(other.m_entity) {}


        Component_ref& operator=(Component_ref&& other) noexcept {
            this->m_entity = std::move(other.m_entity);
            return *this;
        }

        /*
        Component_ref& operator=(const std::nullptr_t& _) {
            if (this->m_entity != nullptr) {
                this->subtract_source_ref<C>();
                this->m_entity = nullptr;
            }
            return *this;
        }*/

        C &operator->() {
            if (this->operator==(nullptr)) {
                // TODO Error
                throw std::exception();
            }
            return this->get_component_source<C>();
        }
        bool operator==(std::nullptr_t _) {
            return this->m_entity == nullptr || this->has_component<C>() == false;
        }

        bool operator!=(std::nullptr_t _) {
            return this->m_entity != nullptr;
        }

        bool operator==(const Component_ref<C>& other) {
            return this->m_entity == other.m_entity;
        }

        template<class T>
        Component_ref<T> add_component() {
            return this->m_entity.add_component<T>();
        }

        template<class T>
        Component_ref<T> get_component() {
            return this->m_entity.get_component<T>();
        }

        template<typename T>
        [[nodiscard]] bool has_component() const {
            return this->m_entity.has_component<T>();
        }
/*
        YAML::Node serialize() override {
            YAML::Node n;
            n[Cecilion::Serializable::s_type_declaration] = Cecilion::type(*this);
            if (this->operator!=(nullptr)) {
                n["m_entity"] = this->m_entity.operator->().m_entity_ID;
            }
            return n;
        }

        Component_ref<C> &operator=(const YAML::Node &serializedNode) override {
            if (serializedNode["m_entity"]) {
                this->m_entity.operator->().m_entity_ID = serializedNode["m_entity"].as<ECS_ENTITY_SIZE>();
            }
            return *this;
        }
*/

    private:
        explicit Component_ref(const Entity_ref &entity): I_Component_ref(std::forward<const Entity_ref>(entity)){}

    };

    template<class C>
   I_Component_ref ECS::add_component(ECS *ecs, const YAML::Node& node, const Entity_ref &entity_ref) {
        if (!node.IsDefined() || node.IsNull() || !node.IsMap()) {
            // TODO Better error
            throw std::runtime_error("Invalid Yaml type when parsing ECS component");
        }

        if (node[Cecilion::Serializable::s_type_declaration]) {
            Component_ref<C> component = ecs->add_component<C>(entity_ref);
            component.operator->() = node;
            return static_cast<I_Component_ref>(component);
        }
        // TODO Better error
        throw std::runtime_error("could not find type declaration in yaml while deserializing component");
   }

    template<typename C, std::enable_if_t<std::is_base_of_v<I_Component, C>> *, typename ... Args>
    Component_ref<C> ECS::add_component(const Entity_ref &entity_reference, Args... args) {
        //ORVOX_ASSERT(ID, "Tried to access invalid entity ID");
        // ORVOX_ASSERT(this, "Could not find an active ECS context"); TODO Error
        if (!this->m_component_storage.contains(typeid(C))) {
            this->m_component_storage[typeid(C)] = new Entity_storage<C>();
        }

        if (this->m_component_storage[typeid(C)]->has_ID(entity_reference.id())) {
            // TODO Exception
            throw std::runtime_error("Entity system already contains a component with ID");
        }

        // ORVOX_TRACE("Emplaced {0} component for ID {1}", typeid(C).name(), ID); TODO Error
        Event_system::post<Events::Component_event<C>>(true);
        static_cast<Entity_storage<C> *>(this->m_component_storage[typeid(C)])->emplace_with_id(entity_reference.id(),
                                                                                              entity_reference,
                                                                                              std::forward<Args>(
                                                                                                      args)...);

        //static_cast<Entity_storage<Entity_metadata> *>(this->m_component_storage[typeid(Entity_metadata)])->emplace(entity_reference);
        return Component_ref<C>(entity_reference);
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

    template<class T, std::enable_if_t<std::is_base_of_v<Cecilion::Serializable, T>> *, std::enable_if_t<std::is_base_of_v<Cecilion::I_Component, T>> *>
    void ECS::register_serializable_component_type(const std::string &name) {
        DeserializeComponentFunc func = ECS::add_component<T>;
        getSerializableComponentRegistry().insert(SerializableComponentRegistry::value_type(name, func));

        DeserializeComponentFunc ref_func = I_Component_ref::deserialize_helper<T>;
        getSerializableComponentRegistry().insert(SerializableComponentRegistry::value_type (name,ref_func));
    }
/*
    template<class T, std::enable_if_t<std::is_base_of_v<Cecilion::Serializable, T>> *E0, std::enable_if_t<std::
        is_base_of_v<Cecilion::I_Component, T>> *E1>
    ECS::Serializable_component_entry<T> & ECS::Serializable_component_entry<T, E0, E1>::Instance(
        const std::string &name) {
        static Serializable_component_entry<T> inst(name);
        return inst;
    }*/
}


