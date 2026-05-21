#pragma once

#include <QMenu>
#include <QWidget>
#include <QMouseEvent>
#include <memory>

namespace Ui
{
    class QCusFluenTitleBar;
    using QCusFluentTitleBar = QCusFluenTitleBar;
}

class QMenuBar;

class QCusFluentTitleBar : public QWidget
{
    Q_OBJECT
        Q_PROPERTY(bool iconIsVisible READ iconIsVisible WRITE setIconVisible)
        Q_PROPERTY(QSize iconSize READ iconSize WRITE setIconSize NOTIFY iconSizeChanged)
public:
    QCusFluentTitleBar(QWidget* parent = Q_NULLPTR);
    ~QCusFluentTitleBar();

    void setMenuBar(QMenuBar* menu);
    QMenuBar* menuBar() const;
    void setIconVisible(bool show);
    bool iconIsVisible() const;
    void setIconSize(const QSize& size);
    const QSize& iconSize() const;
signals:
    void iconSizeChanged(const QSize& size);
    void sigClose();
private slots:
    void onCloseBtn();
    void onMaximumSizeBtn();
    void onMinimumSizeBtn();

protected:
    void paintEvent(QPaintEvent* event) override;
    void showEvent(QShowEvent* event) override;
    bool eventFilter(QObject* watched, QEvent* event) override;
    void actionEvent(QActionEvent* event) override;

private:
    QRect doIconRect() const;

private:
    Ui::QCusFluentTitleBar* ui;

    struct Impl;
    std::unique_ptr<Impl> m_impl;
#ifdef Q_OS_WIN
    friend class QCusFluentFramelessDelegate_win;
#endif
};
