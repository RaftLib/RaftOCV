#pragma once

#include <raft>
// primary template.
template<class T>
struct function_traits : function_traits<decltype(&T::operator())> {
};

// partial specialization for function type
template<class R, class... Args>
struct function_traits<R(Args...)> {
    using result_type = R;
    using argument_types = std::tuple<Args...>;
};

// partial specialization for function pointer
template<class R, class... Args>
struct function_traits<R (*)(Args...)> {
    using result_type = R;
    using argument_types = std::tuple<Args...>;
};

// partial specialization for std::function
template<class R, class... Args>
struct function_traits<std::function<R(Args...)>> {
    using result_type = R;
    using argument_types = std::tuple<Args...>;
};

// partial specialization for pointer-to-member-function (i.e., operator()'s)
template<class T, class R, class... Args>
struct function_traits<R (T::*)(Args...)> {
    using result_type = R;
    using argument_types = std::tuple<Args...>;
};

template<class T, class R, class... Args>
struct function_traits<R (T::*)(Args...) const> {
    using result_type = R;
    using argument_types = std::tuple<Args...>;
};

template< class T >
struct remove_cvref {
    typedef std::remove_cv_t<std::remove_reference_t<T>> type;
};

template <typename F>
struct FunctorFilter : public raft::kernel {
    F f;

    typedef typename remove_cvref<typename std::tuple_element<0, typename function_traits<F>::argument_types>::type>::type input_t;
    typedef typename std::result_of<F(input_t)>::type output_t;

    template <typename... Args>
    FunctorFilter(Args&&... args) : f(args...) {
        output.addPort<input_t>("0");
        input.add_port<output_t>("0");
    }

    raft::kstatus run() override {
        input_t in;
        input["0"].pop(in);

        output["0"].push(f(in));
        return raft::proceed;
    }

};