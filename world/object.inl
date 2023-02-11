
template<class T>
T *Object::removeComponentFromVector(std::vector<T *> &vector, const std::string& name)
{
    for (unsigned long i = 0; i < vector.size(); ++i)
    {
        if(vector[i]->name() == name)
        {
            T *component = vector[i];
            vector.erase(vector.begin() + i);
            return component;
        }
    }
    return nullptr;
}