#pragma once

#include "model/model.hpp"

class Controller
{
public:
	virtual bool init() = 0;
	virtual void loop() = 0;
};