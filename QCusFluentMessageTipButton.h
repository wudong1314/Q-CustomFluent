#pragma once


#include <QAbstractButton>

#include <memory>

class QCusFluentAbstractMessageTipModel;
class QCusFluentAbstractMessageTipView;
class QStyleOptionButton;

class QCusFluentMessageTipButton : public QAbstractButton
{
    Q_OBJECT
    Q_PROPERTY(QColor tipColor READ tipColor WRITE setTipColor)
public:
    QCusFluentMessageTipButton(QWidget* parent);
    ~QCusFluentMessageTipButton();

    void setModel(QCusFluentAbstractMessageTipModel* model);
    QCusFluentAbstractMessageTipModel* model() const;
    void setView(QCusFluentAbstractMessageTipView* view);
    QCusFluentAbstractMessageTipView* view() const;
    void setTipColor(const QColor& color);
    const QColor& tipColor() const;

private:
    void paintEvent(QPaintEvent* event) override;
    QSize sizeHint() const override;
    void connectView();
    void initStyleOption(QStyleOptionButton* opt);

private slots:
    void onClicked(bool);
    void onModelDataChanged();

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
