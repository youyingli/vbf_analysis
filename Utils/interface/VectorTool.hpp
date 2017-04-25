#ifndef __VECTORTOOL__
#define __VECTORTOOL__

#include <vector>

template <typename T>
class VectorTool{

    public:
        VectorTool(){};
        ~VectorTool(){};

        void Append(std::vector<T> invector) {
            for (const auto& it : invector) combinedvector.emplace_back(it);
        }
        std::vector<T> Output() { return combinedvector; }

    private:
        std::vector<T> combinedvector;
};

#endif
