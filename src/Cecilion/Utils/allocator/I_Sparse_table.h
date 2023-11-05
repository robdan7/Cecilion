#include <cstdlib>

namespace Cecilion {
    class I_sparse_table {
    public:
        virtual bool has_ID(const std::size_t& ID) = 0;
        virtual void delete_entry(const std::size_t& ID) = 0;
        virtual bool has_table(const std::size_t& table_index) = 0;
        virtual bool try_delete(const std::size_t& ID) = 0;
        [[nodiscard]] virtual std::size_t packed_size() const = 0;
        [[nodiscard]] virtual std::size_t n_sparse_pages() const = 0;
        //[[nodiscard]] virtual I_sparse_table* make_copy() const = 0;
        virtual ~I_sparse_table() = default;
    protected:
        I_sparse_table() = default;
    };
}