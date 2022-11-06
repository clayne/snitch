#include "testing.hpp"

std::size_t test_object_instances = 0u;
bool        function_called       = false;

struct test_object {
    test_object() noexcept {
        ++test_object_instances;
    }
    test_object(const test_object&) noexcept {
        ++test_object_instances;
    }
    test_object(test_object&&) noexcept {
        ++test_object_instances;
    }
};

using function_0_void = void() noexcept;
using function_0_int  = int() noexcept;
using function_2_void = void(int, test_object) noexcept;
using function_2_int  = int(int, test_object) noexcept;

template<typename S>
struct test_class;

template<typename R, typename... Args>
struct test_class<R(Args...) noexcept> {
    R method(Args...) noexcept {
        function_called = true;
        if constexpr (!std::is_same_v<R, void>) {
            return 42;
        }
    }

    R method_const(Args...) const noexcept {
        function_called = true;
        if constexpr (!std::is_same_v<R, void>) {
            return 43;
        }
    }

    static R method_static(Args...) noexcept {
        function_called = true;
        if constexpr (!std::is_same_v<R, void>) {
            return 44;
        }
    }
};

// Needed for GCC.
template struct test_class<function_0_void>;
template struct test_class<function_0_int>;
template struct test_class<function_2_void>;
template struct test_class<function_2_int>;

template<typename T>
struct type_holder {};

TEMPLATE_TEST_CASE(
    "small function",
    "[utility]",
    function_0_void,
    function_0_int,
    function_2_void,
    function_2_int) {

    [&]<typename R, typename... Args>(type_holder<R(Args...) noexcept>) {
        auto run_tests = [&](Args... values) {
            snatch::small_function<TestType> f;

            test_object_instances                    = 0u;
            function_called                          = false;
            constexpr std::size_t expected_instances = sizeof...(Args) > 0 ? 2u : 0u;

            SECTION("from free function") {
                f = &test_class<TestType>::method_static;

                if constexpr (std::is_same_v<R, void>) {
                    if constexpr (sizeof...(Args) > 0) {
                        f(values...);
                    } else {
                        f();
                    }
                } else {
                    int return_value = 0;
                    if constexpr (sizeof...(Args) > 0) {
                        return_value = f(values...);
                    } else {
                        return_value = f();
                    }
                    CHECK(return_value == 44);
                }

                CHECK(function_called);
                CHECK(test_object_instances <= expected_instances);
            }

            SECTION("from non-const member function") {
                test_class<TestType> obj;
                f = {obj, snatch::constant<&test_class<TestType>::method>{}};

                if constexpr (std::is_same_v<R, void>) {
                    if constexpr (sizeof...(Args) > 0) {
                        f(values...);
                    } else {
                        f();
                    }
                } else {
                    int return_value = 0;
                    if constexpr (sizeof...(Args) > 0) {
                        return_value = f(values...);
                    } else {
                        return_value = f();
                    }
                    CHECK(return_value == 42);
                }

                CHECK(function_called);
                CHECK(test_object_instances <= expected_instances);
            }

            SECTION("from const member function") {
                const test_class<TestType> obj;
                f = {obj, snatch::constant<&test_class<TestType>::method_const>{}};

                if constexpr (std::is_same_v<R, void>) {
                    if constexpr (sizeof...(Args) > 0) {
                        f(values...);
                    } else {
                        f();
                    }
                } else {
                    int return_value = 0;
                    if constexpr (sizeof...(Args) > 0) {
                        return_value = f(values...);
                    } else {
                        return_value = f();
                    }
                    CHECK(return_value == 43);
                }

                CHECK(function_called);
                CHECK(test_object_instances <= expected_instances);
            }

            SECTION("from lambda") {
                f = snatch::small_function<TestType>{[](Args...) noexcept -> R {
                    function_called = true;
                    if constexpr (!std::is_same_v<R, void>) {
                        return 45;
                    }
                }};

                if constexpr (std::is_same_v<R, void>) {
                    if constexpr (sizeof...(Args) > 0) {
                        f(values...);
                    } else {
                        f();
                    }
                } else {
                    int return_value = 0;
                    if constexpr (sizeof...(Args) > 0) {
                        return_value = f(values...);
                    } else {
                        return_value = f();
                    }
                    CHECK(return_value == 45);
                }

                CHECK(function_called);
                CHECK(test_object_instances <= expected_instances);
            }
        };

        std::apply(run_tests, std::tuple<Args...>{});
    }
    (type_holder<TestType>{});
};
