#pragma once
#include <QWidget>

class UiColorDialog;

/**
 * Superimposed color picker layers (like GIMP/Photoshop)
 */
class UiLayerColorPicker : public QWidget
{
public:
    UiLayerColorPicker(QWidget* p_Widget = 0);
    ~UiLayerColorPicker();

    Q_PROPERTY(size_t NumLayers READ NumLayers WRITE setNumLayers MEMBER m_NumLayers)
    size_t NumLayers() const { return m_NumLayers; }
    void setNumLayers(size_t p_NumLayers);

    void SetHighlightedLayer(size_t p_Idx); ///< set >= NumLayers to deselect all layers

    QColor ColorFromLayer(size_t p_Layer) const;
    void SetColor(size_t p_LayerIdx, const QColor& p_Color);

private:
    virtual void paintEvent(QPaintEvent* p_Event);
    virtual void mousePressEvent(QMouseEvent* p_Event);

    void PadColorsToNumLayers();
private:
    size_t m_NumLayers;
    std::vector<QColor> m_Colors;
    UiColorDialog* m_Dialog;
    size_t m_HighlightedLayer;
};
