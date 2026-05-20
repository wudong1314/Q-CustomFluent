#pragma once

#include <QMenuBar>
#include <QStatusBar>
#include <QWidget>

#include <memory>
#include <QMouseEvent>

namespace Ui
{
    class QCusFluentWindow;
}
class QCusFluentNavigationBar;
class QCusFluentTitleBar;

class QCusFluentWindow : public QWidget
{
    Q_OBJECT

public:
    QCusFluentWindow(QWidget* parent = nullptr);
    ~QCusFluentWindow();

    void setStatusBar(QStatusBar* statusBar);
    QStatusBar* statusBar() const;
    void setMenuBar(QMenuBar* menuBar);
    QMenuBar* menuBar() const;
    void setNavigationBar(QCusFluentNavigationBar* menuBar);
    QCusFluentNavigationBar* navigationBar() const;
    void setTitleBar(QCusFluentTitleBar* titleBar);
    QCusFluentTitleBar* titleBar() const;
    void setCentralWidget(QWidget* widget);
    QWidget* centralWidget() const;
    void addMoveBar(QWidget* moveBar);
    void removeMoveBar(QWidget* moveBar);

#ifndef Q_OS_WIN
    void setShadowless(bool flag);
    bool shadowless() const;
#endif
protected:
    bool eventFilter(QObject* watched, QEvent* event) override;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    bool nativeEvent(const QByteArray& eventType, void* message, long* result) override;
#else
    bool nativeEvent(const QByteArray& eventType, void* message, qintptr* result) override;
#endif
private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
    Ui::QCusFluentWindow* ui;
};
