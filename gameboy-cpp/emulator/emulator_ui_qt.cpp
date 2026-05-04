
#include "emulator_ui_qt.hpp"

namespace emulator
{
    static QVariantMap toQtVariantMap(const std::expected<void, std::string>& result)
    {
        return result.has_value()
            ? QVariantMap{ { "success", true } }
            : QVariantMap{ {"success", false }, { "error", result.error().data() }};
    }

    QVariantMap emulator_ui_backend::load_rom(const QUrl& url) const
    {
        return toQtVariantMap(load_rom_fn(url.toLocalFile().toStdString()));
    }

    bool emulator_ui_backend::has_rom() const { return has_rom_fn(); }
    bool emulator_ui_backend::is_running() const { return is_running_fn(); }
    void emulator_ui_backend::resume() const { resume_fn(); }
    void emulator_ui_backend::pause() const { pause_fn(); }
    void emulator_ui_backend::stop() const { stop_fn(); }

    void emulator_ui_backend_singleton::set_backend(emulator_ui_backend* backend)
    {
        backend_instance = backend;
    }

    emulator_ui_backend* emulator_ui_backend_singleton::create(QQmlEngine*, QJSEngine* engine)
    {
        js_engine = js_engine == nullptr ? engine : js_engine;

        Q_ASSERT(engine == js_engine);
        Q_ASSERT(backend_instance);
        Q_ASSERT(js_engine->thread() == backend_instance->thread());

        QJSEngine::setObjectOwnership(backend_instance, QJSEngine::CppOwnership);
        return backend_instance;
    }

    void framebuffer_ui::set_backend(QObject* b)
    {
        backend = static_cast<emulator_ui_backend*>(b);
        Q_ASSERT(backend != nullptr);

        setFlag(ItemHasContents);
        connect(window(),&QQuickWindow::frameSwapped,this, [this]{ update(); });
    }

    QSGNode* framebuffer_ui::updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* updatePaintNodeData)
    {
        Q_ASSERT(backend != nullptr);
        Q_ASSERT(window() != nullptr);

        if (!backend->has_rom() || !backend->is_running()) { return oldNode; }

        auto node = static_cast<QSGSimpleTextureNode*>(oldNode);
        if (node == nullptr ) { node = new QSGSimpleTextureNode(); }

        const auto span = backend->framebuffer();
        const QImage img(span.data(), ui_framebuffer_width, ui_framebuffer_height, QImage::Format_RGB888);

        texture.reset(window()->createTextureFromImage(img, QQuickWindow::TextureIsOpaque));
        texture->setFiltering(QSGTexture::Nearest);

        node->setTexture(texture.get());
        node->setRect(boundingRect());

        return node;
    }

}