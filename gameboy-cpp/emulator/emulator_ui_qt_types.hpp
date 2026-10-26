#pragma once

#include "emulator_ui_qt.hpp"

#include <concepts>
#include <QtQuick>
#include <QString>
#include <QVariant>
#include <QMetaType>
#include <boost/lockfree/spsc_value.hpp>

namespace emulator
{
    template<typename T>
    concept QMLCompatible
        = std::convertible_to<T, int>
        || std::convertible_to<T, bool>
        || std::convertible_to<T, double>
        || std::convertible_to<T, QString>
        || QMetaType::fromType<T>.isValid();

    // Adapted from: https://gist.github.com/oKcerG/fa03827ebbd1c7aa6471c4ce560946ad
    class q_expected
    {
        Q_GADGET
        QML_ELEMENT
        QML_UNCREATABLE("")

    public:
        // Note: required by QML. It's not possible to make it private and befriend Qt's internal methods
        // since they use std::is_default_constructible
        explicit q_expected() = default;

        q_expected(QMLCompatible auto&& value, QJSEngine* engine)
            : is_error { false }
        , value { QVariant::fromValue(std::forward<decltype(value)>(value)) }
        , engine { engine }
        {}

        template<QMLCompatible Error>
        q_expected(std::unexpected<Error>&& unexpected, QJSEngine* engine)
            : is_error { true }
        , value { QVariant::fromValue(std::forward<decltype(unexpected)>(unexpected).error()) }
        , engine { engine }
        {}

        Q_INVOKABLE void visit(const QJSValue& on_expected, const QJSValue& on_error) const;

    private:
        bool is_error;
        QVariant value;
        QJSEngine* engine;
    };

    struct emulator_ui_status
    {
        Q_GADGET
        QML_VALUE_TYPE(emulatorStatus)

    public:
        enum Value { Stopped, Running, Paused };
        Q_ENUM(Value)
    };

    class emulator_ui_status_enum : public emulator_ui_status
    {
        Q_GADGET
    };

    class emulator_ui_controls : public QObject
    {
        Q_OBJECT
        QML_UNCREATABLE("")
        QML_NAMED_ELEMENT(EmulatorControls)

        Q_PROPERTY(emulator::emulator_ui_status::Value status READ status BINDABLE bindable_status)
        Q_PROPERTY(float volume READ volume WRITE setVolume BINDABLE bindable_volume)
        Q_PROPERTY(bool muted READ muted WRITE setMuted BINDABLE bindable_muted)

    public:
        explicit emulator_ui_controls(const backend_functions& _backend);
        Q_INVOKABLE QVariantMap load_rom(const QUrl& url);
        Q_INVOKABLE emulator_ui_status::Value status() const;
        Q_INVOKABLE void resume();
        Q_INVOKABLE void pause();
        Q_INVOKABLE void stop();
        Q_INVOKABLE void nextFrame() const;
        Q_INVOKABLE float volume() const;
        Q_INVOKABLE void setVolume(const float volume);
        Q_INVOKABLE bool muted() const;
        Q_INVOKABLE void setMuted(const bool muted);

    private:
        QBindable<emulator_ui_status::Value> bindable_status();
        void update_current_status();
        QProperty<emulator_ui_status::Value> current_status;
        QProperty<float> current_volume;
        QBindable<float> bindable_volume();
        QProperty<bool> current_muted;
        QBindable<bool> bindable_muted();

        backend_functions backend;
    };

    class emulator_ui_video_source : public QObject
    {
        Q_OBJECT
        QML_NAMED_ELEMENT(EmulatorVideoSource)
        QML_UNCREATABLE("")

    public:
        virtual QImage frame() = 0;
        virtual QSize size() = 0;

        signals:
            void frameAvailable();
    };

    class emulator_ui_framebuffer_source : public emulator_ui_video_source
    {
        Q_OBJECT
        QML_UNCREATABLE("")
        QML_NAMED_ELEMENT(EmulatorFramebuffer)

        Q_PROPERTY(QImage frame READ frame NOTIFY frameAvailable)
        Q_PROPERTY(QSize size READ size CONSTANT)

    public:
        QSize size() override;
        QImage frame() override;

        void push_frame(const ui_framebuffer_view_t frame);

    private:
        boost::lockfree::spsc_value<
            QImage,
            boost::lockfree::allow_multiple_reads<true>
        > buffer {};
    };

    class emulator_ui_video : public QQuickItem
    {
        Q_OBJECT
        QML_NAMED_ELEMENT(EmulatorVideo)
        Q_PROPERTY(emulator::emulator_ui_video_source* source READ get_source WRITE set_source REQUIRED)

    public:
        emulator_ui_video();
        emulator_ui_video_source* get_source() const;
        void set_source(emulator_ui_video_source* new_source);

    protected:
        QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData*) override;

    private:
        emulator_ui_video_source* source { nullptr };
        std::unique_ptr<QSGTexture> texture { nullptr };
    };

    struct emulator_ui_sprite
    {
        std::uint8_t index;
        std::uint8_t x;
        std::uint8_t y;
        std::uint8_t tile_index;
        bool priority;
        bool x_flip;
        bool y_flip;
        bool alternate_palette;
        std::uint8_t width;
        std::uint8_t height;
    };

    enum class sprites_model_roles : int
    {
        index = Qt::UserRole + 1,
        x,
        y,
        tile_index,
        priority,
        x_flip,
        y_flip,
        image_uri,
        width,
        height
    };

    class emulator_ui_sprites_model : public QAbstractListModel
    {
        Q_OBJECT
        QML_UNCREATABLE("")
        QML_NAMED_ELEMENT(EmulatorSprites)

    public:
        explicit emulator_ui_sprites_model(const backend_functions& backend, QObject* parent = nullptr);

        int rowCount(const QModelIndex&) const override;
        QVariant data(const QModelIndex &model_index, const int role) const override;
        QHash<int, QByteArray> roleNames() const override;

        Q_INVOKABLE void refreshSpritesCache();

    private:
        std::array<emulator_ui_sprite, 40> sprites {};
        std::function<std::uint8_t(std::uint16_t)> read_mem_fn;
    };

    class emulator_ui_sprites_image_provider : public QQuickImageProvider
    {
        Q_OBJECT
        QML_UNCREATABLE("")

    public:
        explicit emulator_ui_sprites_image_provider(const backend_functions& backend, QObject* parent = nullptr);
        QImage requestImage(const QString& id, QSize* size, const QSize&) const;

    private:
        std::function<std::uint8_t(std::uint16_t)> read_mem_fn;
    };

    class emulator_ui_background : public emulator_ui_video_source
    {
        Q_OBJECT
        QML_UNCREATABLE("")
        QML_NAMED_ELEMENT(EmulatorBackground)

        Q_PROPERTY(QImage image READ frame BINDABLE bindable_image NOTIFY frameAvailable)
        Q_PROPERTY(QRect window READ window BINDABLE bindable_window)

    public:
        explicit emulator_ui_background(const backend_functions& backend);

        QImage frame() override;
        QSize size() override;

        QRect window() const;

        Q_INVOKABLE void refresh();

    private:
        QProperty<QImage> current_image;
        QBindable<QImage> bindable_image();

        QProperty<QRect> current_window;
        QBindable<QRect> bindable_window();

        std::function<std::uint8_t(std::uint16_t)> read_mem_fn;
    };

    class emulator_ui_background_image_provider : public QQuickImageProvider
    {
        Q_OBJECT
        QML_UNCREATABLE("")

    public:
        explicit emulator_ui_background_image_provider(emulator_ui_background& bg, QObject* parent = nullptr);

        QImage requestImage(const QString& id, QSize* size, const QSize&) const;

    private:
        emulator_ui_background& background_source;
    };

    class project_metadata_provider : public QObject
    {
        Q_OBJECT
        QML_UNCREATABLE("")
        QML_NAMED_ELEMENT(ProjectMetadata)

        Q_PROPERTY(QString author READ author CONSTANT)
        Q_PROPERTY(QString name READ name CONSTANT)
        Q_PROPERTY(QString version READ version CONSTANT)
        Q_PROPERTY(QString description READ description CONSTANT)
        Q_PROPERTY(QUrl url READ url CONSTANT)
        Q_PROPERTY(QString license READ license CONSTANT)
        Q_PROPERTY(QString hash READ hash CONSTANT)

    public:
        explicit project_metadata_provider(const backend_functions& backend);

        Q_INVOKABLE QString author() const;
        Q_INVOKABLE QString name() const;
        Q_INVOKABLE QString version() const;
        Q_INVOKABLE QString description() const;
        Q_INVOKABLE QUrl url() const;
        Q_INVOKABLE QString license() const;
        Q_INVOKABLE QString hash() const;

    private:
        project_metadata metadata;
    };

    enum class third_party_metadata_roles : int
    {
        name = Qt::UserRole + 1,
        url,
        license,
    };

    class third_party_metadata_provider : public QAbstractListModel
    {
        Q_OBJECT
        QML_UNCREATABLE("")
        QML_NAMED_ELEMENT(ThirdPartyMetadata)

    public:
        explicit third_party_metadata_provider(const backend_functions& backend, QObject* parent = nullptr);

        int rowCount(const QModelIndex&) const override;
        QVariant data(const QModelIndex &model_index, const int role) const override;
        QHash<int, QByteArray> roleNames() const override;

    private:
        std::vector<third_party_metadata> metadata;
    };

    class file_utils : public QObject
    {
        Q_OBJECT
        QML_UNCREATABLE("")
        QML_NAMED_ELEMENT(FileUtils)

    public:
        explicit file_utils(QJSEngine& engine, QObject* parent = nullptr);

        Q_INVOKABLE q_expected read_file(const QString& path) const;

    private:
        QJSEngine& engine;
    };
}

namespace emulator_status_enum_foreign
{
    Q_NAMESPACE
    QML_NAMED_ELEMENT(EmulatorStatus)
    QML_FOREIGN_NAMESPACE(::emulator::emulator_ui_status_enum)
}