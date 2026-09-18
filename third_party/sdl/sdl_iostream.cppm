module;
#include<SDL3/SDL_iostream.h>

export module sdl:iostream;

import std;
import :common;
import :internal;

namespace sdl
{
    export class iostream
    {
        explicit iostream(SDL_IOStream* stream)
            : imp { stream, SDL_CloseIO }
        {}

        [[nodiscard]] const auto* native_handle() const noexcept { return imp.get(); }
        [[nodiscard]] auto* native_handle() noexcept { return imp.get(); }

        friend internal::wrapper;
        friend internal::native;
        std::unique_ptr<SDL_IOStream, decltype(&SDL_CloseIO)> imp;
    };

    export template<typename T>
    [[nodiscard]] result<iostream> io_from_memory(T* memory, const size_t size)
    {
        if (SDL_IOStream* stream = SDL_IOFromMem(static_cast<void*>(memory), size * sizeof(T)); stream != nullptr)
        {
            return internal::wrapper::create<iostream>(stream);
        }

        return std::unexpected { SDL_GetError() };
    }

    export template<typename T>
    [[nodiscard]] result<iostream> io_from_memory(const T* memory, const size_t size)
    {
        if (SDL_IOStream* stream = SDL_IOFromConstMem(static_cast<const void*>(memory), size * sizeof(T)); stream != nullptr)
        {
            return internal::wrapper::create<iostream>(stream);
        }

        return std::unexpected { SDL_GetError() };
    }

    export enum class open_file_mode : std::uint8_t
    {
        read,
        overwrite,
        write,
        append,
        read_write,
        read_overwrite,
        read_append
    };

    // TODO: implement properly
    export [[nodiscard]] result<iostream> io_from_file(
        const std::filesystem::path& path,
        const open_file_mode open_mode,
        const bool binary)
    {
        if (SDL_IOStream* stream = SDL_IOFromFile(path.string().c_str(), "rb"); stream != nullptr)
        {
            return internal::wrapper::create<iostream>(stream);
        }

        return std::unexpected { SDL_GetError() };
    }

}
