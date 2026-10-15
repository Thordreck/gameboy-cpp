export module json:types;

import std;

namespace json
{
    export using null = std::nullptr_t;
    export using string = std::string;
    export using number = double;
    export using boolean = bool;
    export class array;
    export class object;

    export using value = std::variant<null, string, number, boolean, array, object>;
    export using document = std::variant<array, object>;

    class array
    {
    public:
        array();
        ~array();

        array(const array&);
        array& operator=(const array&);

        array(array&&) noexcept;
        array& operator=(array&&) noexcept;

        [[nodiscard]] size_t size() const;
        [[nodiscard]] value get(size_t idx) const;
        void set(size_t idx, const value& value);

        void push_back(const value& value);

    private:
        struct imp;
        std::unique_ptr<imp> impl;

        friend struct internal;
    };

    class object
    {
    public:
        object();
        ~object();

        object(const object&);
        object& operator=(const object&);

        object(object&&) noexcept;
        object& operator=(object&&) noexcept;

        [[nodiscard]] size_t size() const;
        [[nodiscard]] bool has(std::string_view key) const;
        [[nodiscard]] value get(std::string_view key) const;
        void set(std::string_view key, const value& value);

    private:
        struct imp;
        std::unique_ptr<imp> impl;

        friend struct internal;
    };

}
