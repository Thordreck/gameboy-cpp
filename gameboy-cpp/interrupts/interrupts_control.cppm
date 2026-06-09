export module interrupts:control;

import std;
import :common;

namespace interrupts
{
    export class interrupt_controller
    {
    public:
        interrupt_controller() = default;

        void request(const interrupt request)
        {
            request_flags |= request.if_flag();
        }

        void clear_request(const interrupt request)
        {
            request_flags &= ~request.if_flag();
        }

        void set_requested(const interrupt rq, const bool requested)
        {
            if (requested)
            {
                request(rq);
            }
            else
            {
                clear_request(rq);
            }
        }

        [[nodiscard]] bool is_requested(const interrupt request) const
        {
            return (request_flags & request.if_flag()) == request.if_flag();
        }

        void enable(const interrupt request)
        {
            enable_flags |= request.ie_flag();
        }

        void disable(const interrupt request)
        {
            enable_flags &= ~request.ie_flag();
        }

        void set_enabled(const interrupt request, const bool enabled)
        {
            if (enabled)
            {
                enable(request);
            }
            else
            {
                disable(request);
            }
        }

        [[nodiscard]] bool is_enabled(const interrupt request) const
        {
            return (enable_flags & request.ie_flag()) == request.ie_flag();
        }

        [[nodiscard]] bool is_pending(const interrupt interrupt) const
        {
            return is_enabled(interrupt) && is_requested(interrupt);
        }

        [[nodiscard]] bool is_any_pending() const
        {
            return (enable_flags & ie_mask) & (request_flags & if_mask);
        }

        [[nodiscard]] std::optional<interrupt> get_first_pending() const
        {
            constexpr std::array interrupts_by_priority
            {
                vblank_interrupt,
                lcd_interrupt,
                timer_interrupt,
                serial_interrupt,
                joypad_interrupt
            };

            for (const auto interrupt : interrupts_by_priority)
            {
                if (is_pending(interrupt))
                {
                    return interrupt;
                }
            }

            return std::nullopt;
        }

        interrupt_controller(const interrupt_controller&) = delete;
        interrupt_controller& operator=(const interrupt_controller&) = delete;

    private:
        std::uint8_t enable_flags {};
        std::uint8_t request_flags {};
    };

}