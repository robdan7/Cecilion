#pragma once
#include "I_Sparse_table.h"
#include "Allocator.h"
namespace Cecilion {
    template<typename ID_t, int page_t, typename offset_t, class T>
    class Sparse_linked_table : public I_sparse_table {
    private:
        template<class C>
        struct Container {
            std::size_t m_next;
            C m_item;
            template<typename... Args>
            Container(const std::size_t& next, Args ...args): m_next(next), m_item(std::forward<Args>(args)...) {}
            Container(const std::size_t& next, const C arg): m_next(next), m_item(arg) {}
        };
    public:
        Sparse_linked_table() {
            if (sizeof(ID_t) < page_t + sizeof(offset_t)) {
                throw std::invalid_argument("ID type must be of larger size than page_t + offset_t");
            }
        }

        ~Sparse_linked_table() override = default;


        T& operator[](std::size_t index) {
            auto packed_index = (std::size_t)this->m_index_set[index];
            return this->m_packed_set[packed_index].item;
        }

        std::vector<T&> at_linked(std::size_t index) {
            std::vector<T&> result;
            auto packed_index = (std::size_t)this->m_index_set[index];
            auto& container = this->m_packed_set[packed_index];
            result.emplace_back(container.item);

            while (container.next != packed_index) {

            }
        }

        bool has_table(const std::size_t& table_index) override {
            return this->m_index_set.has_page(table_index);
        }

        bool has_ID(const std::size_t& ID) override {
            if (!this->m_index_set.has_page_indexed(ID)) {
                return false;
            }
            std::size_t index = this->m_index_set[ID];
            return this->m_packed_index.size() > index && this->m_packed_index[index] == ID;
        }

        ID_t get_ID(const std::size_t&& index) const {
            return this->m_packed_index[index];
        }

        ID_t get_ID(const std::size_t& index) const {
            return this->m_packed_index[index];
        }

        template<typename... Args>
        T& emplace_entry(const std::size_t& ID, Args... args) {
            auto packed_index = this->m_packed_set.emplace(std::forward<Args>(args)...);
            this->m_index_set.emplace(ID, packed_index);
            this->m_packed_index.emplace(ID);
            return this->m_packed_set[packed_index].m_item;
        }

        T& push_entry(const std::size_t& ID, const T& arg) {
            auto packed_index = this->m_packed_set.push(arg);
            this->m_index_set.emplace(ID, packed_index);
            this->m_packed_index.push(ID);
            return this->m_packed_set[packed_index].m_item;
        }

        void delete_entry(const std::size_t& ID) override {
            auto index = this->m_index_set[ID];
            auto collapsed_ID = this->m_packed_index[this->m_packed_index.size() - 1];
            if (index != this->m_packed_index.size()-1) {
                this->m_index_set[collapsed_ID] = index;
            }
            this->m_index_set.free(ID);
            this->m_packed_set.collapse(index);
            this->m_packed_index.collapse(index);
        }

        bool try_delete(const std::size_t& ID) override {
            if (this->has_ID(ID)) {
                this->delete_entry(ID);
                return true;
            }
            return false;
        }

        auto& get_sparse_set() {
            return this->m_index_set;
        }

        auto& get_packed_set() {
            return this->m_packed_set;
        }

        [[nodiscard]] std::size_t n_sparse_pages() const override {
            return this->m_index_set.n_pages();
        }

        [[nodiscard]] std::size_t packed_size() const override {
            return this->m_packed_set.size();
        }

    private:
        Cecilion::Pool_array<page_t, offset_t, ID_t> m_index_set;
        Cecilion::pool_stack<page_t, offset_t, ID_t> m_packed_index;
        Cecilion::pool_stack<page_t, offset_t, Container<T>> m_packed_set;
    };
}