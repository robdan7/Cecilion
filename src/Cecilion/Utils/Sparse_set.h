#pragma once

#include <memory>
#include <vector>
#include <type_traits>
#include <cstdio>
#include <limits>
namespace Cecilion {
    /**
     * A packed list/sparse set contains one storage type (vector or multiple arrays) with unique IDs as index and
     * some form of packed list/vector with the same unique IDs as values. Iterating through a sparse set will
     * only iterate through values that are not null, since the packed list will only contain used values.
     *
     * By adding another vector, it is possible to maintain ID to index pointers to some actual data.
     *
     * IMPORTANT:
     * @tparam Type
     */
    template<typename Type>
    class Sparse_set {
    public:
        static const uint32_t page_size = 128;
        static const auto index_per_page = page_size / sizeof(Type);

        Sparse_set() {
//            sparse_list = std::vector<std::unique_ptr<Type[]>>();
//            packed_list = std::vector<Type>();
        }

        /**
         * Get the page offset of an ID in one of the pages. Fetching the offset of an unknown ID
         * lead to undefined behaviour.
         * @param ID
         * @return
         */
        auto offset(Type ID) const {
            return std::size_t(ID & (index_per_page -1));
        }

        /**
         * Get the page index of an ID. Fetching the page of an unknown ID leads to undefined
         * behaviour.
         */
        auto page(Type ID) const {
            return std::size_t(ID / index_per_page);
        }


        /**
         * Return the index to an ID in the packed list. Fetching the index of an unknown ID leads to undefined
         * behaviour.
         * @param ID
         * @return
         */
        auto index(Type ID) {
            return this->sparse_list[page(ID)][offset(ID)];
        }

        /**
         * Return the ID calculated from page and offset within that page.
         * @param page
         * @param offset
         * @return
         */
        auto ID (Type page, Type offset)  const {
            return page * index_per_page + offset;
        }

        /**
         * Get a page in the sparse list. A new page will be created if it does not exist.
         * @param page_index
         * @return
         */
        Type* fetch_page(std::size_t page_index) {
            if (page_index >= this->sparse_list.size()) {
                this->sparse_list.resize(page_index +1);
            }

            if (!this->sparse_list.at(page_index)) {
//                this->sparse_list.insert(page_index, std::make_unique<Type []>(10));
                this->sparse_list[page_index] = std::make_unique<Type []>(index_per_page);
                std::fill_n(this->sparse_list[page_index].get(), index_per_page, NULL_POINTER);
            }
            return this->sparse_list[page_index].get();
        }

        /**
         * Put a unique id in the sparse list. The ID will be overwritten if it exist
         * already. Big no no.
         * @param ID
         */
        void put(Type ID) {
            fetch_page(page(ID))[offset(ID)] = this->packed_list.size();
            this->packed_list.push_back(ID);
        }

        virtual void remove(Type ID) {
            Type replacement = this->packed_list.back();
            std::size_t ID_page = page(ID);
            std::size_t ID_offset = offset(ID);

            /// Let the replacement ID point to the deleted index.
            sparse_list[page(replacement)][offset(replacement)] = sparse_list[ID_page][ID_offset];
            sparse_list[ID_page][ID_offset] = NULL_POINTER;
            /// Move the replacement ID to the new index.
            packed_list[ID] = replacement;
            packed_list.pop_back();
        }

        bool has_ID(Type ID) const {
            const auto ID_page = page(ID);
            /// 1: the page must be in range. 2: the page pointer must be initialized. 3: The index must not be valid.
            return ID_page < sparse_list.size() && this->sparse_list[ID_page] && (sparse_list[ID_page][offset(ID)] != NULL_POINTER);
        }

        bool has_page(Type page) {
            return page < this->sparse_size().size() && this->sparse_list[page];
        }

        auto sparse_size() const {
            return this->sparse_list.size();
        }

        auto packed_size() const {
            return packed_list.size();
        }

        template<typename Function>
        void for_each_packed(Function f) const {
            for (Type ent : this->packed_list) {
                f(ent);
            }
        }

    protected:
         std::vector<std::unique_ptr<Type[]>> sparse_list;
         std::vector<Type> packed_list;
         const Type NULL_POINTER = std::numeric_limits<Type>::max();
    };
}
