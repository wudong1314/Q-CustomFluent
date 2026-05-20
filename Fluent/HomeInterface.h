#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QPainter>
#include <QPainterPath>
#include <QLinearGradient>

#include "ScrollArea.h"

class LinkCardView;
class SampleCardView;

class BannerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BannerWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QVBoxLayout *_vBoxLayout;
    QLabel *_galleryLabel;
    QPixmap _banner;
    LinkCardView *_linkCardView;

    void setupUI();
    void setupLinks();
};

class HomeInterface : public ScrollArea
{
    Q_OBJECT

public:
    explicit HomeInterface(QWidget *parent = nullptr);

private:
    BannerWidget *_banner;
    QWidget *m_view;
    QVBoxLayout *_vBoxLayout;

    void initWidget();
    void loadSamples();
};
