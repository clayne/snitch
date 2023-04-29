#ifndef SNITCH_MACROS_EXCEPTIONS_HPP
#define SNITCH_MACROS_EXCEPTIONS_HPP

#if SNITCH_WITH_EXCEPTIONS
#    include "snitch/snitch_config.hpp"
#    include "snitch/snitch_matcher.hpp"
#    include "snitch/snitch_registry.hpp"

#    include <exception>

#    define SNITCH_REQUIRE_THROWS_AS(EXPRESSION, ...)                                              \
        do {                                                                                       \
            auto& SNITCH_CURRENT_TEST = snitch::impl::get_current_test();                          \
            try {                                                                                  \
                ++SNITCH_CURRENT_TEST.asserts;                                                     \
                EXPRESSION;                                                                        \
                SNITCH_CURRENT_TEST.reg.report_failure(                                            \
                    SNITCH_CURRENT_TEST, {__FILE__, __LINE__},                                     \
                    #__VA_ARGS__ " expected but no exception thrown");                             \
                SNITCH_TESTING_ABORT;                                                              \
            } catch (const __VA_ARGS__&) {                                                         \
                /* success */                                                                      \
            } catch (...) {                                                                        \
                try {                                                                              \
                    throw;                                                                         \
                } catch (const std::exception& e) {                                                \
                    SNITCH_CURRENT_TEST.reg.report_failure(                                        \
                        SNITCH_CURRENT_TEST, {__FILE__, __LINE__},                                 \
                        #__VA_ARGS__ " expected but other std::exception thrown; message: ",       \
                        e.what());                                                                 \
                } catch (...) {                                                                    \
                    SNITCH_CURRENT_TEST.reg.report_failure(                                        \
                        SNITCH_CURRENT_TEST, {__FILE__, __LINE__},                                 \
                        #__VA_ARGS__ " expected but other unknown exception thrown");              \
                }                                                                                  \
                SNITCH_TESTING_ABORT;                                                              \
            }                                                                                      \
        } while (0)

#    define SNITCH_CHECK_THROWS_AS(EXPRESSION, ...)                                                \
        do {                                                                                       \
            auto& SNITCH_CURRENT_TEST = snitch::impl::get_current_test();                          \
            try {                                                                                  \
                ++SNITCH_CURRENT_TEST.asserts;                                                     \
                EXPRESSION;                                                                        \
                SNITCH_CURRENT_TEST.reg.report_failure(                                            \
                    SNITCH_CURRENT_TEST, {__FILE__, __LINE__},                                     \
                    #__VA_ARGS__ " expected but no exception thrown");                             \
            } catch (const __VA_ARGS__&) {                                                         \
                /* success */                                                                      \
            } catch (...) {                                                                        \
                try {                                                                              \
                    throw;                                                                         \
                } catch (const std::exception& e) {                                                \
                    SNITCH_CURRENT_TEST.reg.report_failure(                                        \
                        SNITCH_CURRENT_TEST, {__FILE__, __LINE__},                                 \
                        #__VA_ARGS__ " expected but other std::exception thrown; message: ",       \
                        e.what());                                                                 \
                } catch (...) {                                                                    \
                    SNITCH_CURRENT_TEST.reg.report_failure(                                        \
                        SNITCH_CURRENT_TEST, {__FILE__, __LINE__},                                 \
                        #__VA_ARGS__ " expected but other unknown exception thrown");              \
                }                                                                                  \
            }                                                                                      \
        } while (0)

#    define SNITCH_REQUIRE_THROWS_MATCHES(EXPRESSION, EXCEPTION, ...)                              \
        do {                                                                                       \
            auto& SNITCH_CURRENT_TEST = snitch::impl::get_current_test();                          \
            try {                                                                                  \
                ++SNITCH_CURRENT_TEST.asserts;                                                     \
                EXPRESSION;                                                                        \
                SNITCH_CURRENT_TEST.reg.report_failure(                                            \
                    SNITCH_CURRENT_TEST, {__FILE__, __LINE__},                                     \
                    #EXCEPTION " expected but no exception thrown");                               \
                SNITCH_TESTING_ABORT;                                                              \
            } catch (const EXCEPTION& e) {                                                         \
                auto&& SNITCH_TEMP_MATCHER = __VA_ARGS__;                                          \
                if (!SNITCH_TEMP_MATCHER.match(e)) {                                               \
                    SNITCH_CURRENT_TEST.reg.report_failure(                                        \
                        SNITCH_CURRENT_TEST, {__FILE__, __LINE__},                                 \
                        "could not match caught " #EXCEPTION " with expected content: ",           \
                        SNITCH_TEMP_MATCHER.describe_match(                                        \
                            e, snitch::matchers::match_status::failed));                           \
                    SNITCH_TESTING_ABORT;                                                          \
                }                                                                                  \
            } catch (...) {                                                                        \
                try {                                                                              \
                    throw;                                                                         \
                } catch (const std::exception& e) {                                                \
                    SNITCH_CURRENT_TEST.reg.report_failure(                                        \
                        SNITCH_CURRENT_TEST, {__FILE__, __LINE__},                                 \
                        #EXCEPTION " expected but other std::exception thrown; message: ",         \
                        e.what());                                                                 \
                } catch (...) {                                                                    \
                    SNITCH_CURRENT_TEST.reg.report_failure(                                        \
                        SNITCH_CURRENT_TEST, {__FILE__, __LINE__},                                 \
                        #EXCEPTION " expected but other unknown exception thrown");                \
                }                                                                                  \
                SNITCH_TESTING_ABORT;                                                              \
            }                                                                                      \
        } while (0)

#    define SNITCH_CHECK_THROWS_MATCHES(EXPRESSION, EXCEPTION, ...)                                \
        do {                                                                                       \
            auto& SNITCH_CURRENT_TEST = snitch::impl::get_current_test();                          \
            try {                                                                                  \
                ++SNITCH_CURRENT_TEST.asserts;                                                     \
                EXPRESSION;                                                                        \
                SNITCH_CURRENT_TEST.reg.report_failure(                                            \
                    SNITCH_CURRENT_TEST, {__FILE__, __LINE__},                                     \
                    #EXCEPTION " expected but no exception thrown");                               \
            } catch (const EXCEPTION& e) {                                                         \
                auto&& SNITCH_TEMP_MATCHER = __VA_ARGS__;                                          \
                if (!SNITCH_TEMP_MATCHER.match(e)) {                                               \
                    SNITCH_CURRENT_TEST.reg.report_failure(                                        \
                        SNITCH_CURRENT_TEST, {__FILE__, __LINE__},                                 \
                        "could not match caught " #EXCEPTION " with expected content: ",           \
                        SNITCH_TEMP_MATCHER.describe_match(                                        \
                            e, snitch::matchers::match_status::failed));                           \
                }                                                                                  \
            } catch (...) {                                                                        \
                try {                                                                              \
                    throw;                                                                         \
                } catch (const std::exception& e) {                                                \
                    SNITCH_CURRENT_TEST.reg.report_failure(                                        \
                        SNITCH_CURRENT_TEST, {__FILE__, __LINE__},                                 \
                        #EXCEPTION " expected but other std::exception thrown; message: ",         \
                        e.what());                                                                 \
                } catch (...) {                                                                    \
                    SNITCH_CURRENT_TEST.reg.report_failure(                                        \
                        SNITCH_CURRENT_TEST, {__FILE__, __LINE__},                                 \
                        #EXCEPTION " expected but other unknown exception thrown");                \
                }                                                                                  \
            }                                                                                      \
        } while (0)

// clang-format off
#if SNITCH_WITH_SHORTHAND_MACROS
#    define REQUIRE_THROWS_AS(EXPRESSION, ...)                 SNITCH_REQUIRE_THROWS_AS(EXPRESSION, __VA_ARGS__)
#    define CHECK_THROWS_AS(EXPRESSION, ...)                   SNITCH_CHECK_THROWS_AS(EXPRESSION, __VA_ARGS__)
#    define REQUIRE_THROWS_MATCHES(EXPRESSION, EXCEPTION, ...) SNITCH_REQUIRE_THROWS_MATCHES(EXPRESSION, EXCEPTION, __VA_ARGS__)
#    define CHECK_THROWS_MATCHES(EXPRESSION, EXCEPTION, ...)   SNITCH_CHECK_THROWS_MATCHES(EXPRESSION, EXCEPTION, __VA_ARGS__)
#endif
// clang-format on

#endif

#endif