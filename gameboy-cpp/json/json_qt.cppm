module;
#include <QJsonObject>
#include <QJsonArray>

module json:implementation;

import std;

import :types;
import :common;

namespace json
{
    struct internal
    {
        static [[nodiscard]] value to_json_value(const QJsonValue& value);
        static [[nodiscard]] document to_json_document(const QJsonDocument& value);

        static [[nodiscard]] QJsonValue to_qt_value(const value& value);
        static [[nodiscard]] QJsonDocument to_qt_document(const document& value);
    };

    // array
    struct array::imp { QJsonArray data; };

    array::array() : impl { std::make_unique<imp>() } {}
    array::~array() = default;

    array::array(const array& other) : impl { std::make_unique<imp>(other.impl->data) } {}
    array& array::operator=(const array& other)
    {
        impl->data = other.impl->data;
        return *this;
    }

    array::array(array&& other) noexcept : impl { std::exchange(other.impl, nullptr) } {}
    array& array::operator=(array&& other) noexcept
    {
        impl = std::exchange(other.impl, nullptr);
        return *this;
    }

    size_t array::size() const { return impl->data.size(); }
    value array::get(const size_t idx) const { return internal::to_json_value(impl->data[idx]); }
    void array::set(size_t idx, const value& value) { impl->data[idx] = internal::to_qt_value(value); }
    void array::push_back(const value& value) { impl->data.append(internal::to_qt_value(value)); }

    // object
    struct object::imp { QJsonObject data; };

    object::object() : impl { std::make_unique<imp>() } {}
    object::~object() = default;

    object::object(const object& other) : impl { std::make_unique<imp>(other.impl->data) } {}
    object& object::operator=(const object& other)
    {
        impl->data = other.impl->data;
        return *this;
    }

    object::object(object&& other) noexcept : impl { std::exchange(other.impl, nullptr) } {}
    object& object::operator=(object&& other) noexcept
    {
        impl = std::exchange(other.impl, nullptr);
        return *this;
    }

    size_t object::size() const { return impl->data.size(); }
    value object::get(const std::string_view key) const
    {
        const QString qt_key = QString::fromStdString(std::string(key));
        return internal::to_json_value(impl->data[qt_key]);
    }

    void object::set(const std::string_view key, const value& value)
    {
        const QString qt_key = QString::fromStdString(std::string(key));
        impl->data[qt_key] = internal::to_qt_value(value);
    }

    // Serialization
    [[nodiscard]] result<document> deserialize(const std::span<const std::uint8_t> data)
    {
        QJsonParseError qt_error {};
        const QJsonDocument qt_document = QJsonDocument::fromJson(
            QByteArray::fromRawData(reinterpret_cast<const char*>(data.data()), data.size()),
            &qt_error);

        return qt_document.isNull()
            ? std::unexpected { qt_error.errorString().toStdString() }
            : result<document> { internal::to_json_document(qt_document) };
    }

    [[nodiscard]] result<std::vector<std::uint8_t>> serialize(const document& doc)
    {
        const QJsonDocument qt_doc = internal::to_qt_document(doc);
        const QByteArray serialized = qt_doc.toJson();

        return std::vector<std::uint8_t> { serialized.begin(), serialized.end() };
    }

    // Helpers
    [[nodiscard]] value internal::to_json_value(const QJsonValue& value)
    {
        switch (value.type())
        {
        case QJsonValue::Null:
            return null {};
        case QJsonValue::Bool:
            return value.toBool();
        case QJsonValue::Double:
            return value.toDouble();
        case QJsonValue::String:
            return value.toString().toStdString();
        case QJsonValue::Array:
            {
                array jarray {};
                jarray.impl->data = value.toArray();
                return jarray;
            }
        case QJsonValue::Object:
            {
                object jobject {};
                jobject.impl->data = value.toObject();
                return jobject;
            }
        default:
            std::unreachable();
        }
    }

    [[nodiscard]] document internal::to_json_document(const QJsonDocument& qt_document)
    {
        if (qt_document.isObject())
        {
            object jobject {};
            jobject.impl->data = qt_document.object();

            return jobject;
        }

        array jarray {};
        jarray.impl->data = qt_document.array();

        return jarray;
    }

    [[nodiscard]] QJsonValue internal::to_qt_value(const value& value)
    {
        return std::visit([] (auto&& input)
        {
            using T = std::decay_t<decltype(input)>;

            if constexpr (std::is_same_v<T, object> || std::is_same_v<T, array>)
            {
                return QJsonValue(input.impl->data);
            }
            else if constexpr (std::is_same_v<T, null>)
            {
                return QJsonValue();
            }
            else if constexpr (std::is_same_v<T, string>)
            {
                return QJsonValue(QString::fromStdString(input));
            }
            else
            {
                return QJsonValue(input);
            }
        }, value);
    }

    [[nodiscard]] QJsonDocument internal::to_qt_document(const document& value)
    {
        return std::visit([] (auto&& input) { return QJsonDocument(input.impl->data); }, value);
    }

}