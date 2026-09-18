module;
#include <lunasvg.h>

export module svg:document;

import std;

import :common;
import :bitmap;
import :internal;

namespace svg
{
    using upstream_document = lunasvg::Document;
    using upstream_document_ptr = std::unique_ptr<upstream_document>;

    export class document
    {
    public:
        [[nodiscard]] static result<document> load(const std::filesystem::path& path)
        {
            if(auto imp = upstream_document::loadFromFile(path.string()); imp != nullptr)
            {
                return document { std::move(imp) };
            }

            return std::unexpected { std::format("Could not load svg document from path {}", path.string()) };
        }

        [[nodiscard]] static result<document> load(const char* data, const size_t size)
        {
            if(auto imp = upstream_document::loadFromData(data, size); imp != nullptr)
            {
                return document { std::move(imp) };
            }

            return std::unexpected { "Could not load svg document from user-provided data" };
        }

        [[nodiscard]] result<bitmap> to_bitmap() const
        {
            if(auto bitmap_imp = imp->renderToBitmap(); !bitmap_imp.isNull())
            {
                return internal::create<bitmap>(std::move(bitmap_imp));
            }

            return std::unexpected { "Could not render svg document as bitmap" };
        }

    private:
        explicit document(upstream_document_ptr&& imp)
            : imp { std::move(imp) }
        {}

        upstream_document_ptr imp;
    };
}