#include "UiLayerColorPicker.h"

#include <QColorDialog>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <cassert>

UiLayerColorPicker::UiLayerColorPicker(QWidget* p_Parent/* = 0*/)
    : QWidget(p_Parent)
    , m_NumLayers(1)
{
    PadColorsToNumLayers();
    SetColor(0, Qt::white);
}

UiLayerColorPicker::~UiLayerColorPicker()
{
}

void UiLayerColorPicker::setNumLayers(size_t p_NumLayers)
{
    m_NumLayers = std::max(size_t(1), p_NumLayers);
    PadColorsToNumLayers();
    update();
}

void UiLayerColorPicker::SetHighlightedLayer(size_t p_Idx)
{
    m_HighlightedLayer = p_Idx;
}

QColor UiLayerColorPicker::ColorFromLayer(size_t p_LayerIdx) const
{
    if (p_LayerIdx < m_Colors.size()) return m_Colors[p_LayerIdx];
    return Qt::black;
}
#include <QDebug>
void UiLayerColorPicker::SetColor(size_t p_LayerIdx, const QColor& p_Color)
{
    if (p_LayerIdx > m_NumLayers)
    {
        m_NumLayers = p_LayerIdx;
        PadColorsToNumLayers();
    }
    m_Colors[p_LayerIdx] = p_Color;
    for (size_t i = m_NumLayers - 1; i < m_NumLayers; --i)
    {
        qDebug() << m_Colors[i];
    }
    update();
}

void UiLayerColorPicker::paintEvent(QPaintEvent* p_Event)
{
    assert(m_Colors.size() == m_NumLayers);
    QRect totalRect = contentsRect();
    totalRect -= QMargins(1, 1, 1, 1);

    // aim to get each overlapping area have 1/3 width/height visible.
    QRect subRect(QPoint(), totalRect.size() * 3 / (m_NumLayers + 2));

    // overlap params - may be determined by custom layering orientation later
    const QPoint startTL = totalRect.topRight() - QPoint(subRect.width(), 0); //TR to BL layering
    const qreal xOffset = -subRect.width() / 3.0;
    const qreal yOffset = subRect.height() / 3.0;

    QPainter p(this);
    QPointF currentTL = startTL;

    for (size_t i = m_NumLayers - 1; i < m_NumLayers; --i)
    {
//        p.setPen(QPen((m_HighlightedLayer == i ? highlightBrush : QBrush(Qt::darkGray)), 3.0));
        p.setPen(Qt::black);
        p.setBrush(m_Colors[i]);
        p.drawRect(QRectF(currentTL, subRect.size()));
        currentTL += QPointF(xOffset, yOffset);
    }
}
void UiLayerColorPicker::mousePressEvent(QMouseEvent* p_Event)
{
    QRect totalRect = contentsRect();
    totalRect -= QMargins(1, 1, 1, 1);

    // aim to get each overlapping area have 1/3 width/height visible.
    QRect subRect(QPoint(), totalRect.size() * 3 / (m_NumLayers + 2));

    const QPoint startTL = totalRect.topRight() - QPoint(subRect.width(), 0); //TR to BL layering
    const qreal xOffset = -subRect.width() / 3.0;
    const qreal yOffset = subRect.height() / 3.0;

    QPointF currentTL = startTL;

    size_t idx = m_NumLayers;
    for (size_t i = m_NumLayers - 1; i < m_NumLayers; --i)
    {
        if (QRectF(currentTL, subRect.size()).contains(p_Event->pos()))
        {
            idx = i;
            // dont break, layers above can override.
        }
        currentTL += QPointF(xOffset, yOffset);
    }
    if (idx < m_NumLayers)
    {
        QColor newColor = QColorDialog::getColor(ColorFromLayer(idx), this);
        qDebug() << "New color" << newColor;
        SetColor(idx, newColor);
    }
}

void UiLayerColorPicker::PadColorsToNumLayers()
{
    if (m_Colors.size() == m_NumLayers) return;

    QColor lastColor = Qt::black;
    while(m_Colors.size() < m_NumLayers)
    {
        m_Colors.push_back(lastColor);
        lastColor = Qt::white;
    }

    if (m_Colors.size() == m_NumLayers) return;
    m_Colors.resize(m_NumLayers);
}
