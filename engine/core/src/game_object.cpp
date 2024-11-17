#include "game_object.h"

namespace Pyro
{

    GameObject GameObject::createObject() { 
        static unsigned int curr = 0;
        return GameObject(curr++);
    }

    unsigned int GameObject::id() const { return id_; }

    GameObject::GameObject(unsigned int id) : id_(id) {}
};
