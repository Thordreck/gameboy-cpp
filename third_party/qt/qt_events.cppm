module;
#include <QEvent>

export module qt:events;

import std;

import :core;
import :internal;

namespace qt
{
    export enum class event_type : std::uint16_t
    {
        none = QEvent::None,
        timer = QEvent::Timer,
        mouse_button_press = QEvent::MouseButtonPress,
        mouse_button_release = QEvent::MouseButtonRelease,
        mouse_button_double_click = QEvent::MouseButtonDblClick,
        mouse_move = QEvent::MouseMove,
        key_press = QEvent::KeyPress,
        key_release = QEvent::KeyRelease,
        focus_in = QEvent::FocusIn,
        focus_out = QEvent::FocusOut,
        focus_about_to_change = QEvent::FocusAboutToChange,
        enter = QEvent::Enter,
        leave = QEvent::Leave,
        paint = QEvent::Paint,
        move = QEvent::Move,
        resize = QEvent::Resize,
        create = QEvent::Create,
        destroy = QEvent::Destroy,
        show = QEvent::Show,
        hide = QEvent::Hide,
        close = QEvent::Close,
        quit = QEvent::Quit,
        parent_change = QEvent::ParentChange,
        parent_about_to_change = QEvent::ParentAboutToChange,
        thread_change = QEvent::ThreadChange,
        window_activate = QEvent::WindowActivate,
        window_deactivate = QEvent::WindowDeactivate,
        show_to_parent = QEvent::ShowToParent,
        hide_to_parent = QEvent::HideToParent,
        wheel = QEvent::Wheel,
        window_title_change = QEvent::WindowTitleChange,
        window_icon_change = QEvent::WindowIconChange,
        application_window_icon_change = QEvent::ApplicationWindowIconChange,
        application_font_change = QEvent::ApplicationFontChange,
        application_layout_direction_change = QEvent::ApplicationLayoutDirectionChange,
        application_palette_change = QEvent::ApplicationPaletteChange,
        palette_change = QEvent::PaletteChange,
        clipboard = QEvent::Clipboard,
        speech = QEvent::Speech,
        meta_call = QEvent::MetaCall,
        socket_activation = QEvent::SockAct,
        win_event_activation = QEvent::WinEventAct,
        deferred_delete = QEvent::DeferredDelete,
        drag_enter = QEvent::DragEnter,
        drag_move = QEvent::DragMove,
        drag_leave = QEvent::DragLeave,
        drop = QEvent::Drop,
        drag_response = QEvent::DragResponse,
        child_added = QEvent::ChildAdded,
        child_polished = QEvent::ChildPolished,
        child_removed = QEvent::ChildRemoved,
        show_window_request = QEvent::ShowWindowRequest,
        polish_request = QEvent::PolishRequest,
        polish = QEvent::Polish,
        layout_request = QEvent::LayoutRequest,
        update_request = QEvent::UpdateRequest,
        update_later = QEvent::UpdateLater,
        embedding_control = QEvent::EmbeddingControl,
        activate_control = QEvent::ActivateControl,
        deactivate_control = QEvent::DeactivateControl,
        context_menu = QEvent::ContextMenu,
        input_method = QEvent::InputMethod,
        tablet_move = QEvent::TabletMove,
        locale_change = QEvent::LocaleChange,
        language_change = QEvent::LanguageChange,
        layout_direction_change = QEvent::LayoutDirectionChange,
        style = QEvent::Style,
        tablet_press = QEvent::TabletPress,
        tablet_release = QEvent::TabletRelease,
        ok_request = QEvent::OkRequest,
        help_request = QEvent::HelpRequest,
        icon_drag = QEvent::IconDrag,
        font_change = QEvent::FontChange,
        enabled_change = QEvent::EnabledChange,
        activation_change = QEvent::ActivationChange,
        style_change = QEvent::StyleChange,
        icon_text_change = QEvent::IconTextChange,
        modified_change = QEvent::ModifiedChange,
        mouse_tracking_change = QEvent::MouseTrackingChange,
        window_blocked = QEvent::WindowBlocked,
        window_unblocked = QEvent::WindowUnblocked,
        window_state_change = QEvent::WindowStateChange,
        readonly_change = QEvent::ReadOnlyChange,
        tooltip = QEvent::ToolTip,
        whats_this = QEvent::WhatsThis,
        statustip = QEvent::StatusTip,
        action_changed = QEvent::ActionChanged,
        action_added = QEvent::ActionAdded,
        action_removed = QEvent::ActionRemoved,
        file_open = QEvent::FileOpen,
        shortcut = QEvent::Shortcut,
        shortcut_override = QEvent::ShortcutOverride,
        whats_this_clicked = QEvent::WhatsThisClicked,
        toolbar_change = QEvent::ToolBarChange,
        query_whats_this = QEvent::QueryWhatsThis,
        enter_whats_this_mode = QEvent::EnterWhatsThisMode,
        leave_whats_this_mode = QEvent::LeaveWhatsThisMode,
        z_order_change = QEvent::ZOrderChange,
        hover_enter = QEvent::HoverEnter,
        hover_leave = QEvent::HoverLeave,
        hover_move = QEvent::HoverMove,
#ifdef QT_KEYPAD_NAVIGATION
        enter_edit_focus = QEvent::EnterEditFocus,
        leave_edit_focus = QEvent::LeaveEditFocus,
#endif
        accept_drops_change = QEvent::AcceptDropsChange,
        zero_timer_event = QEvent::ZeroTimerEvent,
        graphics_scene_mouse_move = QEvent::GraphicsSceneMouseMove,
        graphics_scene_mouse_press = QEvent::GraphicsSceneMousePress,
        graphics_scene_mouse_release = QEvent::GraphicsSceneMouseRelease,
        graphics_scene_mouse_double_click = QEvent::GraphicsSceneMouseDoubleClick,
        graphics_scene_context_menu = QEvent::GraphicsSceneContextMenu,
        graphics_scene_hover_enter = QEvent::GraphicsSceneHoverEnter,
        graphics_scene_hover_move = QEvent::GraphicsSceneHoverMove,
        graphics_scene_hover_leave = QEvent::GraphicsSceneHoverLeave,
        graphics_scene_help = QEvent::GraphicsSceneHelp,
        graphics_scene_drag_enter = QEvent::GraphicsSceneDragEnter,
        graphics_scene_drag_move = QEvent::GraphicsSceneDragMove,
        graphics_scene_drag_leave = QEvent::GraphicsSceneDragLeave,
        grahpics_scene_drop = QEvent::GraphicsSceneDrop,
        graphics_scene_wheel = QEvent::GraphicsSceneWheel,
        graphics_scene_leave = QEvent::GraphicsSceneLeave,
        keyboard_layout_change = QEvent::KeyboardLayoutChange,
        dynamic_property_change = QEvent::DynamicPropertyChange,
        tablet_enter_proximity = QEvent::TabletEnterProximity,
        tablet_leave_proximity = QEvent::TabletLeaveProximity,
        non_client_area_mouse_move = QEvent::NonClientAreaMouseMove,
        non_client_area_mouse_button_press = QEvent::NonClientAreaMouseButtonPress,
        non_client_area_mouse_button_release = QEvent::NonClientAreaMouseButtonRelease,
        non_client_area_mouse_button_double_click = QEvent::NonClientAreaMouseButtonDblClick,
        mac_size_change = QEvent::MacSizeChange,
        contents_rects_change = QEvent::ContentsRectChange,
        mac_gl_window_change = QEvent::MacGLWindowChange,
        future_call_out = QEvent::FutureCallOut,
        graphics_scene_resize = QEvent::GraphicsSceneResize,
        graphics_scene_move = QEvent::GraphicsSceneMove,
        cursor_change = QEvent::CursorChange,
        tooltip_change = QEvent::ToolTipChange,
        network_reply_updated = QEvent::NetworkReplyUpdated,
        grab_mouse = QEvent::GrabMouse,
        ungrab_mouse = QEvent::UngrabMouse,
        grab_keyboard = QEvent::GrabKeyboard,
        ungrab_keyboard = QEvent::UngrabKeyboard,
        state_machine_signal = QEvent::StateMachineSignal,
        state_machine_wrapped = QEvent::StateMachineWrapped,
        touch_begin = QEvent::TouchBegin,
        touch_update = QEvent::TouchUpdate,
        touch_end = QEvent::TouchEnd,
#ifndef QT_NO_GESTURES
        native_gesture = QEvent::NativeGesture,
#endif
        request_software_input_panel = QEvent::RequestSoftwareInputPanel,
        close_software_input_panel = QEvent::CloseSoftwareInputPanel,
        win_id_change = QEvent::WinIdChange,
#ifndef QT_NO_GESTURES
        gesture = QEvent::Gesture,
        gesture_override = QEvent::GestureOverride,
#endif
        scroll_prepare = QEvent::ScrollPrepare,
        scroll = QEvent::Scroll,
        expose = QEvent::Expose,
        input_method_query = QEvent::InputMethodQuery,
        orientation_change = QEvent::OrientationChange,
        touch_cancel = QEvent::TouchCancel,
        theme_change = QEvent::ThemeChange,
        socket_close = QEvent::SockClose,
        platform_panel = QEvent::PlatformPanel,
        style_animation_update = QEvent::StyleAnimationUpdate,
        application_state_change = QEvent::ApplicationStateChange,
        window_change_internal = QEvent::WindowChangeInternal,
        screen_change_internal = QEvent::ScreenChangeInternal,
        platform_surface = QEvent::PlatformSurface,
        pointer = QEvent::Pointer,
        tablet_tracking_change = QEvent::TabletTrackingChange,
        window_about_to_change_internal = QEvent::WindowAboutToChangeInternal,
        device_pixel_ratio_change = QEvent::DevicePixelRatioChange,
        child_window_added = QEvent::ChildWindowAdded,
        child_window_removed = QEvent::ChildWindowRemoved,
        parent_window_about_to_change = QEvent::ParentWindowAboutToChange,
        parent_window_change = QEvent::ParentWindowChange,
        safe_area_margins_change = QEvent::SafeAreaMarginsChange,
        user = QEvent::User,
        max_user = QEvent::MaxUser,
    };

    export template<typename T>
    concept Event = requires()
    {
        { T::type } -> std::convertible_to<event_type>;
    };

    template<Event T>
    struct event_factory
    {
        static constexpr T create(const QEvent* event) = delete;
    };

    export template<typename F, typename Arg>
    concept EventFilter = Event<Arg> && requires(F filter, const Arg& event)
    {
        { filter(event) } -> std::convertible_to<bool>;
    };

    class qt_event_filter : public QObject
    {
    public:
        explicit qt_event_filter(const std::function<bool(const QEvent*)>& filter)
            : filter { filter }
        {}

        bool eventFilter(QObject* object, QEvent* event) override
        {
            return filter(event)
                ? true
                : QObject::eventFilter(object, event);
        }

        std::function<bool(const QEvent*)> filter;
    };

    template<Event Event, typename Filter>
    requires EventFilter<Filter, Event>
    bool user_filter_adapter(const QEvent* event, Filter&& filter)
    {
        const auto received_event_type = static_cast<event_type>(event->type());

        return received_event_type == Event::type
            ? filter(event_factory<Event>::create(event))
            : false;
    }

    export class event_filter
    {
    public:
        template<Event Event, typename Filter>
        requires EventFilter<Filter, Event>
        [[nodiscard]] static event_filter create(const object& target, Filter&& filter)
        {
            QObject* qt_target = internal::get_qt_object(target);
            const auto adapted_filter = [filter] (const QEvent* event) { return user_filter_adapter<Event>(event, filter); };

            return { qt_target, adapted_filter };
        }

        ~event_filter()
        {
            qt_target->removeEventFilter(&qt_filter);
        }

        event_filter(const event_filter&) = delete;
        event_filter& operator=(const event_filter&) = delete;

    private:
        event_filter(QObject* target, const std::function<bool(const QEvent*)>& filter)
            : qt_filter { filter }
            , qt_target { target }
        {
            qt_target->installEventFilter(&qt_filter);
        }

        qt_event_filter qt_filter;
        QObject* qt_target;
    };

}