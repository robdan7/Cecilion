#pragma once
namespace Cecilion {
    template<typename Result_type, typename... Input_params>
    class Compute_query {
    public:
        virtual void execute() = 0;
        virtual Result_type& fetch_result() = 0;
    };
}