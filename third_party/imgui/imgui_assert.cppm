
export module imgui:assert;
export import std;

namespace imgui
{
    struct assert_error
    {
        const char* expression;
        const char* file;
        int line;
    };

    export class assertion_scope;
    thread_local assertion_scope* active_scope { nullptr };
    thread_local std::optional<assert_error> last_error { };

    export class assertion_scope
    {
    public:
        assertion_scope()
        {
            previous_scope = active_scope;
            active_scope = this;
        }

        [[nodiscard]] std::optional<std::string> error() const
        {
            return last_error
                .transform([] (const auto& error)
                {
                    return std::format(
                        "ImGui assertion failed ({}) at {}:{}",
                        error.expression,
                        error.file,
                        error.line);
                });
        }

        ~assertion_scope()
        {
            active_scope = previous_scope;
        }

        assertion_scope(const assertion_scope&) = delete;
        assertion_scope& operator=(const assertion_scope&) = delete;

        assertion_scope(assertion_scope&& other) noexcept
        {}

        assertion_scope& operator=(assertion_scope&& other) noexcept
        {
            return *this;
        }

    private:
        assertion_scope* previous_scope;
        std::optional<assert_error> last_error {};
    };

    extern "C++" void imgui_assert_handler(const char* expr, const char* file, int line)
    {
        if (active_scope == nullptr)
        {
            std::cerr << std::format("{}:{} panic: {}", file, line, expr) << std::endl;
            std::terminate();
        }

        last_error = {expr, file, line};
    }

}