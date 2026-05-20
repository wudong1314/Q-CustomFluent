#pragma once

#include "QCusFluentAbstractMessageTipView.h"

#include <memory>

class QCusFluentNavigationBar;
using QCusFluentMessageTipDataPtr = std::shared_ptr<class QCusFluentMessageTipData>;

class  QCusFluentMessageTipView : public QCusFluentAbstractMessageTipView
{
    Q_OBJECT
        Q_PROPERTY(QColor decoration READ decoration WRITE setDecoration)
        Q_PROPERTY(QColor titleColor READ titleColor WRITE setTitleColor)
        Q_PROPERTY(QColor timeColor READ timeColor WRITE setTimeColor)
        Q_PROPERTY(QPixmap closeButtonIcon READ closeButtonIcon WRITE setCloseButtonIcon)
        Q_PROPERTY(bool touchControlStyle READ touchControlStyle WRITE setTouchControlStyle)
public:
    QCusFluentMessageTipView(QCusFluentNavigationBar* parent);
    ~QCusFluentMessageTipView();

    virtual void setModel(QCusFluentAbstractMessageTipModel* model);
    virtual QCusFluentAbstractMessageTipModel* model() const;
    void setDecoration(const QColor& color);
    const QColor& decoration() const;
    void setTitleColor(const QColor& color);
    const QColor& titleColor() const;
    void setTimeColor(const QColor& color);
    const QColor& timeColor() const;
    void setCloseButtonIcon(const QPixmap& icon);
    const QPixmap& closeButtonIcon() const;
    void setTouchControlStyle(bool flag);
    bool touchControlStyle() const;
signals:
    void closeButtonClicked(const QModelIndex& index);
    void messageClicked(QCusFluentMessageTipDataPtr message);
    bool aboutToCloseMessage(QCusFluentMessageTipDataPtr message);
    bool aboutToClearAllMessages();
public slots:
    void clearAll();

protected:
    void resizeEvent(QResizeEvent*) override;
    void showEvent(QShowEvent*) override;
    bool eventFilter(QObject* o, QEvent* e) override;

private slots:
    void onCloseButtonClicked(const QModelIndex& index);
    void onTitleClicked(const QModelIndex& index);

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
