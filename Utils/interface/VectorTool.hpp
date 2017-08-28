#ifndef __VECTORTOOL__
#define __VECTORTOOL__

#include <vector>

template <typename T>
class VectorTool{

    public:
        VectorTool () {};
        ~VectorTool () {};

        void Append(const std::vector<T>& invector) {
            for (const auto& it : invector) _combinedvector.emplace_back(it);
        }
        std::vector<T> Output() const {return _combinedvector;}

    private:
        std::vector<T> _combinedvector;
};

#endif
