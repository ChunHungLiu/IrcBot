#pragma once

#define _USE_MATH_DEFINES
#include <cmath>

#include <forward_list>
#include <list>
#include <map>
#include <queue>
#include <iostream>
#include <random>
#include <sstream>

#include <enet/enet.h>

#include <boost/signals.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>

#include <Box2D/Box2D.h>
#define STEAMWORKS_CLIENT_INTERFACES

#include <Thor/Particles.hpp>
#include <Thor/Resources.hpp>
#include <Thor/Math/Random.hpp>
#include <Thor/Vectors.hpp>

#include <Framework/Network/Packet.h>

#include <Steamworks.h>
#include <EasySteam/Interface.hpp>

template <class T>
bool operator==(std::shared_ptr<T> a, std::shared_ptr<T> b)
{
	return a.get() == b.get();
}