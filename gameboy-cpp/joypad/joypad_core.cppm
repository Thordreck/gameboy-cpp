export module joypad:core;
import :common;

namespace joypad
{
    using joypad_imp_is_input_pressed_func_t = bool(*)(const void*, joypad_input);

    template <JoypadInputProvider Imp>
    static [[nodiscard]] bool is_input_pressed_imp(const void* imp, const joypad_input input)
    {
        const Imp* joypad = static_cast<const Imp*>(imp);
        return joypad->is_input_pressed(input);
    }

    export class joypad
    {
    public:
        template <JoypadInputProvider Imp>
        explicit joypad(Imp& imp)
            : imp{&imp}
              , is_input_pressed_func{is_input_pressed_imp<Imp>}
        {
        }

        void set_source(const joypad_source new_source) { source = new_source; }
        [[nodiscard]] joypad_source get_source() const { return source; }

        [[nodiscard]] bool is_input_pressed(const joypad_input input) const
        {
            return is_input_pressed_func(imp, input);
        }

    private:
        void* imp;
        joypad_imp_is_input_pressed_func_t is_input_pressed_func;

        joypad_source source{joypad_source::none};
    };

}
