#pragma once
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <iostream>
#include <QEvent>
struct AbstractFeature
{
    QPointF pos;
    float scale;
    float orient_x, orient_y;
};

class ImageView : public QGraphicsView
{
    Q_OBJECT

private:
    QGraphicsScene scene;
    QGraphicsPixmapItem* image_item;
    std::vector<AbstractFeature> features;
    const int id;
protected:
    void wheelEvent(QWheelEvent *) override;
signals:
    void userMoved();
public slots:
    void zoomIn();
    void zoomOut();
    void invalidateAll();
public:
    ImageView(QWidget* parent = nullptr, int id = -1) : id(id) {
        this->setScene(&scene);
        this->setDragMode(ScrollHandDrag);
        this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        //this->setCacheMode(CacheNone);
        //this->setViewportUpdateMode(FullViewportUpdate);
    }

    void LoadImage(const QString& file);

    void LoadFeatures(const std::vector<AbstractFeature>& features);

    void fitViewAllObject();

    bool GetFeaturePos(int id, QPointF& pos) {
        if (id < 0 || id > features.size()) {
            return false;
        }
        pos = features[id].pos;
        return true;
    }

    QPointF MapImagePointToWidget(QPointF pos) const {
        QPoint p;
        QPointF pf = image_item->mapToScene(pos);
        p.setX(pf.x());
        p.setY(pf.y());
        return mapFromScene(p);
    }

    bool event(QEvent* e) override {
        const auto type = e->type();
        switch (type) {
        case QEvent::None:                                          std::cout << "QEvent::None:                               " << std::endl;  break;
        case QEvent::Timer:                                         std::cout << "QEvent::Timer:                              " << std::endl;  break;
        case QEvent::MouseButtonPress:                              std::cout << "QEvent::MouseButtonPress:                   " << std::endl;  break;
        case QEvent::MouseButtonRelease:                            std::cout << "QEvent::MouseButtonRelease:                 " << std::endl;  break;
        case QEvent::MouseButtonDblClick:                           std::cout << "QEvent::MouseButtonDblClick:                " << std::endl;  break;
        case QEvent::MouseMove:                                     std::cout << "QEvent::MouseMove:                          " << std::endl;  break;
        case QEvent::KeyPress:                                      std::cout << "QEvent::KeyPress:                           " << std::endl;  break;
        case QEvent::KeyRelease:                                    std::cout << "QEvent::KeyRelease:                         " << std::endl;  break;
        case QEvent::FocusIn:                                       std::cout << "QEvent::FocusIn:                            " << std::endl;  break;
        case QEvent::FocusOut:                                      std::cout << "QEvent::FocusOut:                           " << std::endl;  break;
        case QEvent::FocusAboutToChange:                            std::cout << "QEvent::FocusAboutToChange:                 " << std::endl;  break;
        case QEvent::Enter:                                         std::cout << "QEvent::Enter:                              " << std::endl;  break;
        case QEvent::Leave:                                         std::cout << "QEvent::Leave:                              " << std::endl;  break;
        case QEvent::Paint:                                         std::cout << "QEvent::Paint:                              " << std::endl;  break;
        case QEvent::Move:                                          std::cout << "QEvent::Move:                               " << std::endl;  break;
        case QEvent::Resize:                                        std::cout << "QEvent::Resize:                             " << std::endl;  break;
        case QEvent::Create:                                        std::cout << "QEvent::Create:                             " << std::endl;  break;
        case QEvent::Destroy:                                       std::cout << "QEvent::Destroy:                            " << std::endl;  break;
        case QEvent::Show:                                          std::cout << "QEvent::Show:                               " << std::endl;  break;
        case QEvent::Hide:                                          std::cout << "QEvent::Hide:                               " << std::endl;  break;
        case QEvent::Close:                                         std::cout << "QEvent::Close:                              " << std::endl;  break;
        case QEvent::Quit:                                          std::cout << "QEvent::Quit:                               " << std::endl;  break;
        case QEvent::ParentChange:                                  std::cout << "QEvent::ParentChange:                       " << std::endl;  break;
        case QEvent::ParentAboutToChange:                           std::cout << "QEvent::ParentAboutToChange:                " << std::endl;  break;
        case QEvent::ThreadChange:                                  std::cout << "QEvent::ThreadChange:                       " << std::endl;  break;
        case QEvent::WindowActivate:                                std::cout << "QEvent::WindowActivate:                     " << std::endl;  break;
        case QEvent::WindowDeactivate:                              std::cout << "QEvent::WindowDeactivate:                   " << std::endl;  break;
        case QEvent::ShowToParent:                                  std::cout << "QEvent::ShowToParent:                       " << std::endl;  break;
        case QEvent::HideToParent:                                  std::cout << "QEvent::HideToParent:                       " << std::endl;  break;
        case QEvent::Wheel:                                         std::cout << "QEvent::Wheel:                              " << std::endl;  break;
        case QEvent::WindowTitleChange:                             std::cout << "QEvent::WindowTitleChange:                  " << std::endl;  break;
        case QEvent::WindowIconChange:                              std::cout << "QEvent::WindowIconChange:                   " << std::endl;  break;
        case QEvent::ApplicationWindowIconChange:                   std::cout << "QEvent::ApplicationWindowIconChange:        " << std::endl;  break;
        case QEvent::ApplicationFontChange:                         std::cout << "QEvent::ApplicationFontChange:              " << std::endl;  break;
        case QEvent::ApplicationLayoutDirectionChange:              std::cout << "QEvent::ApplicationLayoutDirectionChange:   " << std::endl;  break;
        case QEvent::ApplicationPaletteChange:                      std::cout << "QEvent::ApplicationPaletteChange:           " << std::endl;  break;
        case QEvent::PaletteChange:                                 std::cout << "QEvent::PaletteChange:                      " << std::endl;  break;
        case QEvent::Clipboard:                                     std::cout << "QEvent::Clipboard:                          " << std::endl;  break;
        case QEvent::Speech:                                        std::cout << "QEvent::Speech:                             " << std::endl;  break;
        case QEvent::MetaCall:                                      std::cout << "QEvent::MetaCall:                           " << std::endl;  break;
        case QEvent::SockAct:                                       std::cout << "QEvent::SockAct:                            " << std::endl;  break;
        case QEvent::WinEventAct:                                   std::cout << "QEvent::WinEventAct:                        " << std::endl;  break;
        case QEvent::DeferredDelete:                                std::cout << "QEvent::DeferredDelete:                     " << std::endl;  break;
        case QEvent::DragEnter:                                     std::cout << "QEvent::DragEnter:                          " << std::endl;  break;
        case QEvent::DragMove:                                      std::cout << "QEvent::DragMove:                           " << std::endl;  break;
        case QEvent::DragLeave:                                     std::cout << "QEvent::DragLeave:                          " << std::endl;  break;
        case QEvent::Drop:                                          std::cout << "QEvent::Drop:                               " << std::endl;  break;
        case QEvent::DragResponse:                                  std::cout << "QEvent::DragResponse:                       " << std::endl;  break;
        case QEvent::ChildAdded:                                    std::cout << "QEvent::ChildAdded:                         " << std::endl;  break;
        case QEvent::ChildPolished:                                 std::cout << "QEvent::ChildPolished:                      " << std::endl;  break;
        case QEvent::ChildRemoved:                                  std::cout << "QEvent::ChildRemoved:                       " << std::endl;  break;
        case QEvent::ShowWindowRequest:                             std::cout << "QEvent::ShowWindowRequest:                  " << std::endl;  break;
        case QEvent::PolishRequest:                                 std::cout << "QEvent::PolishRequest:                      " << std::endl;  break;
        case QEvent::Polish:                                        std::cout << "QEvent::Polish:                             " << std::endl;  break;
        case QEvent::LayoutRequest:                                 std::cout << "QEvent::LayoutRequest:                      " << std::endl;  break;
        case QEvent::UpdateRequest:                                 std::cout << "QEvent::UpdateRequest:                      " << std::endl;  break;
        case QEvent::UpdateLater:                                   std::cout << "QEvent::UpdateLater:                        " << std::endl;  break;
        case QEvent::EmbeddingControl:                              std::cout << "QEvent::EmbeddingControl:                   " << std::endl;  break;
        case QEvent::ActivateControl:                               std::cout << "QEvent::ActivateControl:                    " << std::endl;  break;
        case QEvent::DeactivateControl:                             std::cout << "QEvent::DeactivateControl:                  " << std::endl;  break;
        case QEvent::ContextMenu:                                   std::cout << "QEvent::ContextMenu:                        " << std::endl;  break;
        case QEvent::InputMethod:                                   std::cout << "QEvent::InputMethod:                        " << std::endl;  break;
        case QEvent::TabletMove:                                    std::cout << "QEvent::TabletMove:                         " << std::endl;  break;
        case QEvent::LocaleChange:                                  std::cout << "QEvent::LocaleChange:                       " << std::endl;  break;
        case QEvent::LanguageChange:                                std::cout << "QEvent::LanguageChange:                     " << std::endl;  break;
        case QEvent::LayoutDirectionChange:                         std::cout << "QEvent::LayoutDirectionChange:              " << std::endl;  break;
        case QEvent::Style:                                         std::cout << "QEvent::Style:                              " << std::endl;  break;
        case QEvent::TabletPress:                                   std::cout << "QEvent::TabletPress:                        " << std::endl;  break;
        case QEvent::TabletRelease:                                 std::cout << "QEvent::TabletRelease:                      " << std::endl;  break;
        case QEvent::OkRequest:                                     std::cout << "QEvent::OkRequest:                          " << std::endl;  break;
        case QEvent::HelpRequest:                                   std::cout << "QEvent::HelpRequest:                        " << std::endl;  break;
        case QEvent::IconDrag:                                      std::cout << "QEvent::IconDrag:                           " << std::endl;  break;
        case QEvent::FontChange:                                    std::cout << "QEvent::FontChange:                         " << std::endl;  break;
        case QEvent::EnabledChange:                                 std::cout << "QEvent::EnabledChange:                      " << std::endl;  break;
        case QEvent::ActivationChange:                              std::cout << "QEvent::ActivationChange:                   " << std::endl;  break;
        case QEvent::StyleChange:                                   std::cout << "QEvent::StyleChange:                        " << std::endl;  break;
        case QEvent::IconTextChange:                                std::cout << "QEvent::IconTextChange:                     " << std::endl;  break;
        case QEvent::ModifiedChange:                                std::cout << "QEvent::ModifiedChange:                     " << std::endl;  break;
        case QEvent::MouseTrackingChange:                           std::cout << "QEvent::MouseTrackingChange:                " << std::endl;  break;
        case QEvent::WindowBlocked:                                 std::cout << "QEvent::WindowBlocked:                      " << std::endl;  break;
        case QEvent::WindowUnblocked:                               std::cout << "QEvent::WindowUnblocked:                    " << std::endl;  break;
        case QEvent::WindowStateChange:                             std::cout << "QEvent::WindowStateChange:                  " << std::endl;  break;
        case QEvent::ReadOnlyChange:                                std::cout << "QEvent::ReadOnlyChange:                     " << std::endl;  break;
        case QEvent::ToolTip:                                       std::cout << "QEvent::ToolTip:                            " << std::endl;  break;
        case QEvent::WhatsThis:                                     std::cout << "QEvent::WhatsThis:                          " << std::endl;  break;
        case QEvent::StatusTip:                                     std::cout << "QEvent::StatusTip:                          " << std::endl;  break;
        case QEvent::ActionChanged:                                 std::cout << "QEvent::ActionChanged:                      " << std::endl;  break;
        case QEvent::ActionAdded:                                   std::cout << "QEvent::ActionAdded:                        " << std::endl;  break;
        case QEvent::ActionRemoved:                                 std::cout << "QEvent::ActionRemoved:                      " << std::endl;  break;
        case QEvent::FileOpen:                                      std::cout << "QEvent::FileOpen:                           " << std::endl;  break;
        case QEvent::Shortcut:                                      std::cout << "QEvent::Shortcut:                           " << std::endl;  break;
        case QEvent::ShortcutOverride:                              std::cout << "QEvent::ShortcutOverride:                   " << std::endl;  break;
        case QEvent::WhatsThisClicked:                              std::cout << "QEvent::WhatsThisClicked:                   " << std::endl;  break;
        case QEvent::ToolBarChange:                                 std::cout << "QEvent::ToolBarChange:                      " << std::endl;  break;
        case QEvent::ApplicationActivate:                           std::cout << "QEvent::ApplicationActivate:                " << std::endl;  break;
        case QEvent::ApplicationDeactivate:                         std::cout << "QEvent::ApplicationDeactivate:              " << std::endl;  break;
        case QEvent::QueryWhatsThis:                                std::cout << "QEvent::QueryWhatsThis:                     " << std::endl;  break;
        case QEvent::EnterWhatsThisMode:                            std::cout << "QEvent::EnterWhatsThisMode:                 " << std::endl;  break;
        case QEvent::LeaveWhatsThisMode:                            std::cout << "QEvent::LeaveWhatsThisMode:                 " << std::endl;  break;
        case QEvent::ZOrderChange:                                  std::cout << "QEvent::ZOrderChange:                       " << std::endl;  break;
        case QEvent::HoverEnter:                                    std::cout << "QEvent::HoverEnter:                         " << std::endl;  break;
        case QEvent::HoverLeave:                                    std::cout << "QEvent::HoverLeave:                         " << std::endl;  break;
        case QEvent::HoverMove:                                     std::cout << "QEvent::HoverMove:                          " << std::endl;  break;
        case QEvent::AcceptDropsChange:                             std::cout << "QEvent::AcceptDropsChange:                  " << std::endl;  break;
        case QEvent::ZeroTimerEvent:                                std::cout << "QEvent::ZeroTimerEvent:                     " << std::endl;  break;
        case QEvent::GraphicsSceneMouseMove:                        std::cout << "QEvent::GraphicsSceneMouseMove:             " << std::endl;  break;
        case QEvent::GraphicsSceneMousePress:                       std::cout << "QEvent::GraphicsSceneMousePress:            " << std::endl;  break;
        case QEvent::GraphicsSceneMouseRelease:                     std::cout << "QEvent::GraphicsSceneMouseRelease:          " << std::endl;  break;
        case QEvent::GraphicsSceneMouseDoubleClick:                 std::cout << "QEvent::GraphicsSceneMouseDoubleClick:      " << std::endl;  break;
        case QEvent::GraphicsSceneContextMenu:                      std::cout << "QEvent::GraphicsSceneContextMenu:           " << std::endl;  break;
        case QEvent::GraphicsSceneHoverEnter:                       std::cout << "QEvent::GraphicsSceneHoverEnter:            " << std::endl;  break;
        case QEvent::GraphicsSceneHoverMove:                        std::cout << "QEvent::GraphicsSceneHoverMove:             " << std::endl;  break;
        case QEvent::GraphicsSceneHoverLeave:                       std::cout << "QEvent::GraphicsSceneHoverLeave:            " << std::endl;  break;
        case QEvent::GraphicsSceneHelp:                             std::cout << "QEvent::GraphicsSceneHelp:                  " << std::endl;  break;
        case QEvent::GraphicsSceneDragEnter:                        std::cout << "QEvent::GraphicsSceneDragEnter:             " << std::endl;  break;
        case QEvent::GraphicsSceneDragMove:                         std::cout << "QEvent::GraphicsSceneDragMove:              " << std::endl;  break;
        case QEvent::GraphicsSceneDragLeave:                        std::cout << "QEvent::GraphicsSceneDragLeave:             " << std::endl;  break;
        case QEvent::GraphicsSceneDrop:                             std::cout << "QEvent::GraphicsSceneDrop:                  " << std::endl;  break;
        case QEvent::GraphicsSceneWheel:                            std::cout << "QEvent::GraphicsSceneWheel:                 " << std::endl;  break;
        case QEvent::KeyboardLayoutChange:                          std::cout << "QEvent::KeyboardLayoutChange:               " << std::endl;  break;
        case QEvent::DynamicPropertyChange:                         std::cout << "QEvent::DynamicPropertyChange:              " << std::endl;  break;
        case QEvent::TabletEnterProximity:                          std::cout << "QEvent::TabletEnterProximity:               " << std::endl;  break;
        case QEvent::TabletLeaveProximity:                          std::cout << "QEvent::TabletLeaveProximity:               " << std::endl;  break;
        case QEvent::NonClientAreaMouseMove:                        std::cout << "QEvent::NonClientAreaMouseMove:             " << std::endl;  break;
        case QEvent::NonClientAreaMouseButtonPress:                 std::cout << "QEvent::NonClientAreaMouseButtonPress:      " << std::endl;  break;
        case QEvent::NonClientAreaMouseButtonRelease:               std::cout << "QEvent::NonClientAreaMouseButtonRelease:    " << std::endl;  break;
        case QEvent::NonClientAreaMouseButtonDblClick:              std::cout << "QEvent::NonClientAreaMouseButtonDblClick:   " << std::endl;  break;
        case QEvent::MacSizeChange:                                 std::cout << "QEvent::MacSizeChange:                      " << std::endl;  break;
        case QEvent::ContentsRectChange:                            std::cout << "QEvent::ContentsRectChange:                 " << std::endl;  break;
        case QEvent::MacGLWindowChange:                             std::cout << "QEvent::MacGLWindowChange:                  " << std::endl;  break;
        case QEvent::FutureCallOut:                                 std::cout << "QEvent::FutureCallOut:                      " << std::endl;  break;
        case QEvent::GraphicsSceneResize:                           std::cout << "QEvent::GraphicsSceneResize:                " << std::endl;  break;
        case QEvent::GraphicsSceneMove:                             std::cout << "QEvent::GraphicsSceneMove:                  " << std::endl;  break;
        case QEvent::CursorChange:                                  std::cout << "QEvent::CursorChange:                       " << std::endl;  break;
        case QEvent::ToolTipChange:                                 std::cout << "QEvent::ToolTipChange:                      " << std::endl;  break;
        case QEvent::NetworkReplyUpdated:                           std::cout << "QEvent::NetworkReplyUpdated:                " << std::endl;  break;
        case QEvent::GrabMouse:                                     std::cout << "QEvent::GrabMouse:                          " << std::endl;  break;
        case QEvent::UngrabMouse:                                   std::cout << "QEvent::UngrabMouse:                        " << std::endl;  break;
        case QEvent::GrabKeyboard:                                  std::cout << "QEvent::GrabKeyboard:                       " << std::endl;  break;
        case QEvent::UngrabKeyboard:                                std::cout << "QEvent::UngrabKeyboard:                     " << std::endl;  break;
        case QEvent::MacGLClearDrawable:                            std::cout << "QEvent::MacGLClearDrawable:                 " << std::endl;  break;
        case QEvent::StateMachineSignal:                            std::cout << "QEvent::StateMachineSignal:                 " << std::endl;  break;
        case QEvent::StateMachineWrapped:                           std::cout << "QEvent::StateMachineWrapped:                " << std::endl;  break;
        case QEvent::TouchBegin:                                    std::cout << "QEvent::TouchBegin:                         " << std::endl;  break;
        case QEvent::TouchUpdate:                                   std::cout << "QEvent::TouchUpdate:                        " << std::endl;  break;
        case QEvent::TouchEnd:                                      std::cout << "QEvent::TouchEnd:                           " << std::endl;  break;
        case QEvent::NativeGesture:                                 std::cout << "QEvent::NativeGesture:                      " << std::endl;  break;
        case QEvent::RequestSoftwareInputPanel:                     std::cout << "QEvent::RequestSoftwareInputPanel:          " << std::endl;  break;
        case QEvent::CloseSoftwareInputPanel:                       std::cout << "QEvent::CloseSoftwareInputPanel:            " << std::endl;  break;
        case QEvent::WinIdChange:                                   std::cout << "QEvent::WinIdChange:                        " << std::endl;  break;
        case QEvent::Gesture:                                       std::cout << "QEvent::Gesture:                            " << std::endl;  break;
        case QEvent::GestureOverride:                               std::cout << "QEvent::GestureOverride:                    " << std::endl;  break;
        case QEvent::ScrollPrepare:                                 std::cout << "QEvent::ScrollPrepare:                      " << std::endl;  break;
        case QEvent::Scroll:                                        std::cout << "QEvent::Scroll:                             " << std::endl;  break;
        case QEvent::Expose:                                        std::cout << "QEvent::Expose:                             " << std::endl;  break;
        case QEvent::InputMethodQuery:                              std::cout << "QEvent::InputMethodQuery:                   " << std::endl;  break;
        case QEvent::OrientationChange:                             std::cout << "QEvent::OrientationChange:                  " << std::endl;  break;
        case QEvent::TouchCancel:                                   std::cout << "QEvent::TouchCancel:                        " << std::endl;  break;
        case QEvent::ThemeChange:                                   std::cout << "QEvent::ThemeChange:                        " << std::endl;  break;
        case QEvent::SockClose:                                     std::cout << "QEvent::SockClose:                          " << std::endl;  break;
        case QEvent::PlatformPanel:                                 std::cout << "QEvent::PlatformPanel:                      " << std::endl;  break;
        case QEvent::StyleAnimationUpdate:                          std::cout << "QEvent::StyleAnimationUpdate:               " << std::endl;  break;
        case QEvent::ApplicationStateChange:                        std::cout << "QEvent::ApplicationStateChange:             " << std::endl;  break;
        case QEvent::WindowChangeInternal:                          std::cout << "QEvent::WindowChangeInternal:               " << std::endl;  break;
        case QEvent::ScreenChangeInternal:                          std::cout << "QEvent::ScreenChangeInternal:               " << std::endl;  break;
        case QEvent::PlatformSurface:                               std::cout << "QEvent::PlatformSurface:                    " << std::endl;  break;
        case QEvent::Pointer:                                       std::cout << "QEvent::Pointer:                            " << std::endl;  break;
        case QEvent::TabletTrackingChange:                          std::cout << "QEvent::TabletTrackingChange:               " << std::endl;  break;
        case QEvent::User:                                          std::cout << "QEvent::User:                               " << std::endl;  break;
        case QEvent::MaxUser:                                       std::cout << "QEvent::MaxUser:                            " << std::endl;  break;
        default:
            std::cout << "unknown" << std::endl;
        }
        return this->QGraphicsView::event(e);
    }

    void paintEvent(QPaintEvent* e) override {
        //std::cout << "view " << id << " repaint" << std::endl;
        this->QGraphicsView::paintEvent(e);
    }

    void scrollContentsBy(int x, int y) override {
        emit userMoved();
        this->QGraphicsView::scrollContentsBy(x,y);
    }
};