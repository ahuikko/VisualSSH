#ifndef LAYER_MANAGER_H
#define LAYER_MANAGER_H


#pragma once

#include <vector>
#include <memory>
#include "layer.h"
#include <string>

class LayerManager
{

private:
	std::vector<std::unique_ptr<Layer>> _layers;

public:

	LayerManager();

	void updateLayers();

	void ReorderLayer(int draggedIndex, int targetIndex);

	void addLayer(Layer* layer) {
		_layers.emplace_back(layer);
	}

	void addLayer(std::unique_ptr<Layer> layer) {
		_layers.push_back(std::move(layer));
	}

	std::vector<std::unique_ptr<Layer>>& getLayers() {
		return _layers;
	}

};

#endif
