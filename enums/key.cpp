#include "key.hpp"

char key::toChar(Key key)
{
    if(key >= 32 && key <= 126) return (char)key;
    return (char)0;
}