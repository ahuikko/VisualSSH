#include "layer_manager.h"
#include <algorithm>    // std::iter_swap

LayerManager::LayerManager()
{
}

void LayerManager::updateLayers()
{
    for (auto& layer : _layers)
    {
        if (layer) 
        {
            layer->Update(); 
        }
    }

}

void LayerManager::ReorderLayer(int draggedIndex, int targetIndex) {
    if (draggedIndex < 0 || draggedIndex >= _layers.size() ||
        targetIndex < 0 || targetIndex >= _layers.size() ||
        draggedIndex == targetIndex) return; // Validate indices

    auto layer = std::move(_layers[draggedIndex]); // Get the layer to move
    _layers.erase(_layers.begin() + draggedIndex); // Remove it from its old position
    _layers.insert(_layers.begin() + targetIndex, std::move(layer)); // Insert it into its new position
}
