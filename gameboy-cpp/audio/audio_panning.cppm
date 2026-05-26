
export module audio:panning;
import :common;

namespace audio
{
    export template <typename T>
    [[nodiscard]] stereo<T> apply_panning(const T mono, const stereo_panning panning)
    {
        return
        {
            panning.left ? mono : T{},
            panning.right ? mono : T{},
        };
    }

    export class panning
    {
    public:
        [[nodiscard]] stereo_panning get_config() const { return config; }
        void set_config(const stereo_panning config) { this->config = config; }

        template <typename T>
        [[nodiscard]] stereo<T> apply(const T mono)
        {
            return apply_panning(mono, config);
        }

    private:
        stereo_panning config {};
    };

}