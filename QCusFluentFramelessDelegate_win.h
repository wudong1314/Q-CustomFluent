#pragma once

#include <QWidget>

#include <functional>
#include <memory>

#ifdef Q_OS_WIN

using MSG = struct tagMSG;
class QCusFluentFramelessDelegate_win : public QObject
{
    Q_OBJECT

public:
    QCusFluentFramelessDelegate_win(QWidget* parent, const QWidgetList& moveBars);
    QCusFluentFramelessDelegate_win(QWidget* parent, QWidget* title);
    ~QCusFluentFramelessDelegate_win();

    void addMoveBar(QWidget* w);
    void removeMoveBar(QWidget* w);

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    bool nativeEvent(const QByteArray& eventType, void* message, long* result);
#else
    bool nativeEvent(const QByteArray& eventType, void* message, qintptr* result);
#endif
protected:
    bool eventFilter(QObject* watched, QEvent* event) override;

private:
    void setWindowLong();
    void showSystemMenu(const QPoint& pos);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    bool onNCTitTest(MSG* msg, long* result);
#else
    bool onNCTitTest(MSG* msg, qintptr* result);
#endif
private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
    friend class WindowsEventFilter;
};
#endif
